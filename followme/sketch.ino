//Follow me script
//Clean up
//Added speed control
//Last edited: Sep 4 2015


#include <Wire.h>
#include <math.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

byte I2Caddress = 0x07;
byte dataAvailable;
const int dataSize = 12;
int Xaxis;
int Yaxis;
int Zaxis;
float voltage;
float leftMotorCurrent;
float rightMotorCurrent;

double x, y, z;

byte alternate;

unsigned int leftMotorSpeed;
unsigned int rightMotorSpeed;

unsigned long previous_Time_Data = 0;
unsigned long previous_Time_Xbee = 0;


int sonar1=0;
int sonar2=0;
const int forwardMaxDistance=50;
const int forwardMinDistance=16;
const int backwardMaxDistance=15; //10 - Aug 26
const int backwardMinDistance=2;
const int calibrate=5;

int sonar_average;
int sonar_calibrate;

byte direction;

void setup()
{
	Serial.begin(9600);
	Wire.begin();
	lcd.init();
	lcd.backlight();
}

void trex_Sensor_Values()
{
	dataAvailable = Wire.requestFrom(0x07, dataSize);
	if(dataAvailable == dataSize)
	{
		Xaxis = Wire.read() << 8 | Wire.read();
		Yaxis = Wire.read() << 8 | Wire.read();
		Zaxis = Wire.read() << 8 | Wire.read();
		voltage = Wire.read() << 8 | Wire.read();
		voltage = voltage / 33.57;
		leftMotorCurrent = Wire.read() << 8 | Wire.read();
		leftMotorCurrent = ((leftMotorCurrent - 511) * 48.83) / 1000;
		rightMotorCurrent = Wire.read() << 8 | Wire.read();
		rightMotorCurrent = ((rightMotorCurrent - 511) * 48.83) / 1000;
	}
	else
	{
		Serial.println("ERROR");
	}
}

void xBee_Control()
{
	byte buffer[5];
	char x = Serial.read();
	leftMotorSpeed=100;
	rightMotorSpeed=100;
	Wire.beginTransmission(I2Caddress);
	buffer[0]=x;
	buffer[1]=leftMotorSpeed >> 8;
	buffer[2]=leftMotorSpeed & 0xFF;
	buffer[3]=rightMotorSpeed >> 8;
	buffer[4]=rightMotorSpeed & 0xFF;
	Wire.write(buffer,5);
	Wire.endTransmission();
}

void forward_normal()
{
	char x='w';
	leftMotorSpeed=100;
	rightMotorSpeed=100;
	byte buffer[5];
	Wire.beginTransmission(I2Caddress);
	buffer[0]=x;
	buffer[1]=leftMotorSpeed >> 8;
	buffer[2]=leftMotorSpeed & 0xFF;
	buffer[3]=rightMotorSpeed >> 8;
	buffer[4]=rightMotorSpeed & 0xFF;
	Wire.write(buffer,5);
	Wire.endTransmission();
}

void forward_speedUp()
{
	char x='w';
	leftMotorSpeed=255;
	rightMotorSpeed=255;
	byte buffer[5];
	Wire.beginTransmission(I2Caddress);
	buffer[0]=x;
	buffer[1]=leftMotorSpeed >> 8;
	buffer[2]=leftMotorSpeed & 0xFF;
	buffer[3]=rightMotorSpeed >> 8;
	buffer[4]=rightMotorSpeed & 0xFF;
	Wire.write(buffer,5);
	Wire.endTransmission();
}

void left()
{
	char x='a';
	byte buffer[5];
	Wire.beginTransmission(I2Caddress);
	buffer[0]=x;
	buffer[1]=leftMotorSpeed >> 8;
	buffer[2]=leftMotorSpeed & 0xFF;
	buffer[3]=rightMotorSpeed >> 8;
	buffer[4]=rightMotorSpeed & 0xFF;
	Wire.write(buffer,5);
	Wire.endTransmission();
}

void right()
{
	char x='d';
	byte buffer[5];
	Wire.beginTransmission(I2Caddress);
	buffer[0]=x;
	buffer[1]=leftMotorSpeed >> 8;
	buffer[2]=leftMotorSpeed & 0xFF;
	buffer[3]=rightMotorSpeed >> 8;
	buffer[4]=rightMotorSpeed & 0xFF;
	Wire.write(buffer,5);
	Wire.endTransmission();
}

