#include "../UART/UART.h"

void UART_init(void){
	uint16_t ubrr_value = (F_CPU / (16UL * BAUD_RATE)) - 1;

	UBRRL = (uint8_t)ubrr_value;
	UBRRH = (uint8_t)(ubrr_value >> 8);
	UCSRB = (1 << RXEN) | (1 << TXEN);
	UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);
}

void UART_sendByte(uint8_t data){
	while (!(UCSRA & (1 << UDRE)));
	UDR = data;
}

uint8_t UART_receiveByte(void){
	while (!(UCSRA & (1 << RXC)));
	return UDR;
}

void UART_sendString(const char* str){
	while (*str){
		UART_sendByte(*str++);
	}
}

void UART_receiveString(char* buffer, char terminator){
	uint8_t i = 0;
	char c;
	do {
		c = UART_receiveByte();
		buffer[i++] = c;
	} while (c != terminator);
	buffer[i - 1] = '\0';
}
