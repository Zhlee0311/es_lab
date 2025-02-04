#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdbool.h>

void Serial_Init(void);

void Serial_Send(void);

bool Serial_GetRxFlag(void);

uint16_t Serial_GetRxData(void);

#endif
