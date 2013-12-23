#include <Servo.h>

#define xservoPin 9

int xservoMin=600;
int xservoCenter=1500;
int xservoMax=2200;

Servo xservo;


void setup()
{
	xservo.attach(xservoPin);
}

void loop()
{
	xservo.writeMicroseconds(2300);
	/*
	delay(1000);
	xservo.write(xservoCenter);
	delay(1000);
	xservo.write(xservoMax);
	delay(1000);
	*/
}
