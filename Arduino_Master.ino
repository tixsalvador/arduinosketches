//Arduino MASTER

#include<Wire.h>

byte I2Caddress=0x07;
byte dataAvailable;
int dataSize=12;
int Xaxis;
int Yaxis;
int Zaxis;
float voltage;
float leftMotorCurrent;
float rightMotorCurrent;

void setup()
{
	Serial.begin(9600);
	Wire.begin();
	forward_Data();
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

void forward_Data()
{
	byte start=0x0F;
	byte leftMotorBreak=0;
	byte leftMotorDir=1;
	int leftMotorSpeed=255;
	byte rightMotorBreak=0;
	byte rightMotorDir=1;
	int rightMotorSpeed=255;
	byte buffer[9];
	buffer[0]=start;
	buffer[1]=leftMotorBreak;
	buffer[2]=leftMotorDir;
	buffer[3]=leftMotorSpeed >> 8;
	buffer[4]=leftMotorSpeed & 0xFF;	
	buffer[5]=rightMotorBreak;
	buffer[6]=rightMotorDir=1;
	buffer[7]=rightMotorSpeed >> 8;
	buffer[8]=rightMotorSpeed & 0xFF;
	Wire.beginTransmission(I2Caddress);
                Wire.write(buffer,9);
        Wire.endTransmission();
	
}

void loop()
{
	delay(100);
}
