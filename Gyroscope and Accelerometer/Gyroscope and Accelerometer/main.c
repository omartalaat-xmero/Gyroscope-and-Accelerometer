#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/interrupt.h>

#include "REGISTER/REGISTER.h"
#include "Stopwatch/Stopwatch.h"
#include "DIO/DIO.h"
#include "LCD16x2/LCD.h"
#include "FT232RL/FT232RL.h"
#include "MPU6050/MPU6050.h"

#define sw_mode_on 1
#define sw_mode_off 0
#define sw_mode_pause 2

volatile uint16_t t_microseconds = 0;
volatile uint16_t t_milliseconds = 0;
volatile uint8_t t_seconds = 0;
volatile uint8_t t_minutes = 0;
volatile uint8_t t_hours = 0;

int main(void){
	
	uint8_t button_prev_status = 0, button_curr_status = 0;
	uint8_t sw_curr_mode = 0, sw_lasts_mode = 0;
	uint8_t hours = 0, minutes = 0, seconds = 0;
	uint16_t milliseconds = 0, microseconds = 0 ;
	uint8_t prev_hours = 0, prev_minutes = 0, prev_seconds = 0;
	uint16_t prev_milliseconds = 0, prev_microseconds = 0;
	uint8_t last_press_hours = 0, last_press_minutes = 0, last_press_seconds = 0;
	uint16_t last_press_milliseconds = 0, last_press_microseconds = 0;
	uint8_t prev_press_h = 0, prev_press_m = 0, prev_press_s = 0;
	uint16_t prev_press_ms = 0, prev_press_us = 0;
	uint8_t curr_press_h = 0, curr_press_m = 0, curr_press_s = 0;
	uint16_t curr_press_ms = 0, curr_press_us = 0;
	uint8_t first_press = 1;
	uint16_t count=0;
	int16_t ax = 0, ay = 0, az = 0;
	int16_t gx = 0, gy = 0, gz = 0;
	float pitch = 0, roll = 0;
	float SPM = 0;

	Stopwatch_Init();
	lcd_init();
	FT232_init();
	MPU6050_Init();
	
    while (1){
		
		DIO_read(2,PORT_B,&button_curr_status);
		
		/******************** Stopwatch Start, Calculate SPM and count *************************/
		
		if (button_curr_status==1 && button_prev_status==0){
			MPU6050_Read_Accel(&ax, &ay, &az);
			MPU6050_Read_Gyro(&gx, &gy, &gz);
			MPU6050_Calculate_Angles(&pitch, &roll);
			sw_curr_mode = sw_mode_on;

			static uint8_t half_stroke = 0;   // 0 = catch, 1 = recovery

			if (half_stroke == 0) {   // count only on first press
				count++;
				if (count == 1) {
					Stopwatch_Reset();
					Stopwatch_Start();
				}

				// Record stroke timestamp
				Stopwatch_GetTime(&curr_press_h, &curr_press_m, &curr_press_s, &curr_press_ms, &curr_press_us);

				if (!first_press) {
					uint32_t t1_ms = (uint32_t)prev_press_h * 3600000UL + (uint32_t)prev_press_m * 60000UL + (uint32_t)prev_press_s * 1000UL + prev_press_ms;
					uint32_t t2_ms = (uint32_t)curr_press_h * 3600000UL + (uint32_t)curr_press_m * 60000UL + (uint32_t)curr_press_s * 1000UL + curr_press_ms;

					uint32_t delta_ms = (t2_ms >= t1_ms) ? (t2_ms - t1_ms) : (86400000UL + t2_ms - t1_ms);

					if (delta_ms > 0) {
						SPM = 60000.0 / delta_ms;  // strokes/min
					}
				}

				// Save for next stroke comparison
				prev_press_h = curr_press_h;
				prev_press_m = curr_press_m;
				prev_press_s = curr_press_s;
				prev_press_ms = curr_press_ms;
				first_press = 0;

				// Save last stroke time for 15s inactivity check
				last_press_hours = curr_press_h;
				last_press_minutes = curr_press_m;
				last_press_seconds = curr_press_s;
				last_press_milliseconds = curr_press_ms;
			}

			half_stroke ^= 1;  // toggle each press
		}
		
		/*********************** Wait 15 sec to close SPM and Stopwatch ********************/\
		
		if (sw_curr_mode == sw_mode_on ) {
			if (sw_lasts_mode == sw_mode_pause){
				Stopwatch_Reset();
			}
			uint8_t curr_hours_t, curr_minutes_t, curr_seconds_t;
			uint16_t curr_milliseconds_t, curr_microsecond;
			Stopwatch_GetTime(&curr_hours_t, &curr_minutes_t, &curr_seconds_t, &curr_milliseconds_t, &curr_microsecond);

			uint32_t t_now = (uint32_t)curr_hours_t * 3600000UL + (uint32_t)curr_minutes_t * 60000UL + (uint32_t)curr_seconds_t * 1000UL + curr_milliseconds_t;
			uint32_t t_last = (uint32_t)last_press_hours * 3600000UL + (uint32_t)last_press_minutes * 60000UL + (uint32_t)last_press_seconds * 1000UL + last_press_milliseconds;

			uint32_t elapsed_ms = (t_now >= t_last) ? (t_now - t_last) : (86400000UL + t_now - t_last); // Handle day wrap-around

			if (elapsed_ms >= 15000) {
				sw_curr_mode = sw_mode_pause;
				Stopwatch_Reset();
				count=0;
				curr_press_h = 0, curr_press_m = 0, curr_press_s = 0, curr_press_ms = 0;
				prev_press_h = 0, prev_press_m = 0, prev_press_s = 0, prev_press_ms = 0;
				last_press_hours = 0, last_press_minutes = 0, last_press_seconds = 0, last_press_milliseconds = 0;
			}
		}
		
		lcd_print_float_line1(SPM);  // Print SPM
    }
	/********************** Stopwatch Mode *****************************/
	
	switch(sw_curr_mode){
		case sw_mode_on:
		Stopwatch_GetTime(&hours, &minutes, &seconds, &milliseconds, &microseconds);
		lcd_print_time_line2_centered(hours,minutes,seconds);
		FT232_sendData(hours, minutes, seconds, milliseconds, microseconds, SPM, ax, ay, az, gx, gy, gz, count);
		break;
		case sw_mode_pause:
		Stopwatch_GetTime(&hours,&minutes,&seconds,&milliseconds, &microseconds);
		lcd_print_time_line2_centered(hours,minutes,seconds);
		break;
		case sw_mode_off:
		lcd_print_time_line2_centered(0,0,0);
		break;
	}
	lcd_print_count_line1_last9(count);
	sw_lasts_mode=sw_curr_mode;
	button_prev_status = button_curr_status;
}

