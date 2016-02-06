#include <Arduino.h>
#include <Wire.h>
#include "mpu6050.h"

//#define MPU6050_raw_data
#define MPU6050_scaled_data

void setup()
{
	Serial.begin(9600);
	Wire.begin();
	Wire.beginTransmission(0x68);
	Wire.write(0x6B);
	Wire.write(0);
	Wire.endTransmission(true);
}

void loop()
{
	MPU6050_read_data();
	
	#ifdef MPU6050_raw_data
		MPU6050_read_raw_data();
	#endif

	#ifdef MPU6050_scaled_data
		MPU6050_read_scaled_data();
	#endif
}
