//Arduino MASTER

#include<Wire.h>

byte I2Caddress=0x07;
byte dataAvailable;
byte buffer[1];
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
	make_Forward_Data();
	send_Forward_Data();
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

void make_Forward_Data()
{
	byte start=0x0F;
	buffer[0]=start;
}

void send_Forward_Data()
{
	Wire.beginTransmission(I2Caddress);
		Wire.write(buffer,1);
	Wire.endTransmission();
}

void loop()
{
	delay(100);
}
