#include "../I2C/I2C.h"

void I2C_init(void){ //This function prepares your microcontroller to act as an I2C Master
	TWSR = 0x00; //Set prescaler to 1 for I2C clock
	TWBR = ((F_CPU / SCL_CLOCK) - 16) / 2; //Set I2C bit rate
	TWCR = (1 << TWEN); //Enable the I2C peripheral
}

void I2C_start(void){ //Its purpose is to send a START condition on the I2C bus, which signals the beginning of communication with an I2C slave
	/*
		Sets the TWI Control Register (TWCR) bits:
			TWSTA (TWI START Condition Bit):
			Tells the I2C hardware to send a START condition on the bus.
			TWEN (TWI Enable Bit):
			Enables the I2C hardware
			TWINT (TWI Interrupt Flag
			Writing a 1 clears the flag and starts the operation (START condition in this case
		This combination initiates a START condition on the I2C bus.
	*/
	TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);
	while (!(TWCR & (1 << TWINT))); //Waits until the START condition has been sent
}

void I2C_write(uint8_t data){ //It is used to send a byte (either a slave address or data) via I2C
	TWDR = data;
	/*
		Starts the transmission:
			TWEN: Enables the I2C hardware.
			TWINT: Clears the interrupt flag to begin the transmission.
		Once this line is executed, the hardware begins sending the byte in TWDR over the I2C bus.
	*/
	TWCR = (1 << TWEN) | (1 << TWINT);
	while (!(TWCR & (1 << TWINT))); //Waits for the transmission to complete.
	//The TWINT flag is set by the hardware when the byte has been fully transmitted and the slave has acknowledged (or not).
}

uint8_t I2C_read_ack(void){ //It is used when you want to read a byte from the slave and tell it that you want more data by sending an ACK.
	TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWEA);
	while (!(TWCR & (1 << TWINT)));
	return TWDR;
}

uint8_t I2C_read_nack(void){ //It's used to read the final byte in a multi-byte read sequence.
	TWCR = (1 << TWEN) | (1 << TWINT);
	while (!(TWCR & (1 << TWINT)));
	return TWDR;
	/*
		Notice: TWEA is not set, which means:
			After receiving this byte, no ACK is sent.
			This is interpreted by the slave as: "Stop sending, this was the last byte."
	*/
}

void I2C_stop(void){ //It is called at the end of an I2C communication session.
	TWCR = (1 << TWSTO) | (1 << TWEN) | (1 << TWINT);
	/*
		You do not need to wait for TWINT after sending a STOP condition.
		The STOP condition is issued immediately, and the hardware automatically clears TWSTO once it’s done.
	*/
}