#ifndef I2C_H_
#define I2C_H_

#include "../REGISTER/REGISTER.h"

#include <util/twi.h>

#define F_CPU       8000000UL
#define SCL_CLOCK   100000UL

void I2C_init(void);
void I2C_start(void);
void I2C_write(uint8_t data);
uint8_t I2C_read_ack(void);
uint8_t I2C_read_nack(void);
void I2C_stop(void);


#endif /* I2C_H_ */