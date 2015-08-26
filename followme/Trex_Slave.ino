//TREX SLAVE
//6/27/15 - Clean up

#include <EEPROM.h>
#include <Wire.h>

#define leftMotorDirPin 2
#define leftMotorPWMPin 3
#define leftMotorBreakPin 4

#define rightMotorBreakPin 9
#define rightMotorDirPin 10
#define rightMotorPWMPin 11

byte I2Caddress;
int Xaxis;
int Yaxis;
int Zaxis;
int voltage;
int leftMotorCurrent;
int rightMotorCurrent;

void setup()
{
	Serial.begin(9600);
	check_I2C_address();
	Wire.begin(I2Caddress);
	Wire.onRequest(send_Sensor_Data);
	Wire.onReceive(receive_Data);
	int m[]={2,3,4,9,10,11};
	for(int i=0;i<6;i++){
		pinMode(m[i],OUTPUT);
	}
}

void check_I2C_address()
{
	byte n=EEPROM.read(0);
	if(n==0x55){
		I2Caddress=EEPROM.read(1);
	}
	else {
		EEPROM.write(0,0x55);
		EEPROM.write(1,0x07);
		I2Caddress=EEPROM.read(1);
	}
}

void check_Trex_Sensors()
{
	Xaxis=analogRead(A0);
	Yaxis=analogRead(A1);
	Zaxis=analogRead(A2);
	voltage=analogRead(A3);
	leftMotorCurrent=analogRead(A6);
	rightMotorCurrent=analogRead(A7);
}

void send_Sensor_Data()
{
	byte buffer[12];
	buffer[0]=Xaxis >> 8;
	buffer[1]=Xaxis & 0xFF;
	buffer[2]=Yaxis >> 8;
	buffer[3]=Yaxis & 0xFF;
	buffer[4]=Zaxis >> 8;
	buffer[5]=Zaxis & 0xFF;
	buffer[6]=voltage >> 8;
	buffer[7]=voltage & 0xFF;
	buffer[8]=leftMotorCurrent >> 8;
	buffer[9]=leftMotorCurrent & 0xFF;
	buffer[10]=rightMotorCurrent >> 8;
	buffer[11]=rightMotorCurrent & 0xFF;
	Wire.write(buffer,12);
}

void stop(int leftMotorSpeed, int rightMotorSpeed)
{
	digitalWrite(leftMotorBreakPin,1);
	digitalWrite(rightMotorBreakPin,1);
	digitalWrite(leftMotorDirPin,1);
	digitalWrite(rightMotorDirPin,1);
	analogWrite(leftMotorPWMPin,leftMotorSpeed);
	analogWrite(rightMotorPWMPin,rightMotorSpeed);
}

void forward(int leftMotorSpeed, int rightMotorSpeed)
{
	digitalWrite(leftMotorBreakPin,0);
	digitalWrite(rightMotorBreakPin,0);
	digitalWrite(leftMotorDirPin,0);
	digitalWrite(rightMotorDirPin,0);
	analogWrite(leftMotorPWMPin,leftMotorSpeed);
	analogWrite(rightMotorPWMPin,rightMotorSpeed);
}

void backward(int leftMotorSpeed, int rightMotorSpeed)
{
	digitalWrite(leftMotorBreakPin,0);
	digitalWrite(rightMotorBreakPin,0);
	digitalWrite(leftMotorDirPin,1);
	digitalWrite(rightMotorDirPin,1);
	analogWrite(leftMotorPWMPin,leftMotorSpeed);
	analogWrite(rightMotorPWMPin,rightMotorSpeed);
}

void left(int leftMotorSpeed, int rightMotorSpeed)
{
	digitalWrite(leftMotorBreakPin,0);
	digitalWrite(rightMotorBreakPin,0);
	digitalWrite(leftMotorDirPin,1);
	digitalWrite(rightMotorDirPin,0);
	analogWrite(leftMotorPWMPin,leftMotorSpeed);
	analogWrite(rightMotorPWMPin,rightMotorSpeed);
}

void right(int leftMotorSpeed, int rightMotorSpeed)
{
	digitalWrite(leftMotorBreakPin,0);
	digitalWrite(rightMotorBreakPin,0);
	digitalWrite(leftMotorDirPin,0);
	digitalWrite(rightMotorDirPin,1);
	analogWrite(leftMotorPWMPin,leftMotorSpeed);
	analogWrite(rightMotorPWMPin,rightMotorSpeed);
}

void receive_Data(int receiveDataBytes)
{
	if(Wire.available()>0){
		char x=Wire.read();
		int leftMotorSpeed=Wire.read()<<8|Wire.read();
		int rightMotorSpeed=Wire.read()<<8|Wire.read();
		switch(x){
			case 'w':
				forward(leftMotorSpeed,rightMotorSpeed);
				break;
			case 's':
				backward(leftMotorSpeed,rightMotorSpeed);
				break;
			case 'a':
				left(leftMotorSpeed,rightMotorSpeed);
				break;
			case 'd':
				right(leftMotorSpeed,rightMotorSpeed);
				break;
			default:
				stop(leftMotorSpeed,rightMotorSpeed);
				break;
		}
	}
}

void loop()
{
	delay(100);
	check_Trex_Sensors();	
}
