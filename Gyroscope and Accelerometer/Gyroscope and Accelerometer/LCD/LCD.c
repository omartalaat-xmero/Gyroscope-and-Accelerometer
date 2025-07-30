#include "../LCD/LCD.h"

void lcd_init(){
	DDRA = 0xFF;   // Configure PORTA as output for data and RS
	DDRB |= (1 << 1);
	DDRB |= (1 << 0);
	lcd_cmd(0x38); // 8-bit, 2 lines, 5x7 dots
	lcd_cmd(0x0F); // Display ON, Cursor ON, Blink ON
	lcd_cmd(0x01); // Clear display
	_delay_ms(2);
	lcd_cmd(0x83);
	lcd_print("SPM");
	lcd_cmd(0x8B);
	lcd_print("Count");
	lcd_cmd(0xCC);
	lcd_print("Time");
}

void lcd_cmd (unsigned char cmd){
	PORTB &= ~(1 << 1);  // RS = 0 for command
	PORTA = cmd; // Send command to A7-
	PORTB |= (1 << 0);   // E = 1
	_delay_us(1);
	PORTB &= ~(1 << 0);  // E = 0
	_delay_ms(2);
}

void lcd_data (unsigned char data){
	PORTB |= (1 << 1);   // RS = 1 for data
	PORTA = data; // Send data to A7-A0
	_delay_us(1);
	PORTB |= (1 << 0);   // E =
	PORTB &= ~(1 << 0);  // E = 0
	_delay_ms(2);
}
void lcd_data_t (unsigned char data){
	PORTB |= (1 << 1);   // RS = 1 for data
	PORTA = data; // Send data to A7-A0
	_delay_us(1);
	PORTB |= (1 << 0);   // E = 1
	PORTB &= ~(1 << 0);  // E = 0
	_delay_us(40);
}

void lcd_print(char *p){
	while(*p){
		lcd_data(*p++);
	}
}

void lcd_print_float_line1(float number) {
	char buffer[10];
	int int_part = (int)number;
	lcd_cmd(0x0C);
	lcd_cmd(0x80);
	if (number < 0) {
		lcd_data('-');
		number = -number;
	}
	itoa(int_part, buffer, 10);
	lcd_print(buffer);
}

void lcd_print_time_line2_centered(uint8_t hours, uint8_t minutes, uint8_t seconds) {
	lcd_cmd(0xC3);
	lcd_data_t((hours / 10) + '0');
	lcd_data_t((hours % 10) + '0');
	lcd_data_t(':');
	lcd_data_t((minutes / 10) + '0');
	lcd_data_t((minutes % 10) + '0');
	lcd_data_t(':');
	lcd_data_t((seconds / 10) + '0');
	lcd_data_t((seconds % 10) + '0');
}

void lcd_print_count_line1_last9(uint16_t number) {
	char buffer[5];   // Enough for 4 digits + null terminator
	char padded[5] = "    ";  // Start with 4 spaces

	itoa(number, buffer, 10);  // Convert number to string (base 10)

	// Copy the number string to the end of padded buffer
	size_t len = strlen(buffer);
	memcpy(&padded[4 - len], buffer, len);

	lcd_cmd(0x87);  // Position 7 in line 1
	for (uint8_t i = 0; i < 4; i++) {
		lcd_data(padded[i]);
	}
}