

#ifndef UART_H_
#define UART_H_

#include "common.h"

void Init_UART0(uint32_t baud_rate);
extern cbuff_t Txbuf, Rxbuf;

#endif /* UART_H_ */
