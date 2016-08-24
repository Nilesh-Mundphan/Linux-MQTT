#ifndef N_BUFFER_H_
#define N_BUFFER_H_

#include <stdint.h>

void buffer_fill(uint8_t* buff,int len);

void buffer_add(uint8_t ch);
uint8_t buffer_read();
uint16_t buffer_available();
void buffer_flush();

#endif
