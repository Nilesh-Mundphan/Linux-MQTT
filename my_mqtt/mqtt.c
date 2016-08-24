#include "mqtt.h"
#include "tcp.h"

uint8_t mqtt_buffer[256];
int16_t nextMsgId;
int m_state=MQTT_DISCONNECTED;

void m_ping()
{
	mqtt_buffer[0] = MQTTPINGREQ;
	mqtt_buffer[1] = 0;
	client_write(mqtt_buffer,2);
}

void debug(char * buff,int len)
{
	int i;
	printf("Debug Info \n\n");	
	for (i = 0; i <len; i++) {
		printf("%02x ", (buff[i] & 0xFF));
		if((i!=0) && ((i%16)==0))
		putchar('\n');
	}
putchar('\n');
}

int m_write(uint8_t header, uint8_t* buf, uint16_t length) {
    uint8_t lenBuf[4];
    uint8_t llen = 0;
    uint8_t digit;
    uint8_t pos = 0;
    uint8_t rc;
    uint16_t len = length; 
    do {
        digit = len % 128;
        len = len / 128;
        if (len > 0) {
            digit |= 0x80;
        }
        lenBuf[pos++] = digit;
        llen++;
    } while(len>0);
	
    buf[4-llen] = header;
    int i;
	for (i=0;i<llen;i++) {
        buf[5-llen+i] = lenBuf[i];
    }

    client_write(buf+(4-llen),length+1+llen);	
    //debug(buf+(4-llen),length+1+llen);
}

uint16_t writeString(const char* string, uint8_t* buf, uint16_t pos) {
    const char* idp = string;
    uint16_t i = 0;
    pos += 2;
    while (*idp) {
        buf[pos++] = *idp++;
        i++;
    }
    buf[pos-i-2] = (i >> 8);
    buf[pos-i-1] = (i & 0xFF);
    return pos;
}

uint8_t m_connect(const char *id, const char *user, const char *pass, const char* willTopic, uint8_t willQos, int willRetain, const char* willMessage) {
   
	int result = 1;
        if (result) {
		nextMsgId = 1;
            	uint16_t length = 5;
            	unsigned int j;
	#if MQTT_VERSION == MQTT_VERSION_3_1
            	uint8_t d[9] = {0x00,0x06,'M','Q','I','s','d','p', MQTT_VERSION};
		#define MQTT_HEADER_VERSION_LENGTH 9
	#elif MQTT_VERSION == MQTT_VERSION_3_1_1
            	uint8_t d[7] = {0x00,0x04,'M','Q','T','T',MQTT_VERSION};
		#define MQTT_HEADER_VERSION_LENGTH 7
	#endif

            	for (j = 0;j<MQTT_HEADER_VERSION_LENGTH;j++) {
                	mqtt_buffer[length++] = d[j];
            	}

            	uint8_t v;
            	if (willTopic) {
                	v = 0x06|(willQos<<3)|(willRetain<<5);
            	} else {
                	v = 0x02;
            	}

            	if(user != NULL) {
                	v = v|0x80;

                	if(pass != NULL) {
                    		v = v|(0x80>>1);
                }
            }

            mqtt_buffer[length++] = v; 
            mqtt_buffer[length++] = ((MQTT_KEEPALIVE) >> 8);
            mqtt_buffer[length++] = ((MQTT_KEEPALIVE) & 0xFF);

            length = writeString(id,mqtt_buffer,length);
            
	    if (willTopic) {
                length = writeString(willTopic,mqtt_buffer,length);
                length = writeString(willMessage,mqtt_buffer,length);
            }

            if(user != NULL) {
                length = writeString(user,mqtt_buffer,length);
                if(pass != NULL) {
                    length = writeString(pass,mqtt_buffer,length);
                }
            }
	    
		m_write(MQTTCONNECT,mqtt_buffer,length-5);
		sleep(1);
		while(!client_available());
		uint8_t llen;
            	uint16_t len = m_readPacket(&llen);
		debug(mqtt_buffer,len);
            	if (len == 4) {
                	if (mqtt_buffer[3] == 0) {
                    		m_state = MQTT_CONNECTED;
				printf("Connected To Server\n\n");
                    		return true;
                	} else {
                    		m_state = mqtt_buffer[3];
                	}
            	}
	}
   return true;
}