void backward()
{
	char x='s';
	leftMotorSpeed=100;
	rightMotorSpeed=100;
	byte buffer[5];
	Wire.beginTransmission(I2Caddress);
	buffer[0]=x;
	buffer[1]=leftMotorSpeed >> 8;
	buffer[2]=leftMotorSpeed & 0xFF;
	buffer[3]=rightMotorSpeed >> 8;
	buffer[4]=rightMotorSpeed & 0xFF;
	Wire.write(buffer,5);
	Wire.endTransmission();
}

void stop()
{	
	char x='t';
	byte buffer[5];
	Wire.beginTransmission(I2Caddress);
	buffer[0]=x;
	buffer[1]=leftMotorSpeed >> 8;
	buffer[2]=leftMotorSpeed & 0xFF;
	buffer[3]=rightMotorSpeed >> 8;
	buffer[4]=rightMotorSpeed & 0xFF;
	Wire.write(buffer,5);
	Wire.endTransmission();
}

void check_angle_magnitude()
{
	int min = 529;
	int max = 724;
	int xAng = map(Xaxis, min, max, -90, 90);
	int yAng = map(Yaxis, min, max, -90, 90);
	int zAng = map(Zaxis, min, max, -90, 90);
	x = RAD_TO_DEG * (atan2(-yAng, -zAng) + PI);
	y = RAD_TO_DEG * (atan2(-xAng, -zAng) + PI);
	z = RAD_TO_DEG * (atan2(-yAng, -xAng) + PI);
}


void show_lcd_data()
{
	lcd.setCursor(6,0);
	lcd.print("ROBORAT");
}

int sonar_left()
{
	int sonarA[10];
	for(int i=0;i<9;i++){
			sonarA[i]=analogRead(A1);
			delayMicroseconds(10);
			sonarA[i]=(sonarA[i]/2)+2;
			sonar1=sonar1+sonarA[i];
	}
	sonar1=sonar1/10;
	return sonar1;
}

int sonar_right()
{
	int sonarB[10];
	for(int i=0;i<9;i++){
			sonarB[i]=analogRead(A0);
			delayMicroseconds(10);
			sonarB[i]=(sonarB[i]/2)+2;
			sonar2=sonar2+sonarB[i];
	}
	sonar2=sonar2/10;
	return sonar2;
}

void where_na_u()
{	
	int left=0;
	int right=0;
	int front=0;
	for(int i=0;i<21;i++){
		delayMicroseconds(500);
		sonar_average=(sonar_left()+sonar_right())/2;
		sonar_calibrate=sonar_average+calibrate;
	
		if(sonar_left()>sonar_calibrate){
			left=left+1;
		}
		else if(sonar_right()>sonar_calibrate){
			right=right+1;
		}
		else 
			front=front+1;
	}
	
	if((left>right)&&(left>front)){
		direction=1;
		Serial.println("Left");
	}
	else if((right>left)&&(right>front)){
		direction=2;
		Serial.println("Right");
	}
	else if((front>left)&&(front>right)){
		direction=3;
		Serial.println("Front");
	}

	lcd.setCursor(10,0);
	lcd.print(direction);
}

void loop()
{
	where_na_u();
}
 	/*	
	if((sonar_left()>=forwardMinDistance)&&(sonar_left()<=forwardMaxDistance)&&(sonar_right()>=forwardMinDistance)&&(sonar_right()<=forwardMaxDistance)){
		forward_normal();
	}
	else if((sonar_left()<=backwardMaxDistance)&&(sonar_left()>=backwardMinDistance)||(sonar_right()<=backwardMaxDistance)&&(sonar_right()>=backwardMinDistance)){
		backward();
	}
	else {
		stop();
	}
	
	
	Serial.print(sonar_left());
	Serial.print("	");
	Serial.print(sonar_right());
	Serial.print("	");
	Serial.print(sonar_average);
	Serial.print("	");
	Serial.println(sonar_calibrate);
	
	show_lcd_data();
	*/

