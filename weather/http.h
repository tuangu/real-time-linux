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

void requestData(const char* city, char *response) {
    char request[MAX_REQUEST_BUFFER];

    // Construct request header based on city name
    snprintf(request, MAX_REQUEST_BUFFER, requestFmt, city, apiKey, host);
    printf("%s", request);

    // Connect and send request
    int fd = socketConnect(host, port);
    write(fd, request, strlen(request));

    // Handle reponse
    bzero(response, MAX_RESPONSE_BUFFER);
    read(fd, response, MAX_RESPONSE_BUFFER - 1);

    shutdown(fd, SHUT_RDWR);
    close(fd);
}

#endif