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

#define led1 5
#define led2 6
#define led4 8
#define ledsenPin1 12
#define ledsenPin2 11
#define ledsenPin4 9

int panservoCenter=90;
int xservoMin=0;
int xservoMax=180;
int xservoturnMax=130;
int xservoturnMin=50;
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

int distanceMin=100;
int distanceMax=200;
int bestDistance=250;
int reverseDistance=550;

int leftmotorSpeed=leftmotorStop;
int rightmotorSpeed=rightmotorStop;
int pan=panservoCenter;
int panScale;
byte panscaleFactor=12;
byte panservoMultiplier=8;
int x;

int tilt=tiltservoCenter;
int tiltScale;
byte tiltscaleFactor=12;
byte tiltservoMultiplier=8;
int y;
 
int move;
int leftmotorMove=leftmotorStop;
int rightmotorMove=rightmotorStop;
int motorSpeed=20;

int irSen1;
int irSen2;
int irSen4;
byte lightChase;
long time;

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
	pinMode(led1, OUTPUT);
	pinMode(led2, OUTPUT);
	pinMode(led4, OUTPUT);
}

void loop()
{ 
	if(millis() - time > 199) {
		time=millis();
		lightChase=lightChase + 1 - 4 * (lightChase > 3);
	}
	
	leftMotor.write(leftmotorMove);
	rightMotor.write(rightmotorMove);
	panServo.write(pan);	
	tiltServo.write(tilt);
	infraDistance();
	infraFollow();
	edgeDetect();
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
	if(irDistance < bestDistance) {
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
	
	if(pan >= xservoturnMax) {
		right();
	}
	else if(pan <= xservoturnMin) {
		left();
	}
	else {
		stop();
	}
	
	if(pan < panservoCenter + 10 && pan > panservoCenter - 10) {
			if(irDistance  < distanceMax && irDistance > distanceMin) {
				forward();
			}
			else if(irDistance > reverseDistance) {
				backward();
			}
			else {
				stop();
			}
	}
}

void edgeDetect()
{
	digitalWrite(led1, HIGH);
	digitalWrite(led2, HIGH);
	digitalWrite(led4, HIGH);
	delayMicroseconds(100);

	irSen1=digitalRead(ledsenPin1);
	irSen2=digitalRead(ledsenPin2);
	irSen4=digitalRead(ledsenPin4);

	digitalWrite(led1, LOW);
	digitalWrite(led2, LOW);
	digitalWrite(led4, LOW);
	delayMicroseconds(100);
	
	digitalWrite(led1, (lightChase == 1 || irSen1 == 0));
	digitalWrite(led2, (lightChase == 2 || irSen2 == 0));
	digitalWrite(led4, (lightChase == 4 || irSen4 == 0));
	
	if(irSen1 == 0 && (leftmotorMove != leftmotorStop || rightmotorMove != rightmotorStop)) {
		stop();
	}
	
	if(irSen2 == 0 && (leftmotorMove != leftmotorStop || rightmotorMove != rightmotorStop)) {
                stop();
        }

	if(irSen4 == 0 && (leftmotorMove != leftmotorStop || rightmotorMove != rightmotorStop)) {
                stop();
        }
	
} 
