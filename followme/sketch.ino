//Follow me script
//Clean up
//Added speed control
//Last edited: Sep 4 2015


#include <Wire.h>
#include <math.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

#define interruptPin 2

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

unsigned int leftMotorSpeed;
unsigned int rightMotorSpeed;

unsigned long previous_Time_Data = 0;
unsigned long previous_Time_Xbee = 0;


int sonar1=0;
int sonar2=0;

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
	digitalWrite(interruptPin,HIGH);
	attachInterrupt(0,shutdown,FALLING);
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
	leftMotorSpeed=220;
	rightMotorSpeed=220;
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

void left_turn()
{
	char x='a';
	leftMotorSpeed=120;
        rightMotorSpeed=120;
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

void right_turn()
{
	char x='d';
	leftMotorSpeed=120;
        rightMotorSpeed=120;
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

void shutdown()
{	
	char x='t';
	leftMotorSpeed=0;
	rightMotorSpeed=0;
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
	//default values x:207; y:138; z:329
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
	lcd.setCursor(0,0);
	lcd.print("Volt:");
	lcd.setCursor(5,0);
	lcd.print(voltage);
	lcd.setCursor(11,0);
	lcd.print("Dir:");
	lcd.setCursor(15,0);
	if(direction==1){
		lcd.print("Left");
	}
	else if(direction==2){
		lcd.print("Right");
	}
	else if(direction==3){
		lcd.print("Front");
	}
	lcd.setCursor(0,1);
	lcd.print(x);
	lcd.setCursor(7,1);
	lcd.print(y);
	lcd.setCursor(14,1);
	lcd.print(z);
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
		
	}
	else if((right>left)&&(right>front)){
		direction=2;
	}
	else if((front>left)&&(front>right)){
		direction=3;
	}
}

void d2_na_me()
{
	if((direction==1)&&(sonar_average>=5)&&(sonar_average<=30)){
		left_turn();
	}
	else if((direction==2)&&(sonar_average>=5)&&(sonar_average<=30)){
		right_turn();
	}
	else if((direction==3)&&(sonar_left()<=15)&&(sonar_left()>=2)||(sonar_right()<=15)&&(sonar_right()>=2)){
		backward();
	}
	else if((direction==3)&&(sonar_left()>=20)&&(sonar_left()<=50)||(sonar_right()>=20)&&(sonar_right()<=50)){
	// else if((direction==3)&&(sonar_average>20)&&(sonar_average<=50)){
		if(sonar_average>=35){
			forward_speedUp();
		}
		else {
			forward_normal();
		}
	}
	else {
		stop();
	}
}

void loop()
{
	trex_Sensor_Values();
	if(voltage>6.50){
		digitalWrite(interruptPin,HIGH);
		check_angle_magnitude();
		show_lcd_data();
		if(Serial.available()>0){
			xBee_Control();
		}	
		else {
			where_na_u();
			d2_na_me();
		}
	}
	else {
		digitalWrite(interruptPin,LOW);
		Serial.print("Battery Low:");
		Serial.print("\t");
		Serial.println(voltage);
	}	
/*
	Serial.print(x);
	Serial.print("\t");
	Serial.print(y);
	Serial.print("\t");
	Serial.println(z);
*/
}
