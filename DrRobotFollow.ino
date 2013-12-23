#include <Servo.h>

#define irdownPin A2
#define irrightPin A3
#define irupPin A4
#define irleftPin A5
#define irledPin 11
#define xservoPin 9

int irdownValue;
int irrightValue;
int irupValue;
int irleftValue;
int irDistance;

int xservoMin=0;
int xservoCenter=90;
int xservoMax=170;

Servo xservo;


void setup()
{
	Serial.begin(9600);
	xservo.attach(xservoPin);
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

