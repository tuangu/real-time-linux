/**
 * File: http.h
 * Handle http request to get weather data using socket.
 * Also parse the response data to get meaningful information.
 */

#ifndef _WEATHER_HTTP_H
#define _WEATHER_HTTP_H

#include <unistd.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "parson/parson.h"
#include "weather.h"

#define MAX_REQUEST_BUFFER  256
#define MAX_RESPONSE_BUFFER 1024

// Sampe request:
// http://api.openweathermap.org/data/2.5/weather?q=CITY_NAME&units=metric&appid=APIKEY
const char *apiKey = "1b08a86400f561eba65ea5ef384aa3e2";
const char *host = "api.openweathermap.org";
const in_port_t port = 80;
const char *requestFmt = 
    "GET /data/2.5/weather?q=%s&units=metric&appid=%s " // city, api key
    "HTTP/1.1\r\n"
    "Host: %s\r\n"                                      // hostname
    "\r\n";

// Client Communication Unix
// socket() -> connect() -> send() -> recv() -> close()
//   Create a socket
//   Establish connection
//   Communicate
//   Close the connection
//
// Reference:
// http://www.csd.uoc.gr/~hy556/material/tutorials/cs556-3rd-tutorial.pdf
// https://gist.github.com/nolim1t/126991

//Initialize a socket
int socketConnect(const char *host, in_port_t port) {
	struct hostent *hp;
	struct sockaddr_in addr;
	int on = 1, sock;     

	if((hp = gethostbyname(host)) == NULL)
		handle_error("gethostbyname()");

	bcopy(hp->h_addr_list[0], &addr.sin_addr, hp->h_length);
	addr.sin_port = htons(port);
	addr.sin_family = AF_INET;
	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (const char *)&on, sizeof(int));

	if(sock == -1)
		handle_error("setsockopt()");
	
	if(connect(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1)
        handle_error("connect()");

    return sock;
}

// Request weather data 
// Data is returned in the response parameter
void requestData(const char* city, char *response) {
    char request[MAX_REQUEST_BUFFER];

    // Construct request header based on city name
    snprintf(request, MAX_REQUEST_BUFFER, requestFmt, city, apiKey, host);
    // printf("%s", request);

    // Connect and send request
    int fd = socketConnect(host, port);
    write(fd, request, strlen(request));

    // Handle reponse
    bzero(response, MAX_RESPONSE_BUFFER);
    read(fd, response, MAX_RESPONSE_BUFFER - 1);

    shutdown(fd, SHUT_RDWR);
    close(fd);
}

// Sample response:
// OK response
    // HTTP/1.1 200 OK
    // Server: openresty
    // Date: Sun, 27 May 2018 18:08:21 GMT
    // Content-Type: application/json; charset=utf-8
    // Content-Length: 428
    // Connection: keep-alive
    // X-Cache-Key: /data/2.5/weather?q=espoo&units=metric
    // Access-Control-Allow-Origin: *
    // Access-Control-Allow-Credentials: true
    // Access-Control-Allow-Methods: GET, POST
    //
    // {"coord":{"lon":24.66,"lat":60.22},"weather":
    // [{"id":800,"main":"Clear","description":"clear sky","icon":"01d"}],
    // "base":"stations","main":{"temp":19,"pressure":1028,"humidity":78,"temp_min":19,"temp_max":19},
    // "visibility":10000,"wind":{"speed":4.6,"deg":30},"clouds":{"all":0},"dt":1527443400,
    // "sys":{"type":1,"id":5019,"message":0.0026,"country":"FI","sunrise":1527383745,"sunset":1527448972},
    // "id":660129,"name":"Espoo","cod":200}
// Not Found response
    // HTTP/1.1 404 Not Found
    // Server: openresty
    // Date: Sun, 27 May 2018 18:08:21 GMT
    // Content-Type: application/json; charset=utf-8
    // Content-Length: 40
    // Connection: keep-alive
    // X-Cache-Key: /data/2.5/weather?q=espooq&units=metric
    // Access-Control-Allow-Origin: *
    // Access-Control-Allow-Credentials: true
    // Access-Control-Allow-Methods: GET, POST
    // 
    // {"cod":"404","message":"city not found"}
//
// Return: 
//      0 if success, weather data is in report
//      -1 if error (invalid city name, error in parsing response, etc.), ignore report
int parseResponse(const char *response, struct CityReport *report) {
    char *json;
    JSON_Value *rootValue;
    JSON_Object *rootObj;
    JSON_Array *weather;
    int cod;

    // Clean up reponse, only json part left
    char *resBegin;
    char jsonTargetBegin = '{';
    if ((resBegin = strchr(response, jsonTargetBegin)) != NULL) {
        json = resBegin;
    } else {
        // Invalid reponse
        // Should not reach here
        return -1;
    }
#if 0
    printf("dump json\n");
    printf("%s\n", json);
#endif
    rootValue = json_parse_string(json);
    rootObj = json_value_get_object(rootValue);
    cod = json_object_get_number(rootObj, "cod");
    if (cod != 200) {
        // Error, early return
        return -1;
    }
    strcpy(report->cmd, "DATA");
    // report->city = json_object_get_string(rootObj,"name");
    strcpy(report->city, json_object_get_string(rootObj,"name"));
    weather = json_object_get_array(rootObj, "weather");
    // report->description = json_object_get_string(
    //     json_array_get_object(weather, 0), "description");
    strcpy(report->description, json_object_get_string(
        json_array_get_object(weather, 0), "description"));
    report->temp = json_object_dotget_number(rootObj, "main.temp");
    report->humidity = json_object_dotget_number(rootObj, "main.humidity");
    report->windSpeed = json_object_dotget_number(rootObj, "wind.speed");
    report->windDegree = json_object_dotget_number(rootObj, "wind.deg");

    return 0;
}

#endif