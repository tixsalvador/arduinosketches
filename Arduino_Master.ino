//Arduino MASTER

#include <Wire.h>

byte I2Caddress=0x07;
byte dataAvailable;
int dataSize=12;
int Xaxis;
int Yaxis;
int Zaxis;
float voltage;
float leftMotorCurrent;
float rightMotorCurrent;

byte start;
byte buffer[9];

byte leftMotorBreak;
byte leftMotorDir;
int leftMotorSpeed;

byte rightMotorBreak;
byte rightMotorDir;
int rightMotorSpeed;

unsigned int lSpeed=255;
unsigned int rSpeed=255;

void setup()
{
	Serial.begin(9600);
	Wire.begin();
}

void trex_Sensor_Values()
{
	dataAvailable=Wire.requestFrom(0x07,dataSize);
	if(dataAvailable==dataSize){
		Xaxis=Wire.read()<<8|Wire.read();
		Yaxis=Wire.read()<<8|Wire.read();
		Zaxis=Wire.read()<<8|Wire.read();
		voltage=Wire.read()<<8|Wire.read();
		voltage=voltage/33.57;
		leftMotorCurrent=Wire.read()<<8|Wire.read();
		leftMotorCurrent=((leftMotorCurrent-511)*48.83)/1000;
		rightMotorCurrent=Wire.read()<<8|Wire.read();
		rightMotorCurrent=((rightMotorCurrent-511)*48.83)/1000;
/*		Serial.print(Xaxis);
		Serial.print("\t");
		Serial.print(Yaxis);
		Serial.print("\t");
		Serial.print(Zaxis);
		Serial.print("\t");
		Serial.print(voltage);
		Serial.print("\t");
		Serial.print(leftMotorCurrent);
		Serial.print("\t");
		Serial.println(rightMotorCurrent); */
	}
	else {
		Serial.println("ERROR");
	}
}

void stop()
{
	leftMotorBreak=1;
        rightMotorBreak=1;
        leftMotorDir=1;
        rightMotorDir=1;
        leftMotorSpeed=0;
        rightMotorSpeed=0;
        trex_Send_Data();
}

void forward()
{
	leftMotorBreak=0;
	rightMotorBreak=0;
	leftMotorDir=1;
	rightMotorDir=1;
	leftMotorSpeed=lSpeed;
	rightMotorSpeed=rSpeed;
	trex_Send_Data();
	
}


void backward()
{
        leftMotorBreak=0;
        rightMotorBreak=0;
        leftMotorDir=0;
        rightMotorDir=0;
        leftMotorSpeed=lSpeed;
        rightMotorSpeed=rSpeed;
        trex_Send_Data();

}

void left()
{
        leftMotorBreak=0;
        rightMotorBreak=0;
        leftMotorDir=0;
        rightMotorDir=1;
        leftMotorSpeed=lSpeed;
        rightMotorSpeed=rSpeed;
        trex_Send_Data();

}

void right()
{
        leftMotorBreak=0;
        rightMotorBreak=0;
        leftMotorDir=1;
        rightMotorDir=0;
        leftMotorSpeed=lSpeed;
        rightMotorSpeed=rSpeed;
        trex_Send_Data();

}


void trex_Send_Data()
{
	start=0x0F;
	buffer[0]=start;
	buffer[1]=leftMotorBreak;
	buffer[2]=leftMotorDir;
	buffer[3]=leftMotorSpeed >> 8;
	buffer[4]=leftMotorSpeed & 0xFF;	
	buffer[5]=rightMotorBreak;
	buffer[6]=rightMotorDir;
	buffer[7]=rightMotorSpeed >> 8;
	buffer[8]=rightMotorSpeed & 0xFF;
	Wire.beginTransmission(I2Caddress);
                Wire.write(buffer,9);
        Wire.endTransmission();
	
}

void loop()
{
	delay(100);
	forward();
	delay(10000);
	stop();
	delay(10000);
	backward();
	delay(10000);
	left();
	delay(10000);
	right();
	delay(10000);


}
