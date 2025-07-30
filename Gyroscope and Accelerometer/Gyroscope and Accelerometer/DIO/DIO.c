#include "../DIO/DIO.h"

void DIO_init(unit8_t pinnumber, unit8_t portnumber, unit8_t direction){
	switch(portnumber){
		case PORT_A:
		if (direction == IN){
			DDRA &= ~(1<<pinnumber);
		}
		else if(direction == OUT){
			DDRA |=(1<<pinnumber);
		}
		else{
			
		}
		break;
		case PORT_B:
		if (direction == IN){
			DDRB &= ~(1<<pinnumber);
		}
		else if(direction == OUT){
			DDRB |=(1<<pinnumber);
		}
		else{
			
		}
		break;
		case PORT_C:
		if (direction == IN){
			DDRC &= ~(1<<pinnumber);
		}
		else if(direction == OUT){
			DDRC |=(1<<pinnumber);
		}
		else{
			
		}
		break;
		case PORT_D:
		if (direction == IN){
			DDRD &= ~(1<<pinnumber);
		}
		else if(direction == OUT){
			DDRD |=(1<<pinnumber);
		}
		else{
			
		}
		break;
	}
}

void DIO_write(unit8_t pinnumber, unit8_t portnumber, unit8_t value){
	switch(portnumber){
		case PORT_A:
		if (value == LOW){
			PORTA &= ~(1<<pinnumber);
		}
		else if(value == HIGH){
			PORTA |=(1<<pinnumber);
		}
		else{
			
		}
		break;
		case PORT_B:
		if (value == LOW){
			PORTB &= ~(1<<pinnumber);
		}
		else if(value == HIGH){
			PORTB |=(1<<pinnumber);
		}
		else{
			
		}
		break;
		case PORT_C:
		if (value == LOW){
			PORTC &= ~(1<<pinnumber);
		}
		else if(value == HIGH){
			PORTC |=(1<<pinnumber);
		}
		else{
			
		}
		break;
		case PORT_D:
		if (value == LOW){
			PORTD &= ~(1<<pinnumber);
		}
		else if(value == HIGH){
			PORTD |=(1<<pinnumber);
		}
		else{
			
		}
		break;
	}
}

void DIO_toggel(unit8_t pinnumber, unit8_t portnumber){
	switch(portnumber){
		case PORT_A:
		PORTA ^= (1<<pinnumber);
		break;
		case PORT_B:
		PORTB ^= (1<<pinnumber);
		break;
		case PORT_C:
		PORTC ^= (1<<pinnumber);
		break;
		case PORT_D:
		PORTD ^= (1<<pinnumber);
		break;
	}
	
}

void DIO_read(unit8_t pinnumber, unit8_t portnumber,  unit8_t *value){
	switch(portnumber) {
		case PORT_A:
		*value = (PINA & (1 << pinnumber)) ? HIGH : LOW;
		break;
		case PORT_B:
		*value = (PINB & (1 << pinnumber)) ? HIGH : LOW;
		break;
		case PORT_C:
		*value = (PINC & (1 << pinnumber)) ? HIGH : LOW;
		break;
		case PORT_D:
		*value = (PIND & (1 << pinnumber)) ? HIGH : LOW;
		break;
	}
}