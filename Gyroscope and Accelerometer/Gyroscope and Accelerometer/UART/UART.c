#include "../UART/UART.h"
/*
	This function sets up the UART peripheral for serial communication.
	It uses a predefined F_CPU (CPU clock frequency) and BAUD_RATE to set the correct baud rate.
*/
void UART_init(void){
	uint16_t ubrr_value = (F_CPU / (16UL * BAUD_RATE)) - 1; //This calculates the UBRR (USART Baud Rate Register) value using the formula from the datasheet:
	UBRRL = (uint8_t)ubrr_value; //Writes the lower 8 bits of ubrr_value to the USART Baud Rate Register Low (UBRRL).
	UBRRH = (uint8_t)(ubrr_value >> 8); //Writes the upper 8 bits of ubrr_value to the USART Baud Rate Register High (UBRRH).
	/*	
		RXEN: The receiver, to allow receiving data over UART.
		TXEN: The transmitter, to allow sending data over UART.
	*/
	UCSRB = (1 << RXEN) | (1 << TXEN);
	/*
		Configures the USART Control and Status Register C (UCSRC):
		URSEL: Must be set to write to UCSRC instead of UBRRH (on ATmega32).
		UCSZ1 and UCSZ0: Set the frame format to 8-bit data.
		This combination (UCSZ1=1, UCSZ0=1) selects 8 data bits (most common setting).
	*/
	UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);
}

void UART_sendByte(uint8_t data){ //send 8-bit 
	/*
		This waits until the UART Data Register is empty and ready to accept new data:
		UCSRA is the USART Control and Status Register A.
		UDRE (USART Data Register Empty) is a flag bit.
		UDRE = 1 means the previous data was already transmitted, and UDR is ready for a new byte.
		The loop blocks (waits) until UDRE becomes 1.
	*/
	while (!(UCSRA & (1 << UDRE)));
	/*
		Writes the byte to the USART Data Register (UDR).
		This starts the transmission of data over the TX (Transmit) line.
	*/
	UDR = data;
}

uint8_t UART_receiveByte(void){
	while (!(UCSRA & (1 << RXC))); //Waits until a byte is received via UART
	return UDR; //Returns the received byte to the caller
}

/*
	Declares a function that takes a pointer to a constant character array (const char* str).
	The string must be null-terminated (i.e., ends with '\0').
	Returns nothing (void).
*/

void UART_sendString(const char* str){
	while (*str){ //Loops until end of the string ('\0')
		UART_sendByte(*str++); //Sends each character one by one
	}
}

/*
	This function takes:
		buffer: a pointer to a character array where the received string will be stored.
		terminator: the character that signals the end of the string (e.g., newline '\n', hash '#', etc.).
	It does not return a value, but it modifies buffer to store the received string.
*/

void UART_receiveString(char* buffer, char terminator){
	uint8_t i = 0;
	char c;
	do {
		c = UART_receiveByte(); //Reads a character from UART
		buffer[i++] = c; //Stores it in the buffer
	} while (c != terminator); //Continues until the terminator is received
	buffer[i - 1] = '\0'; //Replaces the terminator with '\0' to end the string
}
// we can extend buffer length by maxlength to prevent overflow.