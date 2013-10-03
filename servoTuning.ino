/*
servo.write X 0-right, 50 center, 100 left;
servo.write Y 110 center, 170 down, 10 up

servo.writeMicroseconds X 1100 center, 1650 left, 550 right
servo.writeMicroseconds Y 1600 center, 2050 down, 800 up
*/

#include <Servo.h>
Servo xservo;
Servo yservo;

int xCenter=1100;
int xLeft=1650;
int xRight=550;
int yCenter=1650;
int yDown=2050;
int yUp=1150;


void setup()
{
	xservo.attach(9);
	yservo.attach(10);
}

void loop()
{
		 	
	xservo.writeMicroseconds(xCenter);
	yservo.writeMicroseconds(yCenter);
	delay(5000);
	xservo.writeMicroseconds(xRight);	
	yservo.writeMicroseconds(yDown);		
	delay(5000);
	xservo.writeMicroseconds(xCenter);
	yservo.writeMicroseconds(yCenter);
        delay(5000);
	xservo.writeMicroseconds(xLeft);
	yservo.writeMicroseconds(yUp);
	delay(5000);
}
