#ifndef _WEATHER
#define _WEATHER

#define UPDATE_INTERVAL     60          // Update weather status every 60 seconds

struct CityReport {
    char *city;
    char *description;
    float temp;
    float humidity;
    float windSpeed;
    float windDegree;
};

struct City {
    struct CityReport *report;
    struct City *next;
};

#endif