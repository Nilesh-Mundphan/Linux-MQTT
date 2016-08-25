#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "buffer.h"
#include "mqtt.h"

const char* mqtt_server = "184.106.153.149";//"io.adafruit.com"

int main(int argc, char *argv[])
{
	int rd[256];
	char req[]="GET /update?api_key=V35R55DSYIIIACCR&field1=22\n";
	client_init();
	printf("Connecting to IO\n");
        client_connect(mqtt_server,80);

	printf("Connecting to USER\n");
        printf("Buffer Ava :%d \n",buffer_available());
	sleep(5);
	//client_write("GET / HTTP/1.1\n\n");

	client_write(req,sizeof(req)); 
	while(1){
            if(client_available()){
		sleep(1);
 		printf("Buffer Ava :%d \n",client_available());
		while(client_available())
		{
			printf("%c",client_read());
		}
		printf("\n");
		buffer_flush();
	    }    
        }	
	


 return 0;
}

