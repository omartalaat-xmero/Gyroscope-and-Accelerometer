#include "../TIMER1 16-bit/TIMER1 16-bit.h"
#include <avr/interrupt.h>

static void (*overflowCallback)(void) = 0;
static void (*compareMatchACallback)(void) = 0;
static void (*compareMatchBCallback)(void) = 0;

static volatile uint16_t t_milliseconds = 0;
static volatile uint8_t t_seconds = 0;
static volatile uint8_t t_minutes = 0;
static volatile uint8_t t_hours = 0;

/*
	Initializes Timer1 with the specified mode and prescaler
	Mode: Normal, CTC (using OCR1A), and Fast PWM using (ICR1) or (OCR1A)
		I cannot use four modes as the same time
	Prescaler: is a divider for the system clock. It reduces the clock frequency that feeds the timer to control how fast the timer counts.
		1    =  8,000,000 Hz  =  0.125 µs
		8    =  1,000,000 Hz  =  1 µs
		64   =  125,000 Hz    =  8 µs
		256  =  31,250 Hz     =  32 µs
		1024 =  7,812.5 Hz    =  128 µs
*/

void Timer1_Init(Timer1Mode mode, Timer1Prescaler prescaler){
	TCCR1A = 0;
	TCCR1B = 0;

	switch (mode) {
		/*	1) Normal mode
			Start from 0 to 65535(this is overflow interrupt)
			Can use overflow interrupt (TIMER1_OVF_vect) to detect when the counter wraps 
			(write code inside this interrupt to "do something" every time the overflow occurs.)
		*/
		case TIMER1_NORMAL: 
		break;
		/*
			2)TIMER1_CTC_OCR1A
			More precise and predictable timing than overflow mode.
			You choose exactly how many ticks make a cycle.
			Useful for:
				Software timers
				Blinking LEDs
				Generating exact periodic interrupts
		*/
		case TIMER1_CTC_OCR1A:
		TCCR1B |= (1 << WGM12);
		break;
		/*
			3)TIMER1_FAST_PWM_ICR1
			Fast PWM mode with TOP = ICR1
			Timer counts from 0 to ICR1, then resets to 0.
			OCR1A or OCR1B sets the duty cycle.
			ICR1 allows flexible resolution and frequency control.
			Use case:
				Generating high-resolution PWM signals (servo motors, audio, analog control)
				Frequency control without changing prescaler
				More accurate than using OCR1A as TOP
		*/
		case TIMER1_FAST_PWM_ICR1:
		TCCR1A |= (1 << WGM11);
		TCCR1B |= (1 << WGM12) | (1 << WGM13);
		break;
		/*
			4)TIMER1_FAST_PWM_OCR1A
			 How it works:
				Fast PWM mode with TOP = OCR1A
				Timer counts from 0 to OCR1A, resets
				PWM output uses OCR1B only (you can't use OCR1A for both duty & top at once)
			 Limitations:
				You can't use OCR1A for duty cycle in this mode only OCR1B.
				Less flexible than ICR1 method.
			 Use case:
				PWM output where frequency isn't changed often
				When only one channel (OCR1B) is needed for output
		*/
		case TIMER1_FAST_PWM_OCR1A:
		TCCR1A |= (1 << WGM11) | (1 << WGM10);
		TCCR1B |= (1 << WGM12) | (1 << WGM13);
		break;
	}
	TCCR1B |= prescaler;
}

/*
	Timer1_SetCompareValue:
	If you're using a Compare Match interrupt, this function sets the point at which that interrupt happens.
	Imagine Timer1 is in CTC mode (Clear Timer on Compare Match) and counting from 0 upwards.
	
		Timer1_SetCompareValue(CHANNEL_A, 124);
		
		When the timer reaches 124, it will:
		Trigger the Compare Match A interrupt (if enabled)
		Automatically reset back to 0 (if in CTC mode)
*/

void Timer1_SetCompareValue(Timer1Channel channel, uint16_t value){
	if (channel == CHANNEL_A)
	OCR1A = value;
	else
	OCR1B = value;
}

/*
	Timer1_SetICR1:
	This function sets the TOP value for Timer1 when it's running in certain Fast PWM or CTC modes.
	What does ICR1 mean?
		ICR1 = Input Capture Register 1
		In some modes, it is used to define the maximum count value the timer will reach before:
		Resetting to 0, or
		Repeating the waveform
		This is called the TOP value.
		
	Timer1_Init(TIMER1_FAST_PWM_ICR1, PRESCALER_8);
	Timer1_SetICR1(19999);  // Set top value
	
	This sets the TOP value of the counter to 19999. That means:
	Timer1 will count 0 ? 19999, then restart
	In PWM mode, this defines the frequency of the output
	Think of a timer like a loop from 0 to N.
	Setting ICR1 = 1000 is like saying:
	“Count from 0 to 1000, then repeat.”
	This is crucial for generating accurate timing or PWM signals.
*/

