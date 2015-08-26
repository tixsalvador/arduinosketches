//Follow me script
//Clean up
//Last edited: Aug 21 2015

#include <Wire.h>
#include <math.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

byte I2Caddress = 0x07;
byte dataAvailable;
int dataSize = 12;
int Xaxis;
int Yaxis;
int Zaxis;
float voltage;
float leftMotorCurrent;
float rightMotorCurrent;

double x, y, z;

byte alternate;

int sonar1=0;

int leftMotorSpeed=100;
int rightMotorSpeed=100;

unsigned long previous_Time_Data = 0;
unsigned long previous_Time_Xbee = 0;

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
	Wire.beginTransmission(I2Caddress);
	buffer[0]=x;
	buffer[1]=leftMotorSpeed >> 8;
	buffer[2]=leftMotorSpeed & 0xFF;
	buffer[3]=rightMotorSpeed >> 8;
	buffer[4]=rightMotorSpeed & 0xFF;
	Wire.write(buffer,5);
	Wire.endTransmission();
}




void forward()
{
	char x='w';
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
	lcd.setCursor(0, 0);
	lcd.print("V:");
	lcd.setCursor(2, 0);
	lcd.print(voltage);
}

int sonar1_data()
{
	int sonarA[10];
	for(int i=0;i<9;i++){
			sonarA[i]=analogRead(A0);
			delayMicroseconds(10);
			sonarA[i]=(sonarA[i]/2)+2;
			sonar1=sonar1+sonarA[i];
	}
	sonar1=sonar1/10;
	return sonar1;
}

void loop()
{
	Serial.println(sonar1_data());
	if((sonar1_data()>=16)&&(sonar1_data()<=40)){
		forward();
	}
	else if((sonar1_data()<=10)&&(sonar1_data()>=2)){
		backward();
	}
	else {
		stop();
	}
	
	if(Serial.available()>0){
		xBee_Control();
	}
}
