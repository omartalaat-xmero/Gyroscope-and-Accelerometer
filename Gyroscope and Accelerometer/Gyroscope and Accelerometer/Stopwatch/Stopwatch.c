#include "Stopwatch.h"
#include "../REGISTER/REGISTER.h"
#include <avr/interrupt.h>

static volatile uint8_t stopwatch_hours = 0;
static volatile uint8_t stopwatch_minutes = 0;
static volatile uint8_t stopwatch_seconds = 0;
static volatile uint16_t stopwatch_milliseconds = 0;
static volatile uint16_t stopwatch_microseconds = 0;

void Stopwatch_Init(void) {
	TCCR1A = 0;
	TCCR1B = 0;
	TCCR1B |= (1 << WGM12);
	OCR1A = 7;
	TIMSK |= (1 << OCIE1A);
}

void Stopwatch_Start(void) {
	TCCR1B |= (1 << CS10);
	sei();
}

void Stopwatch_Stop(void) {
	TCCR1B &= ~(1 << CS10);
}

void Stopwatch_Reset(void) {
	cli();
	stopwatch_hours = 0;
	stopwatch_minutes = 0;
	stopwatch_seconds = 0;
	stopwatch_milliseconds = 0;
	stopwatch_microseconds = 0;
	sei();
}

void Stopwatch_GetTime(uint8_t* h, uint8_t* m, uint8_t* s, uint16_t* ms, uint16_t* us) {
	cli();
	*h = stopwatch_hours;
	*m = stopwatch_minutes;
	*s = stopwatch_seconds;
	*ms = stopwatch_milliseconds;
	*us = stopwatch_microseconds;
	sei();
}

ISR(TIMER1_COMPA_vect) {
	stopwatch_microseconds++;

	if (stopwatch_microseconds >= 1000) {
		stopwatch_microseconds = 0;
		stopwatch_milliseconds++;

		if (stopwatch_milliseconds >= 1000) {
			stopwatch_milliseconds = 0;
			stopwatch_seconds++;

			if (stopwatch_seconds >= 60) {
				stopwatch_seconds = 0;
				stopwatch_minutes++;

				if (stopwatch_minutes >= 60) {
					stopwatch_minutes = 0;
					stopwatch_hours++;

					if (stopwatch_hours >= 24)
					stopwatch_hours = 0;
				}
			}
		}
	}
}