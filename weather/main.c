#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "weather.h"
#include "http.h"

// Global stuff
int updateInterval = UPDATE_INTERVAL;
struct City *cityListHead = NULL;       // Head of the city's linked-list
pid_t pidWeatherData;
pid_t pidLog;

// Terminal color code
#define COLOR_RESET     "\x1b[0m"
#define COLOR_BOLD      "\x1b[1m"
#define COLOR_WHITE     "\x1b[37m"

//========== Termination ==========//

static void freeList(struct City *head) {
    struct City *node = head;
    while (node != NULL) {
        struct City *temp = node;
        node = node->next;
        free(temp->report);
        free(temp);
    }
    head = NULL;
}

static void byeByeParent(int signo) {
    if (signo == SIGINT) {
        char ch;
        printf("\nDo you want to terminal program? [Y/N]: ");
        scanf("%s", &ch);
        if ((ch == 'Y') || (ch == 'y')) {
            kill(pidWeatherData, SIGUSR1);
            kill(pidLog, SIGUSR1);
            freeList(cityListHead);
            exit(signo);
        } else {
            signal(SIGINT, byeByeParent);
        }
    }
}

static void byeByeChild(int signo) {
    if (signo == SIGINT) {
        // do nothing
    } else if (signo == SIGUSR1) {
        // terminate process
        exit(EXIT_SUCCESS);
    }
}

//========== Command line arguments ==========//

/**
 * Replace `_` in input name with space.
 * Example: "Ho_Chi_Minh_City" becomes "Ho Chi Minh City"
 */
static void normalizeCityName(char *name) {
    char target = '_';
    char replace = ' ';
    while ((name = strchr(name, target)) != NULL) {
        *name = replace;
        ++name;
    }
}

static void parseCommand(int argc, char *argv[]) {
    const char *helpString = 
        "Usage: weather [OPTION]... [CITIES]... \n"
        "Show current weather data for inputted cities \n"
        "\n"
        "  interval=SECONDS     update interval. Default is 60 senconds \n"
        "  --help               show help \n"
        "\n"
        "City name is case insensitive. Invalid city name will be ignored. " 
        "If city name contain spaces, replace them with _. \n"
        "For example: Ho Chi Minh City becomes Ho_Chi_Minh_City \n";

    if (argc < 2) {
        printf("%s", helpString);
        exit(EXIT_SUCCESS);
    }

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0) {
            printf("%s", helpString);
            exit(EXIT_SUCCESS);
        } else if (strncmp(argv[i], "interval=", 9) == 0) {
            int interval = atoi(argv[i] + 9);
            if (interval > 0)
                updateInterval = interval;
        } else {
            // Add cities to a linked-list of cities
            if (cityListHead == NULL) {
                char *cityName = argv[i];
                normalizeCityName(cityName);

                cityListHead = malloc(sizeof(struct City));
                cityListHead->report = malloc(sizeof(struct CityReport));
                // cityListHead->report->city = cityName;
                strcpy(cityListHead->report->city, cityName);
                cityListHead->next = NULL;
            } else {
                struct City *parent = cityListHead;
                while (parent->next != NULL)
                    parent = parent->next;

                char *cityName = argv[i];
                normalizeCityName(cityName);

                parent->next = malloc(sizeof(struct City));
                struct City* child = parent->next;
                child->report = malloc(sizeof(struct CityReport));
                // child->report->city = cityName;
                strcpy(child->report->city, cityName);
                child->next = NULL;
            }
        }

    }
}

//========== Child processes ==========//

