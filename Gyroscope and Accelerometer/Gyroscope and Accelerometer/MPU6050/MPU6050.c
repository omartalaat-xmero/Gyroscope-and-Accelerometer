#include "MPU6050 Register.h"
#include "MPU6050.h"
#include "../I2C/I2C.h"
#include "../REGISTER/REGISTER.h"

#include <math.h>

//static uint8_t MPU6050_Read_Register(uint8_t reg);
static void MPU6050_Read_Multi(uint8_t reg, uint8_t* data, uint8_t length);

void MPU6050_Init(void) {
	I2C_start();
	I2C_write((MPU6050_ADDR << 1) | 0);
	I2C_write(MPU6050_PWR_MGMT_1);
	I2C_write(0x00);
	I2C_stop();
}

void MPU6050_Read_Accel(int16_t* ax, int16_t* ay, int16_t* az) {
	uint8_t data[6];
	MPU6050_Read_Multi(MPU6050_ACCEL_XOUT_H, data, 6);

	*ax = (int16_t)(data[0] << 8 | data[1]);
	*ay = (int16_t)(data[2] << 8 | data[3]);
	*az = (int16_t)(data[4] << 8 | data[5]);
}

void MPU6050_Read_Gyro(int16_t* gx, int16_t* gy, int16_t* gz) {
	uint8_t data[6];
	MPU6050_Read_Multi(MPU6050_GYRO_XOUT_H, data, 6);

	*gx = (int16_t)(data[0] << 8 | data[1]);
	*gy = (int16_t)(data[2] << 8 | data[3]);
	*gz = (int16_t)(data[4] << 8 | data[5]);
}

void MPU6050_Calculate_Angles(float* pitch, float* roll) {
	int16_t ax_raw, ay_raw, az_raw;
	MPU6050_Read_Accel(&ax_raw, &ay_raw, &az_raw);

	float ax = ax_raw / 16384.0;
	float ay = ay_raw / 16384.0;
	float az = az_raw / 16384.0;

	*pitch = atan2(ax, sqrt(ay * ay + az * az)) * (180.0 / M_PI);
	*roll  = atan2(ay, sqrt(ax * ax + az * az)) * (180.0 / M_PI);
}

static void MPU6050_Read_Multi(uint8_t reg, uint8_t* data, uint8_t length) {
	I2C_start();
	I2C_write((MPU6050_ADDR << 1) | 0); // Write
	I2C_write(reg);
	I2C_stop();
	
	I2C_start();
	I2C_write((MPU6050_ADDR << 1) | 1); // Read

	for (uint8_t i = 0; i < length - 1; i++) {
		data[i] = I2C_read_ack();
	}
	data[length - 1] = I2C_read_nack();
	I2C_stop();
}