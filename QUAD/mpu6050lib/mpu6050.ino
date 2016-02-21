#include <Arduino.h>
#include <Wire.h>
#include "mpu6050.h"

//#define MPU6050_raw_data
#define MPU6050_scaled_data


void setup()
{
	Serial.begin(9600);
	Wire.begin();
	mpu6050.begin();
}

void loop()
{
	mpu6050.readData();

	#ifdef MPU6050_raw_data
		MPU6050_read_raw_data();
	#endif

	#ifdef MPU6050_scaled_data
		MPU6050_read_scaled_data();
	#endif
}
