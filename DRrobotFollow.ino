#include<Servo.h>

#define irupPin A4
#define irdownPin A2
#define irleftPin A5
#define irrightPin A3
#define irledPin 11
#define leftFmotor 6 //M1 Speed Control 
#define rightFmotor 5 //M2 Speed Control
#define leftRmotor 8 //M1 Direction Control
#define rightRmotor 7 //M2 Direction Control

int leftmotorSpeed = 255;
int rightmotorSpeed = 255;

int irupValue;
int irdownValue;
int irleftValue;
int irrightValue;
int irDistance;
int minforwardDistance = 100;
int maxforwardDistance = 400;
int maxbackwardDistance = 500;

#define ultrasensorPin 3

Servo xservo;
Servo yservo;
#define xservoPin 9
#define yservoPin 10
int xservoCenter = 75;
int yservoCenter = 120;

void setup()
{
	Serial.begin(9600);
	xservo.attach(xservoPin);
	xservo.write(xservoCenter);
	yservo.attach(yservoPin);
	yservo.write(yservoCenter);
	pinMode(irledPin, OUTPUT);
	for(int i=5;i<=8;i++) {
  		pinMode(i, OUTPUT);
  	}
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

void stop()
{
	digitalWrite(leftFmotor, LOW);
        digitalWrite(rightFmotor, LOW);
        digitalWrite(leftRmotor, LOW);
        digitalWrite(rightRmotor, LOW);
}

void forward()
{
	analogWrite(leftFmotor, leftmotorSpeed);
	analogWrite(rightFmotor, rightmotorSpeed);
	digitalWrite(leftRmotor, LOW);
	digitalWrite(rightRmotor, LOW);
}

void backward()
{
	analogWrite(leftFmotor, leftmotorSpeed);
        analogWrite(rightFmotor, rightmotorSpeed);
        digitalWrite(leftRmotor, HIGH);
        digitalWrite(rightRmotor, HIGH);
}

void loop()
{
	getirDistance();
	if(irDistance > minforwardDistance && irDistance < maxforwardDistance) {
		forward();
	}
	else if(irDistance > maxbackwardDistance) {
		backward();
	}
	else {
		stop();
	}
/*
	Serial.print(irDistance);
	Serial.print(" ");
	Serial.print(irupValue);
	Serial.print(" ");
	Serial.print(irdownValue);
	Serial.print(" ");
	Serial.print(irleftValue);
	Serial.print(" ");
	Serial.println(irrightValue);
*/
}
