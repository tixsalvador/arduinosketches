#include <Servo.h>

Servo leftMotor;
Servo rightMotor;

void setup()
{
	leftMotor.attach(3);
	rightMotor.attach(4);	
}

void loop()
{
	leftMotor.write(0);
	rightMotor.write(0);
}
