#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "buffer.h"
#include "mqtt.h"

const char* mqtt_server = "216.58.220.14";//"io.adafruit.com"

int main(int argc, char *argv[])
{
	int rd[256];
	client_init();
	printf("Connecting to IO\n");
    client_connect(mqtt_server,80);

	printf("Connecting to USER\n");
    printf("Buffer Ava :%d \n",buffer_available());
	sleep(5);
	client_write("GET / HTTP/1.1\n\n");
	 
	while(1){
            if(buffer_available()){
		sleep(1);
 		printf("Buffer Ava :%d \n",buffer_available());
		buffer_flush();
	    }    
        }	
	


 return 0;
}

