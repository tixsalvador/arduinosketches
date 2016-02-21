#ifndef mpu6050_h
#define mpu6050_h
#include "Arduino.h"
	
extern int accel_x,accel_y,accel_z,gyro_x,gyro_y,gyro_z;
extern float accel_x_scaled,accel_y_scaled,accel_z_scaled,gyro_x_scaled,gyro_y_scaled,gyro_z_scaled,temp_scaled;

void MPU6050_read_scaled_data();
void MPU6050_read_raw_data();

class mpu6050Class {
	public:
		void begin();
		void readData();
	};
extern mpu6050Class mpu6050;
#endif

