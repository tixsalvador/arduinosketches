//Arduino MASTER_MANUAL

#include <Wire.h>
#include <math.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);

byte I2Caddress=0x07;
byte dataAvailable;
int dataSize=12;
int Xaxis;
int Yaxis;
int Zaxis;
float voltage;
float leftMotorCurrent;
float rightMotorCurrent;

double x,y,z;

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

byte alternate;
unsigned long previous_Time_Data=0;

unsigned long previous_Time_Xbee=0;

union lat2bytes
{
	float latf;
	char latb[sizeof(float)];
};

union lon2bytes
{
	float lonf;
	char lonb[sizeof(float)];
};

float latitude;
float longitude;

void setup()
{
	Serial.begin(9600);
	Wire.begin();
	lcd.init();
	lcd.init();
	lcd.backlight();
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
/*
		Serial.print(Xaxis);
		Serial.print("\t");
		Serial.print(Yaxis);
		Serial.print("\t");
		Serial.print(Zaxis);
		Serial.print("\t");
		Serial.print(voltage);
		Serial.print("\t");
		Serial.print(leftMotorCurrent);
		Serial.print("\t");
		Serial.println(rightMotorCurrent);
*/
	}
	else {
		Serial.println("ERROR");
	}
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
	Wire.beginTransmission(I2Caddress);
    Wire.write(x);
    Wire.endTransmission();
}

void check_angle_magnitude()
{
	int min=529;
	int max=724;
	int xAng=map(Xaxis,min,max,-90,90);
	int yAng=map(Yaxis,min,max,-90,90);
	int zAng=map(Zaxis,min,max,-90,90);
	x = RAD_TO_DEG * (atan2(-yAng, -zAng) + PI);
  	y = RAD_TO_DEG * (atan2(-xAng, -zAng) + PI);
 	z = RAD_TO_DEG * (atan2(-yAng, -xAng) + PI);
/*	
	Serial.print("x: ");
  	Serial.print(x);
	Serial.print(" | y: ");
  	Serial.print(y);
  	Serial.print(" | z: ");
  	Serial.println(z);
*/
}

void get_GPS_data()
{
	lat2bytes la2b;
	lon2bytes lo2b;
	Wire.requestFrom(6,8);
	while(Wire.available())
	{
		la2b.latb[0]=Wire.read();
		la2b.latb[1]=Wire.read();
		la2b.latb[2]=Wire.read();
		la2b.latb[3]=Wire.read();
		latitude=la2b.latf;
		lo2b.lonb[0]=Wire.read();
		lo2b.lonb[1]=Wire.read();
		lo2b.lonb[2]=Wire.read();
		lo2b.lonb[3]=Wire.read();
		longitude=lo2b.lonf;
	}
	Serial.print(latitude,6);
	Serial.print("\t");
	Serial.println(longitude,6);
}

void lcd_Gps_Angle()
{
	lcd.setCursor(0,0);
        lcd.print("Latitude:");
        lcd.setCursor(9,0);
        lcd.print(latitude,6);
        lcd.setCursor(0,1);
        lcd.print("Longitude:");
        lcd.setCursor(10,1);
        lcd.print(longitude,6);
	lcd.setCursor(0,2);
	lcd.print("x:");
	lcd.setCursor(2,2);
	lcd.print(x);
	lcd.setCursor(11,2);
	lcd.print("y:");
	lcd.setCursor(13,2);
	lcd.print(y);
	lcd.setCursor(0,3);
	lcd.print("z:");
	lcd.setCursor(2,3);
	lcd.print(y);
	lcd.setCursor(11,3);
	lcd.print("V:");
	lcd.setCursor(13,3);
	lcd.print(voltage);
}

void loop()
{
	delay(100);
	trex_Sensor_Values();
	while(voltage > 6.50){
		unsigned int timeDelay=500;
		if(millis()-previous_Time_Data>=timeDelay){
			alternate=alternate^1;
			previous_Time_Data=millis();
			if(alternate){
				trex_Sensor_Values();
				check_angle_magnitude();
			}
			else{
				get_GPS_data();
			}
		}
		if(Serial.available()>0){
			xBee_Control();
		}
		lcd_Gps_Angle();
	} Serial.println("Battery charge low");
}
