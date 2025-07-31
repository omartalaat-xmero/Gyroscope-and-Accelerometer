#include "OpenLog.h"
#include "../UART/UART.h"
#include "../ATM32A EEPROM/ATM32 EEPROM.h"
#include <stdio.h>
#include <util/delay.h>

static void OpenLog_sendCtrlZ(void) {
	UART_sendByte(26);
	_delay_ms(10);
}

void OpenLog_init(void) {
	char filename[20];
	uint16_t sessionNum = EEPROM_readSessionNumber();
	sprintf(filename, "session%u.txt", sessionNum);
	EEPROM_writeSessionNumber(sessionNum + 1);
	_delay_ms(1000);
	OpenLog_sendCtrlZ();
	UART_sendString("append ");
	UART_sendString(filename);
	UART_sendByte('\r');
	_delay_ms(10);
	OpenLog_writeLine("Session started:");
}

void OpenLog_writeLine(const char* data) {
	UART_sendString(data);
	UART_sendByte('\n');
	_delay_ms(1);
}

void OpenLog_write(const char* data) {
	UART_sendString(data); // Just send data as-is
	_delay_ms(1);
}

void OpenLog_LogFullLine(
	uint8_t h, uint8_t m, uint8_t s, uint16_t ms, uint16_t micros,
	uint16_t spm,
	int16_t ax, int16_t ay, int16_t az,
	int16_t gx, int16_t gy, int16_t gz,
	uint16_t count,
	float p, float r
	){
		char buffer[128];
		sprintf(buffer,
		"time:%02u:%02u:%02u:%03u:%03u "
		"SPM:%u "
		"Ax:%d Ay:%d Az:%d "
		"gx:%d gy:%d gz:%d "
		"count:%u "
		"P:%.2f R:%.2f",
		h, m, s, ms, micros,
		spm,
		ax, ay, az,
		gx, gy, gz,
		count,
		p, r
		);
		OpenLog_writeLine(buffer);
}