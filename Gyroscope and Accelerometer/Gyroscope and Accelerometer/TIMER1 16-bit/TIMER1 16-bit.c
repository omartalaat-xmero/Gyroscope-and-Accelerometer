#include "TIMER1 16-bit/TIMER1 16-bit.h"

#include <avr/interrupt.h>

static void (*overflowCallback)(void) = 0;
static void (*compareMatchACallback)(void) = 0;
static void (*compareMatchBCallback)(void) = 0;

static volatile uint16_t t_milliseconds = 0;
static volatile uint8_t t_seconds = 0;
static volatile uint8_t t_minutes = 0;
static volatile uint8_t t_hours = 0;

void Timer1_Init(Timer1Mode mode, Timer1Prescaler prescaler){
	TCCR1A = 0;
	TCCR1B = 0;

	switch (mode) {
		case TIMER1_NORMAL:
		break;
		case TIMER1_CTC_OCR1A:
		TCCR1B |= (1 << WGM12);
		break;
		case TIMER1_FAST_PWM_ICR1:
		TCCR1A |= (1 << WGM11);
		TCCR1B |= (1 << WGM12) | (1 << WGM13);
		break;
		case TIMER1_FAST_PWM_OCR1A:
		TCCR1A |= (1 << WGM11) | (1 << WGM10);
		TCCR1B |= (1 << WGM12) | (1 << WGM13);
		break;
	}
	TCCR1B |= prescaler;
}

void Timer1_SetCompareValue(Timer1Channel channel, uint16_t value){
	if (channel == CHANNEL_A)
	OCR1A = value;
	else
	OCR1B = value;
}

void Timer1_SetICR1(uint16_t topValue){
	ICR1 = topValue;
}

void Timer1_EnableInterruptOverflow(void){
	TIMSK |= (1 << TOIE1);
}

void Timer1_EnableInterruptCompareMatchA(void){
	TIMSK |= (1 << OCIE1A);
}

void Timer1_EnableInterruptCompareMatchB(void){
	TIMSK |= (1 << OCIE1B);
}

void Timer1_SetCallbackOverflow(void (*callback)(void)){
	overflowCallback = callback;
}

void Timer1_SetCallbackCompareMatchA(void (*callback)(void)){
	compareMatchACallback = callback;
}

void Timer1_SetCallbackCompareMatchB(void (*callback)(void)){
	compareMatchBCallback = callback;
}

ISR(TIMER1_OVF_vect){
	if (overflowCallback)
	overflowCallback();
}

ISR(TIMER1_COMPA_vect){
	if (compareMatchACallback)
	compareMatchACallback();
}

ISR(TIMER1_COMPB_vect){
	if (compareMatchBCallback)
	compareMatchBCallback();
}

void Timer1_GetTime(uint8_t* h, uint8_t* m, uint8_t* s, uint16_t* ms){
	cli();
	*h = t_hours;
	*m = t_minutes;
	*s = t_seconds;
	*ms = t_milliseconds;
	sei();
}

void Timer1_SetTime(uint8_t h, uint8_t m, uint8_t s, uint16_t ms){
	cli();
	t_hours = h;
	t_minutes = m;
	t_seconds = s;
	t_milliseconds = ms;
	sei();
}

ISR(TIMER1_COMPA_vect){
	t_milliseconds++;
	if (t_milliseconds >= 1000) {
		t_milliseconds = 0;
		t_seconds++;
		if (t_seconds >= 60) {
			t_seconds = 0;
			t_minutes++;
			if (t_minutes >= 60) {
				t_minutes = 0;
				t_hours++;
				if (t_hours >= 24) t_hours = 0;
			}
		}
	}

	if (compareMatchACallback)
	compareMatchACallback();
}