uint8_t m_publish(const char* topic, const uint8_t* payload, unsigned int plength, int retained) {
    
	if(m_connected()){
        if (MQTT_MAX_PACKET_SIZE < 5 + 2+strlen(topic) + plength) {
            // Too long
            return false;
        }
        // Leave room in the mqtt_buffer for header and variable length field
        uint16_t length = 5;
        length = writeString(topic,mqtt_buffer,length);
        uint16_t i;
        for (i=0;i<plength;i++) {
            mqtt_buffer[length++] = payload[i];
        }
        uint8_t header = MQTTPUBLISH;
        if (retained) {
            header |= 1;
        }
		return m_write(header,mqtt_buffer,length-5);
    }
    return false;
}

uint8_t m_subscribe(const char* topic, uint8_t qos) {
    if (qos < 0 || qos > 1) {
        return false;
    }
    if (MQTT_MAX_PACKET_SIZE < 9 + strlen(topic)) {
        // Too long
        return false;
    }
    if (m_connected()) {
        // Leave room in the mqtt_buffer for header and variable length field
        uint16_t length = 5;
        nextMsgId++;
        if (nextMsgId == 0) {
            nextMsgId = 1;
        }
        mqtt_buffer[length++] = (nextMsgId >> 8);
        mqtt_buffer[length++] = (nextMsgId & 0xFF);
        length = writeString((char*)topic, mqtt_buffer,length);
        mqtt_buffer[length++] = qos;
        return m_write(MQTTSUBSCRIBE|MQTTQOS1,mqtt_buffer,length-5);
    }
    return false;
}

void responce_type(uint8_t * buff,int llen)
{
	uint8_t type = buff[0]&0xF0,llen1=1;
   	char *payload;

	if (type == MQTTPUBLISH) 
	{
        	uint16_t topic_len=(buff[llen1+1]<<8)+buff[llen1+2];
		char topic[topic_len+1];

        	uint8_t payload_len=llen-topic_len-4;     	
		int i=0;

        	for(i=0;i<topic_len;i++)
        	{
			topic[i]=buff[llen1+3+i];
		}    

		topic[topic_len]=0;
	
		printf("Topic: %s \n",topic);
       		payload= buff+llen1+3+topic_len;

        	printf("Payload: ");

        	for(i=0;i<payload_len;i++)
        	{
               		printf("%c",payload[i]);
        	}
  
	printf("\n");
	}
	else if(type == MQTTPINGREQ)
	{
		mqtt_buffer[0] = MQTTPINGRESP;
                mqtt_buffer[1] = 0;
		client_write(mqtt_buffer,2);
	}	
	else if(type == MQTTPINGRESP)
	{
		printf("Connection is Alive\n");
	}
	else if(type == MQTTSUBACK)
        {
                printf("Subscription is Successful\n");
        }
	
}


uint8_t m_connected(){
	return client_connected();
}


void m_disconnect(){
	mqtt_buffer[0] = MQTTDISCONNECT;
    	mqtt_buffer[1] = 0;
    	client_write(mqtt_buffer,2);
    	m_state = MQTT_DISCONNECTED;
}

uint8_t m_readByte() {
    while(!client_available()) {}
    return client_read();
}

uint16_t m_readPacket(uint8_t* lengthLength) {
    uint16_t len = 0;
    mqtt_buffer[len++] = m_readByte();
    int isPublish = (mqtt_buffer[0]&0xF0) == MQTTPUBLISH;
    uint32_t multiplier = 1;
    uint16_t length = 0;
    uint8_t digit = 0;
    uint16_t skip = 0;
    uint8_t start = 0;

    do {
        digit = m_readByte();
        mqtt_buffer[len++] = digit;
        length += (digit & 127) * multiplier;
        multiplier *= 128;
    } while ((digit & 128) != 0);
    
	*lengthLength = len-1;

    if (isPublish) {
        // Read in topic length to calculate bytes to skip over for Stream writing
        mqtt_buffer[len++] = m_readByte();
        mqtt_buffer[len++] = m_readByte();
        skip = (mqtt_buffer[*lengthLength+1]<<8)+mqtt_buffer[*lengthLength+2];
        start = 2;
        if (mqtt_buffer[0]&MQTTQOS1) {
            // skip message id
            skip += 2;
        }
    }

	uint16_t i;
    for (i = start;i<length;i++) {
        digit = m_readByte();

        if (len < MQTT_MAX_PACKET_SIZE) {
            mqtt_buffer[len] = digit;
        }
        len++;
    }

    return len;
}

