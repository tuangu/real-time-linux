/**
 * File: weather.h
 * Define data structure and other constants.
 */

#ifndef _WEATHER_H
#define _WEATHER_H

#include <stdio.h>
#include <stdlib.h>

#define handle_error(msg) \
    do { perror(msg); exit(1); } while (0)

#define UPDATE_INTERVAL     60  // Update weather status every 60 seconds

struct CityReport {
    char cmd[8];                // START, DATA, END
    char city[32];
    char description[32];
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