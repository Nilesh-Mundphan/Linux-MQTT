#include <stdio.h>
#include "buffer.h"

#define BUFFER_SIZE 1024

volatile uint8_t buffer[BUFFER_SIZE];
volatile int16_t buffer_head=-1,buffer_tail=-1;

void buffer_fill(uint8_t* buff,int len)
{
	uint16_t i;
	for(i=0;i<len;i++)
	{
		buffer_add(buff[i]);
	}
}

void buffer_add(uint8_t ch)
{
	uint8_t rxData=ch;

	if((buffer_tail == BUFFER_SIZE-1 && (buffer_head == 0)) || ((buffer_tail + 1) == buffer_head))
	{
		buffer_head++;
	}
			
	if(buffer_head == BUFFER_SIZE || buffer_head == -1)
		buffer_head=0;
	
	if(buffer_tail == BUFFER_SIZE - 1) 
		buffer_tail=0;
	else 
		buffer_tail++;

	buffer[buffer_tail]=rxData;
}

uint8_t buffer_read()
{
	uint8_t ch;

	if(buffer_head == -1)
		return 0;
	ch=buffer[buffer_head];
	
	if(buffer_head == buffer_tail)
	{
		buffer_head = buffer_tail = -1;
	}
	else
	{
		buffer_head++;
     		if(buffer_head == BUFFER_SIZE)
			buffer_head=0;
	}

return ch;
}

uint16_t buffer_available()
{
	if(buffer_head==-1) 
		return 0;
	if(buffer_head < buffer_tail)
		return(buffer_tail - buffer_head + 1);
	else if(buffer_head > buffer_tail)
		return (BUFFER_SIZE - buffer_head + buffer_tail + 1);
	else
	return 1;
}

void buffer_flush()
{
	while(buffer_available())
		buffer_read();
}