void Timer1_SetICR1(uint16_t topValue){
	ICR1 = topValue;
}

/*
	Timer1_EnableInterruptOverflow:
		(Enables the Timer1 overflow interrupt so that the ISR(TIMER1_OVF_vect) is called)
		(every time Timer1 overflows (reaches its maximum count and resets to 0).)
	This function enables the Timer1 overflow interrupt.
	TIMSK is the Timer Interrupt Mask Register.
	TOIE1 stands for Timer/Counter1 Overflow Interrupt Enable.
	Setting the TOIE1 bit allows interrupts to trigger when Timer1 overflows (i.e., when it counts past its maximum value and wraps back to 0).
	This function tells the microcontroller:
	“Interrupt me every time Timer1 reaches its max value and overflows
	Then the ISR (Interrupt Service Routine) ISR(TIMER1_OVF_vect) will be automatically called.
*/

void Timer1_EnableInterruptOverflow(void){
	TIMSK |= (1 << TOIE1);
}

/*
	Timer1_EnableInterruptCompareMatchI(A/B):
	Enables the Timer1 Compare Match A or B interrupt.
	This causes the ISR(TIMER1_COMP(A/B)_vect) to be called when TCNT1 == OCR1(A/B).
*/

void Timer1_EnableInterruptCompareMatchA(void){
	TIMSK |= (1 << OCIE1A);
}

void Timer1_EnableInterruptCompareMatchB(void){
	TIMSK |= (1 << OCIE1B);
}

/*
	Timer1_SetCallbackOverflow: This function sets the callback function to be executed when the Timer1 overflow interrupt occurs.
		(
			Sets the function to be called when Timer1 overflows (TCNT1 overflows from 0xFFFF to 0).
			The provided function will be called inside the TIMER1_OVF_vect ISR.
		)
	overflowCallback is a function pointer of type void (*)(void).
	When the timer counter (TCNT1) overflows (i.e., wraps around from 0xFFFF to 0), the TIMER1_OVF_vect ISR is triggered.
	Inside the ISR, the overflowCallback() function (if set) is called
	This allows you to customize what code runs on timer overflow without hardcoding it into the ISR.
*/

void Timer1_SetCallbackOverflow(void (*callback)(void)){
	overflowCallback = callback;
}

/*
	Timer1_SetCallbackCompareMatch(A/B):
	These functions are part of a callback mechanism. They let you register your own custom function to be executed when:
	TCNT1 == OCR1A (for Compare Match A)
	TCNT1 == OCR1B (for Compare Match B)
	This makes your code modular and reusable.
	
*/

void Timer1_SetCallbackCompareMatchA(void (*callback)(void)){
	compareMatchACallback = callback;
}

void Timer1_SetCallbackCompareMatchB(void (*callback)(void)){
	compareMatchBCallback = callback;
}

/*
	This ISR is triggered automatically when Timer1 overflows (TCNT1 reaches 0xFFFF and rolls back to 0).
	It checks if a callback function has been assigned using Timer1_SetCallbackOverflow(), and if so, it calls that function.
*/

ISR(TIMER1_OVF_vect){
	if (overflowCallback)
	overflowCallback(); // Call the user-defined overflow handler (if set)
}

/*
	This ISR is triggered when Timer1's counter (TCNT1) matches the value in OCR1A.
	It's mainly used in CTC (Clear Timer on Compare Match) or Fast PWM modes.
	It checks if a callback function has been assigned using Timer1_SetCallbackCompareMatchA(), and if so, it calls that function.
*/

// ISR(TIMER1_COMPA_vect){
// 	if (compareMatchACallback)
// 	compareMatchACallback(); // Call the user-defined Compare Match A handler (if set)
// }

/*
	This ISR is triggered when Timer1's counter (TCNT1) matches the value in OCR1B.
	It can be used in CTC or PWM modes where a second compare channel is useful (e.g., dual event timing).
	It checks if a callback function has been assigned using Timer1_SetCallbackCompareMatchB(), and if so, it calls that function.
*/

ISR(TIMER1_COMPB_vect){
	if (compareMatchBCallback)
	compareMatchBCallback(); // Call the user-defined Compare Match B handler (if set)
}
/*
	Each ISR handles a specific type of Timer1 event:
	Overflow: Entire counter range is completed.
	Compare Match A: Timer matches OCR1A.
	Compare Match B: Timer matches OCR1B.
	Each one is also decoupled from logic by using a user-defined callback pointer. This makes your timer driver modular and flexible.
*/