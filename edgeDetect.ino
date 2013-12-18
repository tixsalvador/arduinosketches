#include <Servo.h>

Servo leftServo;
Servo rightServo;

#define led1 5
#define led2 6

#define ledsenPin1 12
#define ledsenPin2 11

int irSen1;
int irSen2;

byte lightChase;
long time;

void setup()
{
	Serial.begin(9600);
	leftServo.attach(3);
	rightServo.attach(4);
	pinMode(led1, OUTPUT);
	pinMode(led2, OUTPUT);
}

void irSen() 
{
	digitalWrite(led1, HIGH);
	digitalWrite(led2, HIGH);
	delayMicroseconds(100);

	irSen1=digitalRead(ledsenPin1);
	irSen2=digitalRead(ledsenPin2);

	digitalWrite(led1, LOW);
	digitalWrite(led2, LOW);
	delayMicroseconds(100);

	digitalWrite(led1,(lightChase == 1 || irSen1 == 0));
	digitalWrite(led2,(lightChase == 2 || irSen2 == 0));
}

void stop()
{
	leftServo.detach();
	rightServo.detach();
}

void reAttach()
{
	if(!leftServo.attached())
		leftServo.attach(3);
	if(!rightServo.attached())
		rightServo.attach(4);	
}
void forward()
{
	reAttach();
	leftServo.write(80);
        rightServo.write(100);
}

void loop()
{
	if(millis() - time >199)
	{
		time=millis();
		lightChase=lightChase + 1 - 4 * (lightChase > 3);
	}
	
	irSen();
	if(irSen1 == 1 || irSen2 == 1) {
		forward();
	}
	else {
		stop();
	}
}
