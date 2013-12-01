#include <Servo.h>

#define panservoPin 1
#define tiltservoPin 2
#define leftmotorPin 3
#define rightmotorPin 4

#define irledPin 10
#define irupPin A5
#define irleftPin A4
#define irdownPin A3
#define irrightPin A2

int panservoCenter=1500;
int tiltservoCenter=1500;
int leftmotorStop=1490;
int rightmotorStop=1500;

int irUpvalue;
int irLeftvalue;
int irDownvalue;
int irRightvalue;
int irDistance;

int distanceMax=180;

int leftmotorSpeed=leftmotorStop;
int rightmotorSpeed=rightmotorStop;

int pan;
int tilt;

Servo panServo;
Servo tiltServo;
Servo leftMotor;
Servo rightMotor;

void setup()
{
	Serial.begin(9600);
	panServo.attach(panservoPin);
	panServo.writeMicroseconds(panservoCenter);
	tiltServo.attach(tiltservoPin);
	tiltServo.writeMicroseconds(tiltservoCenter);
	leftMotor.attach(leftmotorPin);
	leftMotor.writeMicroseconds(leftmotorStop);
	rightMotor.attach(rightmotorPin);
	rightMotor.writeMicroseconds(rightmotorStop);
	pinMode(irledPin, OUTPUT);
}

void loop()
{
	infraDistance();
	Serial.print(irUpvalue);	
	Serial.print(" ");
	Serial.print(irDownvalue);
	Serial.print(" ");
	Serial.print(irLeftvalue);	
	Serial.print(" ");
	Serial.print(irRightvalue);
	Serial.print(" ");
	Serial.println(irDistance);
}

void stop()
{
	leftMotor.writeMicroseconds(leftmotorStop);
	rightMotor.writeMicroseconds(rightmotorStop);
}

void infraDistance()
{
	digitalWrite(irledPin, HIGH);
	delayMicroseconds(100);
	irUpvalue=analogRead(irupPin);
	irLeftvalue=analogRead(irleftPin);
	irDownvalue=analogRead(irdownPin);
	irRightvalue=analogRead(irrightPin);
	digitalWrite(irledPin, LOW);
	delayMicroseconds(100);
	irUpvalue=irUpvalue - analogRead(irupPin);
	irLeftvalue=irLeftvalue - analogRead(irleftPin);
	irDownvalue=irDownvalue - analogRead(irdownPin);
	irRightvalue=irRightvalue - analogRead(irrightPin);
	irDistance=(irUpvalue + irLeftvalue + irDownvalue + irRightvalue) / 4;
}

void infraFollow()
{
	leftmotorSpeed=leftmotorStop;
	rightmotorSpeed=rightmotorStop;
	if(irDistance < distanceMax) {
		if(pan > panservoCenter) {
			pan=pan - 1;
		}
		if(pan < panservoCenter) {
			pan=pan + 1;	
		}
		if(tilt > tiltservoCenter) {
			tilt=tilt - 1;
		}
		if(tilt < tiltservoCenter) {	
			tilt=tilt + 1;
		}
	}
	else
	{	
		
	}
}
