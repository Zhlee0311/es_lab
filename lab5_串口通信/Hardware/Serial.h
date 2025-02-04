#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdbool.h>

void Serial_Init(void);
uint8_t Serial_GetFlag(void);
uint8_t Serial_GetRxpos(void);
bool Serial_FrameHandler(void);

#endif
