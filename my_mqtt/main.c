#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mqtt.h"

#define DEBUG 1

#define AIO_USERNAME    "nilesh_mundphan"
#define AIO_KEY         "98a49d6201df44b393a8ce0c066d2ce4"
#define PUB_PATH        "nilesh_mundphan/feeds/access_place"
#define SUB_PATH        "nilesh_mundphan/feeds/button"


const char* mqtt_server = "52.70.203.194";//"io.adafruit.com"

int main(int argc, char *argv[])
{
	char b[]="Ubuntu Client Connected";
	client_init();
	printf("Connecting to IO\n");
        client_connect(mqtt_server,1883);

	printf("Connecting to USER\n");
        
	m_connect("UbuntuClient",AIO_USERNAME,AIO_KEY,0,0,0,0);
	sleep(2);
	m_subscribe(SUB_PATH,0);
    sleep(2);
	m_publish(PUB_PATH,b,strlen(b),0);
	while(1)
	{
		//sprintf(b,"Welcome # %d",var++);
		//m_publish(PUB_PATH,b,strlen(b),0);
		m_ping();
		sleep(10);
	}	

 return 0;
}

