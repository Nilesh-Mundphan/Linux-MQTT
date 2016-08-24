#ifndef MY_TCP_H_
#define MY_TCP_H_

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 

void *cread(void *p);

int client_init();
int client_connect(const char *ip ,uint16_t port);
size_t client_write(const uint8_t *buff,size_t len);
uint8_t client_read();
uint16_t client_available();
uint8_t client_connected();
void client_disconnect();
void client_stop();

#endif
