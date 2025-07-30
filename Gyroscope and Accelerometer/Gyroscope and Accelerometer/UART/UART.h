#ifndef UART_H_
#define UART_H_

#include "../REGISTER/REGISTER.h"

#define F_CPU     8000000UL
#define BAUD_RATE 9600

void UART_init(void);
void UART_sendByte(uint8_t data);
uint8_t UART_receiveByte(void);
void UART_sendString(const char* str);
void UART_receiveString(char* buffer, char terminator);

#endif /* UART_H_ */