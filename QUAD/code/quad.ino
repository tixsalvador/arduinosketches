#include <Wire.h>
#include <Servo.h>

Servo motor1;

int16_t acX,acY,acZ,Tmp,gyX,gyY,gyZ;

const int AccelSen=16384;
const int GyroSen=131;

int speed;

void setup()
{
        Serial.begin(9600);
        Wire.begin();
        Wire.beginTransmission(0x68);
        Wire.write(0x6B);
        Wire.write(0);
        Wire.endTransmission(true);
	motor1.attach(9);
	motor1.write(0);
}

void mpu6050()
{
        Wire.beginTransmission(0x68);
        Wire.write(0x3B);
        Wire.endTransmission(false);
        Wire.requestFrom(0x68,14,true);
        acX=Wire.read()<<8|Wire.read();
        acY=Wire.read()<<8|Wire.read();
        acZ=Wire.read()<<8|Wire.read();
        Tmp=Wire.read()<<8|Wire.read();
        gyX=Wire.read()<<8|Wire.read();
        gyY=Wire.read()<<8|Wire.read();
        gyZ=Wire.read()<<8|Wire.read();
        acX=acX/AccelSen;
        acY=acY/AccelSen;
        acZ=acZ/AccelSen;
        gyX=gyX/GyroSen;
        gyY=gyY/GyroSen;
        gyZ=gyZ/GyroSen;
        Serial.print(acX);
        Serial.print("  ");
        Serial.print(acY);
        Serial.print("  ");
        Serial.print(acZ);
        Serial.print("  ");
        Serial.print(Tmp/340.00+36.53);
        Serial.print("  ");
        Serial.print(gyX);
        Serial.print("  ");
        Serial.print(gyY);
        Serial.print("  ");
        Serial.println(gyZ);
}

void loop()
{
	mpu6050();
	if(acZ==1){
		for(speed=0;speed<=170;speed ++){
			motor1.write(speed);
		}
	}	
	else{
		motor1.write(0);
	}

}
