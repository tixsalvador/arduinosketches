#include <Servo.h>

#define panservoPin 7
#define tiltservoPin 2
#define leftmotorPin 3
#define rightmotorPin 4

#define irledPin 10
#define irupPin A5
#define irleftPin A4
#define irdownPin A3
#define irrightPin A2

int panservoCenter=90;
int xservoMin=0;
int xservoMax=180;
int tiltservoCenter=90;
int leftmotorStop=1490;
int rightmotorStop=1500;

int irUpvalue;
int irLeftvalue;
int irDownvalue;
int irRightvalue;
int irDistance;

int distanceMax=250;

int leftmotorSpeed=leftmotorStop;
int rightmotorSpeed=rightmotorStop;

int pan=panservoCenter;
int panScale;
byte panscaleFactor=5;
byte servoMultiplier=10;
int x;
int panLeftRight;

int tilt=tiltservoCenter;
 
Servo panServo;
Servo tiltServo;
Servo leftMotor;
Servo rightMotor;

void setup()
{
	Serial.begin(9600);
	panServo.attach(panservoPin);
	panServo.write(panservoCenter);
	tiltServo.attach(tiltservoPin);
	tiltServo.write(tiltservoCenter);
	leftMotor.attach(leftmotorPin);
	leftMotor.writeMicroseconds(leftmotorStop);
	rightMotor.attach(rightmotorPin);
	rightMotor.writeMicroseconds(rightmotorStop);
	pinMode(irledPin, OUTPUT);
}

void loop()
{ 
	panServo.write(pan);	
	infraDistance();
	infraFollow();
	Serial.print(irLeftvalue);
	Serial.print(" ");
	Serial.print(irRightvalue);
	Serial.print(" ");
	Serial.println(pan);
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
		panScale=(irLeftvalue + irRightvalue) * panscaleFactor / 10;
		if(irLeftvalue < irRightvalue) {
			x=(irLeftvalue - irRightvalue) * servoMultiplier / panScale;
			pan=pan - x;
		}
		if(irLeftvalue > irRightvalue) {
			x=(irRightvalue - irLeftvalue) * servoMultiplier / panScale;
			pan=pan +x;
		} 
	}
if(pan < xservoMin) {
	pan=xservoMin;
}
if(pan > xservoMax) {
	pan=xservoMax;
}

}
