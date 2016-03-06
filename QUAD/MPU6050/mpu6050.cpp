#include <Wire.h>
#include "Arduino.h"
#include "mpu6050.h"

#define MPU6050_address 0x68

#define MPU6050_config 0x1A
#define MPU6050_gyro_config 0x1B
#define MPU6050_accel_config 0x1C

#define gravity 9.81 //Gravitational acceleration

int accel_x=0,accel_y=0,accel_z=0,temp=0,gyro_x=0,gyro_y=0,gyro_z=0;
int accel_x_OC=0,accel_y_OC=0,accel_z_OC=0,gyro_x_OC=0,gyro_y_OC=0,gyro_z_OC=0;
float accel_x_scaled,accel_y_scaled,accel_z_scaled,gyro_x_scaled,gyro_y_scaled,gyro_z_scaled,temp_scaled;
float accel_scaled_fact=1, gyro_scaled_fact=1;

void mpu6050Class::begin()
{
	Wire.beginTransmission(MPU6050_address);
        Wire.write(0x6B);
        Wire.write(0b10000000); //Reset
        Wire.endTransmission();
        delay(200);
        Wire.beginTransmission(MPU6050_address);
        Wire.write(0x6B);
        Wire.write(0b00000000);  //Wake
        Wire.endTransmission(true);
}

void mpu6050Class::readData()
{
	Wire.beginTransmission(MPU6050_address);
        Wire.write(0x3B);
        Wire.endTransmission(false);

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
        temp_scaled=(float)temp/340+36.53;
}

void mpu6050Class::dlpfConfig(int DLPF_CFG)
{
	if(DLPF_CFG<0||DLPF_CFG>6){
		DLPF_CFG=0;
	}
	Wire.beginTransmission(MPU6050_address);
	Wire.write(MPU6050_config);
	Wire.write(DLPF_CFG);
	Wire.endTransmission();
}

void mpu6050Class::setGains(int gyro,int accel)
{
	byte gyro_byte,accel_byte;
	
	Wire.beginTransmission(MPU6050_address);
	Wire.write(MPU6050_gyro_config);
	if(gyro==0){
		gyro_byte=0b00000000;
		gyro_scaled_fact=(float)250*0.0305; //16 bit 2^15-1=32767 milli divide by 1000 //1000/32767=0.0305 
	}
	else if(gyro==1){
		gyro_byte=0b00001000;
		gyro_scaled_fact=(float)500*0.0305;
	}
	else if(gyro==2){
		gyro_byte=0b00010000;
		gyro_scaled_fact=(float)1000*0.0305;
	}
	else if(gyro==3){
		gyro_byte=0b00011000;
		gyro_scaled_fact=(float)2000*0.0305;
	}
	else {
		gyro_scaled_fact=1;
	}
	Wire.write(gyro_byte);
	Wire.endTransmission();

	Wire.beginTransmission(MPU6050_address);
	Wire.write(MPU6050_accel_config);
	if(accel==0){
		accel_byte=0b00000000;
		accel_scaled_fact=(float)2*gravity*0.0305;
	}
	else if(accel==1){
		accel_byte=0b00001000;
		accel_scaled_fact=(float)4*gravity*0.0305;
	}
	else if(accel==2){
		accel_byte=0b00010000;
		accel_scaled_fact=(float)8*gravity*0.0305;
	}
	else if(accel==3){
		accel_byte=0b00011000;
		accel_scaled_fact=(float)16*gravity*0.0305;
	}
	else {
		accel_scaled_fact=1;
	}
	Wire.write(accel_byte);
	Wire.endTransmission();
}

void mpu6050Class::offsetCalibration(){
	int i,x=0,y=0,z=0;
	
	mpu6050.readData();	
	mpu6050.readData();
	
	x=gyro_x;
	y=gyro_y;
	z=gyro_z;
	for(i=1;i<=1000;i++){
		mpu6050.readData();
		x=(x+gyro_x)/2;
		y=(y+gyro_y)/2;
		z=(z+gyro_z)/2;
	}
	gyro_x_OC=x;
	gyro_y_OC=y;
	gyro_z_OC=z;

	x=accel_x;
	y=accel_y;
	z=accel_z;
	for(i=1;i<=1000;i++){
		mpu6050.readData();
		x=(x+accel_x)/2;
		y=(y+accel_y)/2;
		z=(z+accel_z)/2;
	}
	accel_x_OC=x;
	accel_y_OC=y;
	accel_z_OC=z-(float)gravity*1000/accel_scaled_fact;
}

mpu6050Class mpu6050;

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
