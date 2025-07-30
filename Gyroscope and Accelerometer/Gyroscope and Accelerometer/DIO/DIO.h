#ifndef DIO_H_
#define DIO_H_

#include "../REGISTER/REGISTER.h"

#define PORT_A 'A'
#define PORT_B 'B'
#define PORT_C 'C'
#define PORT_D 'D'
#define IN 0
#define OUT 1
#define LOW 0
#define HIGH 1

void DIO_init(unit8_t pinnumber, unit8_t portnumber,  unit8_t direction);
void DIO_write(unit8_t pinnumber, unit8_t portnumber,  unit8_t value);
void DIO_toggel(unit8_t pinnumber, unit8_t portnumber);
void DIO_read(unit8_t pinnumber, unit8_t portnumber,  unit8_t *value);

#endif /* DIO_H_ */