#ifndef TIMER1 16-BIT_H_
#define TIMER1 16-BIT_H_

#include "../REGISTER/REGISTER.h"
#include "TIMER1 16-bit/TIMER1 TIMER1 16-bit Typedef.h"
#include <stdint.h>

void Timer1_Init(Timer1Mode mode, Timer1Prescaler prescaler);
void Timer1_SetCompareValue(Timer1Channel channel, uint16_t value);
void Timer1_SetICR1(uint16_t topValue);
void Timer1_EnableInterruptOverflow(void);
void Timer1_EnableInterruptCompareMatchA(void);
void Timer1_EnableInterruptCompareMatchB(void);
void Timer1_SetCallbackOverflow(void (*callback)(void));
void Timer1_SetCallbackCompareMatchA(void (*callback)(void));
void Timer1_SetCallbackCompareMatchB(void (*callback)(void));
void Timer1_GetTime(uint8_t* h, uint8_t* m, uint8_t* s, uint16_t* ms);
void Timer1_SetTime(uint8_t h, uint8_t m, uint8_t s, uint16_t ms);


#endif /* TIMER1 16-BIT_H_ */