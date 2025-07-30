#ifndef LCD_H_
#define LCD_H_

#define F_CPU 8000000UL

#include <util/delay.h>
#include <stdlib.h>
#include <string.h>

#include "../DIO/DIO.h"

void lcd_init ();
void lcd_cmd (unsigned char cmd);
void lcd_data (unsigned char data);
void lcd_print(char *p);
void lcd_print_float_line1(float number);
void lcd_print_time_line2_centered(uint8_t hours, uint8_t minutes, uint8_t seconds);
void lcd_print_count_line1_last9(uint16_t number);

#endif /* LCD_H_ */