#ifndef REGISTER_H_
#define REGISTER_H_

#include <avr/sfr_defs.h>

#include "Typedef.h"

#define TIFR *((volatile uint8_t*)0x58)
#define TCCR0 *((volatile uint8_t*)0x53)
#define TCNT0 *((volatile uint8_t*)0x52)

#define PORTA *((volatile uint8_t*)0x3B)
#define DDRA *((volatile uint8_t*)0x3A)
#define PINA *((volatile uint8_t*)0x39)

#define PORTB *((volatile uint8_t*)0x38)
#define DDRB *((volatile uint8_t*)0x37)
#define PINB *((volatile uint8_t*)0x36)

#define PORTC *((volatile uint8_t*)0x35)
#define DDRC *((volatile uint8_t*)0x34)
#define PINC *((volatile uint8_t*)0x33)

#define PORTD *((volatile uint8_t*)0x32)
#define DDRD *((volatile uint8_t*)0x31)
#define PIND *((volatile uint8_t*)0x30)

#define TWBR    _SFR_IO8(0x20)
#define TWSR    _SFR_IO8(0x21)
#define TWDR    _SFR_IO8(0x23)
#define TWCR    _SFR_IO8(0x36)
#define TWAR    _SFR_IO8(0x22)
#define TWEN    2
#define TWSTO   4
#define TWSTA   5
#define TWINT   7

#define sei()  __asm__ __volatile__ ("sei" ::: "memory")
#define cli()  __asm__ __volatile__ ("cli" ::: "memory")

#define TCCR1A *((volatile uint8_t*)0x4F)
#define TCCR1B *((volatile uint8_t*)0x4E)
#define TCNT1H *((volatile uint8_t*)0x4D)
#define TCNT1L *((volatile uint8_t*)0x4C)
#define OCR1AH *((volatile uint8_t*)0x4B)
#define OCR1AL *((volatile uint8_t*)0x4A)
#define ICR1H  *((volatile uint8_t*)0x47)
#define ICR1L  *((volatile uint8_t*)0x46)

#define TCNT1  *((volatile uint16_t*)0x4C)
#define OCR1A  *((volatile uint16_t*)0x4A)
#define ICR1   *((volatile uint16_t*)0x46)

#define UDR     (*(volatile uint8_t*)0x2C)
#define UCSRA   (*(volatile uint8_t*)0x2B)
#define UCSRB   (*(volatile uint8_t*)0x2A)
#define UCSRC   (*(volatile uint8_t*)0x40)
#define UBRRH   (*(volatile uint8_t*)0x40)
#define UBRRL   (*(volatile uint8_t*)0x29)

#define RXC     7
#define TXC     6
#define UDRE    5

#define RXCIE   7
#define TXCIE   6
#define UDRIE   5
#define RXEN    4
#define TXEN    3
#define UCSZ2   2

#define URSEL   7
#define UCSZ1   2
#define UCSZ0   1
#define USBS    3
#define UPM1    5
#define UPM0    4
#define UMSEL   6

#endif /* REGISTER_H_ */