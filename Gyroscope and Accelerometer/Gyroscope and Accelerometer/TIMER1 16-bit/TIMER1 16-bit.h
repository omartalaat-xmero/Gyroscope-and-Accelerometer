#ifndef TIMER1_16_BIT_H_
#define TIMER1_16_BIT_H_

#include "../REGISTER/REGISTER.h"
#include <stdint.h>

typedef enum {
	TIMER1_NORMAL,
	TIMER1_CTC_OCR1A,
	TIMER1_FAST_PWM_ICR1,
	TIMER1_FAST_PWM_OCR1A
} Timer1Mode;

typedef enum {
	TIMER1_NO_CLOCK,
	TIMER1_NO_PRESCALER,
	TIMER1_PRESCALER_8,
	TIMER1_PRESCALER_64,
	TIMER1_PRESCALER_256,
	TIMER1_PRESCALER_1024
} Timer1Prescaler;

typedef enum {
	CHANNEL_A,
	CHANNEL_B
} Timer1Channel;

void Timer1_Init(Timer1Mode mode, Timer1Prescaler prescaler);
void Timer1_SetCompareValue(Timer1Channel channel, uint16_t value);
void Timer1_SetICR1(uint16_t topValue);
void Timer1_EnableInterruptOverflow(void);
void Timer1_EnableInterruptCompareMatchA(void);
void Timer1_EnableInterruptCompareMatchB(void);
void Timer1_SetCallbackOverflow(void (*callback)(void));
void Timer1_SetCallbackCompareMatchA(void (*callback)(void));
void Timer1_SetCallbackCompareMatchB(void (*callback)(void));

#endif /* TIMER1_16_BIT_H_ */