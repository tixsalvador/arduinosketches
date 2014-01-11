#include <Servo.h>

Servo leftMotor;
Servo rightMotor;

int xservoCenter=75;
int xservoLeft=160;
int xservoRight=0;
int yservoCenter=120;
int yservoUp=0;
int yservoDown=140;


void setup()
{
	leftMotor.attach(9);
	rightMotor.attach(10);	
}

void loop()
{
	leftMotor.write(xservoCenter);
	rightMotor.write(yservoCenter);
	delay(2000);
	leftMotor.write(xservoLeft);
	rightMotor.write(yservoDown);
	delay(2000);
	leftMotor.write(xservoRight);
	rightMotor.write(yservoUp);
	delay(2000);
}
