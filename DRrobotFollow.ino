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
int followDistance = 400;
int minforwardDistance = 200;
int maxforwardDistance = 50;
int maxbackwardDistance = 600;

#define ultrasensorPin 3

Servo xservo;
Servo yservo;
#define xservoPin 9
#define yservoPin 10
int xservoCenter = 75;
int yservoCenter = 120;
int pan=xservoCenter;
int tilt=yservoCenter;
int panScale;
byte panscaleFactor=12;
byte panservoMultiplier=5;
int tiltScale;
byte tiltscaleFactor=12;
byte tiltservoMultiplier=5;
int x;
int xservoMax=160;
int xservoMin=0;
int xturnMax=150;
int xturnMin=10;
int y;
int yservoMin=0;
int yservoMax=140;

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

void left()
{
        analogWrite(leftFmotor, leftmotorSpeed);
        analogWrite(rightFmotor, rightmotorSpeed);
        digitalWrite(leftRmotor, HIGH);
        digitalWrite(rightRmotor, LOW);
}

void right()
{
        analogWrite(leftFmotor, leftmotorSpeed);
        analogWrite(rightFmotor, rightmotorSpeed);
        digitalWrite(leftRmotor, LOW);
        digitalWrite(rightRmotor, HIGH);
}

void infraFollow()
{
	if(irDistance < followDistance) {
		if(pan > xservoCenter) {
			pan=pan - 1;
		}
		if(pan < xservoCenter) {
			pan=pan + 1;
		}
		if(tilt > yservoCenter) {
			tilt=tilt - 1;
		}
		if(tilt < yservoCenter) {
			tilt=tilt + 1;
		}
	}
	else {
		panScale=(irleftValue + irrightValue) * panscaleFactor / 10;
		x=(irrightValue - irleftValue) * panservoMultiplier / panScale;
		pan=pan - x;
		tiltScale=(irupValue + irdownValue) * tiltscaleFactor / 10;
		y=(irupValue - irdownValue) * tiltservoMultiplier / tiltScale;
		tilt=tilt - y;
	} 
	if(pan > xservoMax) {
		pan=xservoMax;
	}
	if(pan < xservoMin) {
		pan=xservoMin;
	}
	if(tilt > yservoMax) {
		tilt=yservoMax;
	}
	if(tilt < yservoMin) {
		tilt=yservoMin;
	}
}

int ultrasensorDistance()
{
	int distance;
	unsigned long pulse=0;
	pinMode(ultrasensorPin, OUTPUT);	
	digitalWrite(ultrasensorPin, LOW);
	delayMicroseconds(5);
	digitalWrite(ultrasensorPin, HIGH);
	delayMicroseconds(5);
	digitalWrite(ultrasensorPin, LOW);
	pinMode(ultrasensorPin, INPUT);
	pulse=pulseIn(ultrasensorPin, HIGH);
	distance=(pulse / 2) / 29;
	return distance;
}

void turn()
{
	if(pan >= xturnMax) {
                left();
        }
        else if(pan <= xturnMin) {
                right();
        }
        else {
                stop();
        }
}

void move()
{
	if(pan == xservoCenter) {
                if(irDistance < minforwardDistance && irDistance > maxforwardDistance) {
                        forward();
                }
                else if(irDistance > maxbackwardDistance) {
                        backward();
                }
                else {
                        stop();
                }
        }
}

void loop()
{
	xservo.write(pan);
	yservo.write(tilt);
	getirDistance();
	infraFollow();
	turn();
	move();
}

