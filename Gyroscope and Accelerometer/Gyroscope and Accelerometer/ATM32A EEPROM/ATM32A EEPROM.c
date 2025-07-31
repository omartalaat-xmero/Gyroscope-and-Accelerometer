#include <avr/eeprom.h>

uint16_t EEPROM_readSessionNumber(void) {
	uint16_t sessionNumber = eeprom_read_word((uint16_t*)0);
	if (sessionNumber == 0xFFFF || sessionNumber == 0) {
		sessionNumber = 1;
	}
	return sessionNumber;
}

void EEPROM_writeSessionNumber(uint16_t number) {
	eeprom_write_word((uint16_t*)0, number);
}