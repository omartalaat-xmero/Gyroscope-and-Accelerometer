#ifndef ATM32_EEPROM_H_
#define ATM32_EEPROM_H_

#include <stdint.h>

uint16_t EEPROM_readSessionNumber(void);
void EEPROM_writeSessionNumber(uint16_t number);

#endif /* ATM32_EEPROM_H_ */