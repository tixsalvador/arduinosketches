//Arduino MASTER

#include <Wire.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <SD.h>

SoftwareSerial sS(2,3);
TinyGPSPlus gps;

File myFile;

const int sdCard=10;

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

unsigned long previous_Time=0;

void setup()
{
	Serial.begin(9600);
	sS.begin(9600);
	Wire.begin();
	pinMode(6,OUTPUT);
	pinMode(10,OUTPUT);
	digitalWrite(6,LOW);
	check_SDCard();
}

void check_SDCard()
{
	if(!SD.begin(sdCard)){
		Serial.println("Card not Present");
		return;
	}
	if(!SD.exists("gpsLog.txt")){
		myFile=SD.open("gpsLog.txt",FILE_WRITE);
		myFile.close();
		Serial.println("Created File");
	}
	myFile=SD.open("gpsLog.txt",FILE_WRITE);
	if(myFile){
		if(myFile.size()>1000000){
			SD.remove("gpsLog.txt");
			myFile=SD.open("gpsLog.txt",FILE_WRITE);
			myFile.close();
		}
	}
}


void gps_Stats()
{
	while(sS.available()>0){
		gps.encode(sS.read());
		if(gps.location.isUpdated()){
			myFile=SD.open("gpsLog.txt",FILE_WRITE);
			if(myFile){
				myFile.print(gps.date.value());
				myFile.print(", ");
				myFile.print(gps.time.value());
				myFile.print(", ");
				myFile.print(gps.altitude.value());
				myFile.print(", ");
				myFile.print(gps.speed.mps());
				myFile.print(", ");
				myFile.print(gps.location.lat());
				myFile.print(", ");
				myFile.print(gps.location.lng());
				myFile.print(", ");
				myFile.println(gps.satellites.value());
				myFile.close();
			}
		}
/*
		myFile=SD.open("gpsLog.txt");
		if(myFile){
			while(myFile.available()){
				Serial.write(myFile.read());
			}myFile.close();
		}
*/
	}
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

void backward()
{
	leftMotorBreak=0;
	rightMotorBreak=0;
	leftMotorDir=1;
	rightMotorDir=1;
	leftMotorSpeed=lSpeed;
	rightMotorSpeed=rSpeed;
	trex_Send_Data();
	
}


void forward()
{
        leftMotorBreak=0;
        rightMotorBreak=0;
        leftMotorDir=0;
        rightMotorDir=0;
        leftMotorSpeed=lSpeed;
        rightMotorSpeed=rSpeed;
        trex_Send_Data();

}

void right()
{
        leftMotorBreak=0;
        rightMotorBreak=0;
        leftMotorDir=0;
        rightMotorDir=1;
        leftMotorSpeed=150;
        rightMotorSpeed=150;
        trex_Send_Data();

}

void left()
{
        leftMotorBreak=0;
        rightMotorBreak=0;
        leftMotorDir=1;
        rightMotorDir=0;
        leftMotorSpeed=150;
        rightMotorSpeed=150;
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

void xBee_Control()
{
		char x=Serial.read();
		if(x=='w'){
			forward();
		}
		else if(x=='s'){
			backward();
		}
		else if(x=='a'){
			left();
		}
		else if(x=='d'){
			right();
		}
		else {
			stop();
		}
}

void loop()
{
	delay(100);
//	trex_Sensor_Values();
	gps_Stats();
	while(voltage > 6.50){
		unsigned int timeDelay=5000;
		if(millis()-previous_Time>=timeDelay){
			trex_Sensor_Values();
			previous_Time=millis();
		}
		if(Serial.available()>0){
			xBee_Control();
		}
	}
}
