#include "tcp.h" 
#include "buffer.h"
#include "mqtt.h"

int client_state=0;
int sockfd = 0;
pthread_t cls,clr;
struct sockaddr_in serv_addr;

void *cread(void *p)
{
	char recvBuff[1024];
	int n=0;
	memset(recvBuff, '0',sizeof(recvBuff));
	while(1)
	{
	 	n = read(sockfd, recvBuff, sizeof(recvBuff)-1);
         	recvBuff[n] = 0;
	 	if(n==0)
			break;
		//printf("Len: %d \nData: %s\n\n",n,recvBuff);
		buffer_fill(recvBuff,n);
		responce_type(recvBuff,n);
		//debug(recvBuff,n);
	}
 client_state=0;
 printf("#### Connection lost ####\n");
}

int client_init()
{
        memset(&serv_addr, '0', sizeof(serv_addr));
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
}

int client_connect(const char *ip ,uint16_t port)
{
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(port);
        inet_pton(AF_INET, ip, &serv_addr.sin_addr);

        if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        {
                perror("connect");
                return 1;
        }
	client_state=1;
	pthread_create(&clr,0,cread,NULL);
}

size_t client_write(const uint8_t *buff,size_t len)
{
	int n=0;
        n=write(sockfd,buff,len);
	return n;
} 

uint16_t client_available()
{
	return buffer_available();
}

uint8_t client_read()
{
	return buffer_read();
}

uint8_t client_connected()
{
	return client_state;
}

void client_disconnect()
{
	client_state=0;
	client_stop();
}

void client_stop()
{
	close(sockfd);
}
