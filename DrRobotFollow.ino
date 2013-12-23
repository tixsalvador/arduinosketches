#include <Servo.h>

#define xservoPin 9

/*
int xservoMin=600;
int xservoCenter=1500;
int xservoMax=2200;
*/

int xservoMin=0;
int xservoCenter=90;
int xservoMax=170;

Servo xservo;


void setup()
{
	xservo.attach(xservoPin);
}

void loop()
{
	xservo.write(170);
	delay(1000);
	xservo.write(90);
	delay(1000);
	xservo.write(0);
	delay(1000);
}
