#ifndef mpu6050_h
#define mpu6050_h
#include "Arduino.h"
	
extern int accel_x,accel_y,accel_z,gyro_x,gyro_y,gyro_z;
extern float accel_x_scaled,accel_y_scaled,accel_z_scaled,gyro_x_scaled,gyro_y_scaled,gyro_z_scaled,temp_scaled;
extern float angle_x_gyro,angle_y_gyro,angle_z_gyro,angle_x_accel,angle_y_accel,angle_z_accel,angle_x,angle_y,angle_z;

void MPU6050_read_scaled_data();
void MPU6050_read_raw_data();
void MPU6050_read_angle();

class mpu6050Class {
	public:
		void begin();
		void readData();
		void dlpfConfig(int DLPF_CFG);
		void setGains(int gyro,int accel);
		void offsetCalibration();
		void sensorAngle();
	};
extern mpu6050Class mpu6050;
#endif

