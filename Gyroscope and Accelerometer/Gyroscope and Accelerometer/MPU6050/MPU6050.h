#ifndef MPU6050_H_
#define MPU6050_H_

#include <stdint.h>

void MPU6050_Init(void);

// Accelerometer
void MPU6050_Read_Accel(int16_t* ax, int16_t* ay, int16_t* az);

// Gyroscope
void MPU6050_Read_Gyro(int16_t* gx, int16_t* gy, int16_t* gz);

/*
	Angel:
		1)pitch: rotation around X-axis
		2)roll: rotation around Y-axis
*/
void MPU6050_Calculate_Angles(float* pitch, float* roll); 


#endif /* MPU6050_H_ */