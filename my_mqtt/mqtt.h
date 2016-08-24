#ifndef MY_MQTT_H_
#define MY_MQTT_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#define MQTT_VERSION_3_1      3
#define MQTT_VERSION_3_1_1    4
//#define MQTT_VERSION MQTT_VERSION_3_1
#define MQTT_VERSION MQTT_VERSION_3_1_1

#define MQTT_MAX_PACKET_SIZE 128		// MQTT_KEEPALIVE : keepAlive interval in Seconds

#define MQTT_KEEPALIVE 15
#define MQTT_CONNECTION_TIMEOUT     -4
#define MQTT_CONNECTION_LOST        -3
#define MQTT_CONNECT_FAILED         -2
#define MQTT_DISCONNECTED           -1
#define MQTT_CONNECTED               0
#define MQTT_CONNECT_BAD_PROTOCOL    1
#define MQTT_CONNECT_BAD_CLIENT_ID   2
#define MQTT_CONNECT_UNAVAILABLE     3
#define MQTT_CONNECT_BAD_CREDENTIALS 4
#define MQTT_CONNECT_UNAUTHORIZED    5

#define MQTTCONNECT     (1 << 4)  		// Client request to connect to Server
#define MQTTCONNACK     (2 << 4)  		// Connect Acknowledgment
#define MQTTPUBLISH     (3 << 4)  		// Publish message
#define MQTTPUBACK      (4 << 4)  		// Publish Acknowledgment
#define MQTTPUBREC      (5 << 4)  		// Publish Received (assured delivery part 1)
#define MQTTPUBREL      (6 << 4)  		// Publish Release (assured delivery part 2)
#define MQTTPUBCOMP     (7 << 4)  		// Publish Complete (assured delivery part 3)
#define MQTTSUBSCRIBE   (8 << 4)  		// Client Subscribe request
#define MQTTSUBACK      (9 << 4)  		// Subscribe Acknowledgment
#define MQTTUNSUBSCRIBE (10 << 4) 		// Client Unsubscribe request
#define MQTTUNSUBACK    (11 << 4) 		// Unsubscribe Acknowledgment
#define MQTTPINGREQ     (12 << 4) 		// PING Request
#define MQTTPINGRESP    (13 << 4) 		// PING Response
#define MQTTDISCONNECT  (14 << 4) 		// Client is Disconnecting
#define MQTTReserved    (15 << 4) 		// Reserved

#define MQTTQOS0        (0 << 1)
#define MQTTQOS1        (1 << 1)
#define MQTTQOS2        (2 << 1)

#define true  1
#define false 0

void debug(char * buff,int len);
void responce_type(uint8_t * buff,int llen);

uint16_t writeString(const char* string, uint8_t* buf, uint16_t pos);
int m_write(uint8_t header, uint8_t* buf, uint16_t length);
uint8_t m_connect(const char *id, const char *user, const char *pass, const char* willTopic, uint8_t willQos, int willRetain, const char* willMessage);
uint8_t m_publish(const char* topic, const uint8_t* payload, unsigned int plength, int retained);
uint8_t m_subscribe(const char* topic, uint8_t qos);
void m_ping();

uint8_t m_readByte();
uint16_t m_readPacket(uint8_t* lengthLength);

uint8_t m_connected();
void m_disconnect();

#endif
