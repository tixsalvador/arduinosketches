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
int tiltservoCenter=30;
int yservoMin=0;
int yservoMax=160;
int leftmotorStop=94;
int rightmotorStop=96;

int irUpvalue;
int irLeftvalue;
int irDownvalue;
int irRightvalue;
int irDistance;

int distanceMax=250;
int bestDistance=600;

int leftmotorSpeed=leftmotorStop;
int rightmotorSpeed=rightmotorStop;
int pan=panservoCenter;
int panScale;
byte panscaleFactor=3;
byte panservoMultiplier=10;
int x;

int tilt=tiltservoCenter;
int tiltScale;
byte tiltscaleFactor=3;
byte tiltservoMultiplier=10;
int y;
 
int move;
int leftmotorMove=leftmotorStop;
int rightmotorMove=rightmotorStop;
int motorSpeed=20;


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
	leftMotor.write(leftmotorStop);
	rightMotor.attach(rightmotorPin);
	rightMotor.write(rightmotorStop);
	pinMode(irledPin, OUTPUT);
}

void loop()
{ 
	leftMotor.write(leftmotorMove);
	rightMotor.write(rightmotorMove);
	panServo.write(pan);	
	tiltServo.write(tilt);
	infraDistance();
	infraFollow();
	Serial.println(pan);
}

void reAttach()
{
	if(!leftMotor.attached()) 
		leftMotor.attach(leftmotorPin);	
		
	if(!rightMotor.attached()) 
		rightMotor.attach(rightmotorPin);
}
	
	
void stop()
{
	leftMotor.detach();
	rightMotor.detach();
}

void forward()
{
	reAttach();
	leftmotorMove=leftmotorStop - 30;
	rightmotorMove=rightmotorStop + 30;
}

void backward()
{
	reAttach();
        leftmotorMove=leftmotorStop + 30;
        rightmotorMove=rightmotorStop - 30;
}

void left()
{
	reAttach();
	leftmotorMove=leftmotorStop + motorSpeed;
        rightmotorMove=rightmotorStop + motorSpeed;
}

void right()
{
        reAttach();
        leftmotorMove=leftmotorStop - motorSpeed;
        rightmotorMove=rightmotorStop - motorSpeed;
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
		tiltScale=(irUpvalue + irDownvalue) * tiltscaleFactor / 10;
		if(irLeftvalue < irRightvalue) {
			x=(irLeftvalue - irRightvalue) * panservoMultiplier / panScale;
			pan=pan - x;
		}
		if(irLeftvalue > irRightvalue) {
			x=(irRightvalue - irLeftvalue) * panservoMultiplier / panScale;
			pan=pan +x;
		} 
		if(irUpvalue > irDownvalue) {
			y=(irDownvalue - irUpvalue) * tiltservoMultiplier / tiltScale;
			tilt= tilt - y;
		}
		if(irUpvalue < irDownvalue) {
			y=(irUpvalue - irDownvalue) * tiltservoMultiplier / tiltScale;
			tilt= tilt + y;
		}
	}
	if(pan < xservoMin) {
		pan=xservoMin;
	}
	if(pan > xservoMax) {
		pan=xservoMax;
	}
	if(tilt < yservoMin) {
		tilt=yservoMin;
	}
	if(tilt > yservoMax) {
		tilt=yservoMax;
	}
	
	if(pan == 180) {
		right();
	}
	else if(pan == 0) {
		left();
	}
	else {
		stop();
	}
		
}