static void weatherProcess(pid_t *pidWeatherData, int fdWeatherData[2]) {
    pipe(fdWeatherData);
    *pidWeatherData = fork();

    if(*pidWeatherData < 0) {
        handle_error("fork()");
    } else if (*pidWeatherData == 0) {
        signal(SIGINT, byeByeChild);
        signal(SIGUSR1, byeByeChild);

        // close the read end of the pipe
        close(fdWeatherData[0]);

        char response[MAX_RESPONSE_BUFFER];
        struct CityReport report;
        int ret;
        while (1) {
            // Send START indication
            strcpy(report.cmd, "START");
            write(fdWeatherData[1], &report, sizeof(struct CityReport));

            // Send actual weather data
            struct City *prev = NULL;
            struct City *current = cityListHead;

            while (current != NULL) {
                requestData(current->report->city, response);
                ret = parseResponse(response, &report);
                if (ret == 0) {
                    write(fdWeatherData[1], &report, sizeof(struct CityReport));
                    prev = current;
                    current = current->next;
                } else {
                    // Invalid city name
                    // Remove from the city list
                    prev->next = current->next;
                    free(current->report);
                    free(current);
                    current = prev->next;
                }
            }

            sleep(updateInterval);
        }
    }
}

static void logProcess(pid_t *pidLog, int fdLog[2]) {
    pipe(fdLog);
    *pidLog = fork();

    if (*pidLog < 0) {
        handle_error("pid()");
    } else if (*pidLog == 0) {
        signal(SIGINT, byeByeChild);
        signal(SIGUSR1, byeByeChild);
        
        // close the write end
        close(fdLog[1]);

        // open log file
        FILE *logFile = fopen("/tmp/weather.log", "w");
        if (logFile == NULL)
            handle_error("fopen()");
        
        char logbuffer[256];
        ssize_t nlog;
        while (1) {
            if ((nlog = read(fdLog[0], logbuffer, 256)) > 0) {
                fwrite(logbuffer, sizeof(char), nlog, logFile);
            }
        }

    }
}

// Convert tm time struct to hh:mm:ss
int chartime(const struct tm *time_ptr, char *chtime, int size) {
    return snprintf(chtime, size, "%02d:%02d:%02d", time_ptr->tm_hour, 
        time_ptr->tm_min, time_ptr->tm_sec);
}

int main(int argc, char *argv[]) {
    signal(SIGINT, byeByeParent);
    parseCommand(argc, argv);

    // Weather data process
    int fdWeatherData[2];
    weatherProcess(&pidWeatherData, fdWeatherData);
    close(fdWeatherData[1]);    // close the write end of the pipe

    // Logging process
    char logbuffer[256];
    int fdLog[2];
    logProcess(&pidLog, fdLog);
    close(fdLog[0]);            // close the read end of the pipe    

    time_t now;
    char timebuffer[10];
    int nlog;
    
    while (1) {
        struct CityReport weatherReport;
        now = time(NULL);
        chartime(localtime(&now), timebuffer, 10);

        if (read(fdWeatherData[0], &weatherReport, sizeof(struct CityReport)) > 0) {
            if (strcmp(weatherReport.cmd, "START") == 0) {
                system("clear");
                printf(COLOR_WHITE);
                printf(COLOR_BOLD);
                printf("%-18s%-18s%-8s%-10s%-12s%-12s\n",
                       "City", "Description", "Temp", "Humidity",
                       "Wind Speed", "Wind Degree");
                printf(COLOR_RESET);

                // send log
                nlog = snprintf(logbuffer, 256, "\n%s New weather data received\n", timebuffer);
                write(fdLog[1], logbuffer, nlog);
            } else if (strcmp(weatherReport.cmd, "DATA") == 0) {
                printf("%-18s%-18s%-8.2f%-10.2f%-12.2f%-12.2f\n", 
                weatherReport.city, weatherReport.description, weatherReport.temp, 
                weatherReport.humidity, weatherReport.windSpeed, weatherReport.windDegree);

                // send log
                nlog = snprintf(logbuffer, 256, "%s %-18s %-18s T:%4.2f H:%4.2f WS:%4.2f WD:%4.2f\n", timebuffer, 
                    weatherReport.city, weatherReport.description, weatherReport.temp, 
                    weatherReport.humidity, weatherReport.windSpeed, weatherReport.windDegree);
                write(fdLog[1], logbuffer, nlog);
            }
        }
    }
}