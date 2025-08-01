#include <stdio.h>
#include "FT232RL.h"
#include "../UART/UART.h"

static char tx_buffer[128];

void FT232_init(void) {
	UART_init();
}

void FT232_sendData(
uint8_t h, uint8_t m, uint8_t s, uint16_t ms, uint16_t micros,
uint16_t spm,
int16_t ax, int16_t ay, int16_t az,
int16_t gx, int16_t gy, int16_t gz,
uint16_t count
) {
	snprintf(tx_buffer, sizeof(tx_buffer),
	"time=%02u:%02u:%02u.%03u.%03u, "
	"spm=%u, "
	"ax=%d, ay=%d, az=%d, "
	"gx=%d, gy=%d, gz=%d, "
	"count=%u\r\n",
	h, m, s, ms, micros,
	spm,
	ax, ay, az,
	gx, gy, gz,
	count
	);

	UART_sendString(tx_buffer);
}