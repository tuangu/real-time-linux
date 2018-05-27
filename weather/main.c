#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "weather.h"

// Global stuff
int updateInterval = UPDATE_INTERVAL;
struct City *cityListHead = NULL;       // Head of the city's linked-list


//========== Command line arguments ==========//

/**
 * Replace _ in input name with space.
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
            // Add cities to list of cities
            if (cityListHead == NULL) {
                char *cityName = argv[i];
                normalizeCityName(cityName);

                cityListHead = malloc(sizeof(struct City));
                cityListHead->report = malloc(sizeof(struct CityReport));
                cityListHead->report->city = cityName;
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
                child->report->city = cityName;
                child->next = NULL;
            }
        }

    }
}

int main(int argc, char *argv[]) {
    
    parseCommand(argc, argv);
    
    printf("Update interval %d\n", updateInterval);
    struct City *list = cityListHead;
    while (list != NULL) {
        printf("City name: %s\n", list->report->city);
        list = list->next;
    }

    return 0;
}