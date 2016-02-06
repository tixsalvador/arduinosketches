#include <Arduino.h>
#include <Wire.h>
#include "mpu6050.h"

#define MPU6050_address 0x68

int accel_x=0,accel_y=0,accel_z=0,temp=0,gyro_x=0,gyro_y=0,gyro_z=0;
int accel_x_OC=0,accel_y_OC=0,accel_z_OC=0,gyro_x_OC=0,gyro_y_OC=0,gyro_z_OC=0;
float accel_x_scaled,accel_y_scaled,accel_z_scaled,gyro_x_scaled,gyro_y_scaled,gyro_z_scaled,temp_scaled;
float accel_scaled_fact=1, gyro_scaled_fact=1;

void MPU6050_read_data()
{
        Wire.beginTransmission(MPU6050_address);
        Wire.write(0x3B);
        Wire.endTransmission();

        int MPU6050_data_size=14;

        Wire.requestFrom(MPU6050_address,MPU6050_data_size);
        if(Wire.available()== MPU6050_data_size){
                accel_x = Wire.read()<<8 | Wire.read();
                accel_y = Wire.read()<<8 | Wire.read();
                accel_z = Wire.read()<<8 | Wire.read();
                temp = Wire.read()<<8 | Wire.read();
                gyro_x = Wire.read()<<8 | Wire.read();
                gyro_y = Wire.read()<<8 | Wire.read();
                gyro_z = Wire.read()<<8 | Wire.read();
        }
	accel_x_scaled=(float)(accel_x-accel_x_OC)*accel_scaled_fact/1000;
	accel_y_scaled=(float)(accel_y-accel_y_OC)*accel_scaled_fact/1000;
	accel_z_scaled=(float)(accel_z-accel_z_OC)*accel_scaled_fact/1000;
	gyro_x_scaled=(float)(gyro_x-gyro_x_OC)*gyro_scaled_fact/1000;
	gyro_y_scaled=(float)(gyro_y-gyro_y_OC)*gyro_scaled_fact/1000;
	gyro_z_scaled=(float)(gyro_z-gyro_z_OC)*gyro_scaled_fact/1000;
}

void MPU6050_read_raw_data()
{
        Serial.print(accel_x);
        Serial.print("  ");
        Serial.print(accel_y);
        Serial.print("  ");
        Serial.print(accel_z);
        Serial.print("  ");
        Serial.print(temp);
        Serial.print("  ");
        Serial.print(gyro_x);
        Serial.print("  ");
        Serial.print(gyro_y);
        Serial.print("  ");
        Serial.println(gyro_z);
}

void MPU6050_read_scaled_data()
{
	Serial.print(accel_x_scaled);
	Serial.print("	");
	Serial.print(accel_y_scaled);
	Serial.print("	");
	Serial.print(accel_z_scaled);
	Serial.print("		");
	Serial.print(gyro_x_scaled);
	Serial.print("	");
	Serial.print(gyro_y_scaled);
	Serial.print("	");
	Serial.println(gyro_z_scaled);
}
