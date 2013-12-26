#include <Servo.h>

#define irdownPin A2
#define irrightPin A3
#define irupPin A4
#define irleftPin A5
#define irledPin 11
#define xservoPin 9
#define yservoPin 10

int irdownValue;
int irrightValue;
int irupValue;
int irleftValue;
int irDistance;

int distanceMax=100;

int xservoMin=0;
int xservoCenter=90;
int xservoMax=170;
int pan=xservoCenter;
int panScale;
byte panscaleFactor=12;
byte panservoMultiplier=10;
int x;

int yservoMin=0;
int yservoCenter=90;
int yservoMax=170;
int tilt=yservoCenter;
int tiltScale;
byte tiltscaleFactor=12;
byte tiltservoMultiplier=10;
int y;

Servo xservo;
Servo yservo;


void setup()
{
	Serial.begin(9600);
	xservo.attach(xservoPin);
	xservo.write(xservoCenter);
	yservo.attach(yservoPin);
	yservo.write(yservoCenter);
	pinMode(irledPin, OUTPUT);
}

void loop()
{
	getirDistance();
	
	/*
	Serial.print(irDistance);
	Serial.print(" ");
	Serial.print(irdownValue);
	Serial.print(" ");	
	Serial.print(irupValue);
	Serial.print(" ");
	Serial.print(irleftValue);
	Serial.print(" ");
	Serial.println(irrightValue);
	*/
}

void getirDistance()
{
	digitalWrite(irledPin, HIGH);
	delayMicroseconds(100);
	irdownValue=analogRead(irdownPin);
	irrightValue=analogRead(irrightPin);
	irupValue=analogRead(irupPin);
	irleftValue=analogRead(irleftPin);
	digitalWrite(irledPin, LOW);
	delayMicroseconds(100);
	irdownValue=irdownValue - analogRead(irdownPin);
	irrightValue=irrightValue - analogRead(irrightPin);
	irupValue=irupValue - analogRead(irupPin);
	irleftValue=irleftValue - analogRead(irleftPin);
	irDistance=(irdownValue + irrightValue + irupValue + irleftValue) / 4;
}

void irFollow()
{
	if(irDistance < distanceMax) {
		if(tilt > yservoCenter) {
			tilt=tilt - 1;
		}
		if(tilt < yservoCenter) {
			tilt=tilt + 1;
		}
	}
	else {
		tiltScale=(irupValue + irdownValue) * tiltscaleFactor / 10;
		if(irupValue < irdownValue) {
			y=(irdownValue - irupValue) * tiltservoMultiplier / tiltScale;
			tilt=tilt - x;
		}
	} 
	
}
