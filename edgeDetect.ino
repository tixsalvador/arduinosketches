#include <Servo.h>

Servo leftServo;
Servo rightServo;

#define led1 5
#define led2 6

#define ledsenPin1 12
#define ledsenPin2 11

int irSen1;
int irSen2;

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
	irSen();
	if(irSen1 == 1 || irSen2 == 1) {
		forward();
	}
	else {
		stop();
	}
	Serial.print(irSen1);
	Serial.print(" ");
	Serial.println(irSen2);
}
