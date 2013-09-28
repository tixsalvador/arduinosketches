/*
091513	Configure Ultrasonic Sensor 
091613	Configure bot movement relative to encoder pair
091813	Added bot Uturn
092313	Added bot Left and Right Turn
092513	Remove reverse when crash is true; 
092713 	Added wheretoGo() -Bot will know where to go left or right
*/

#include <Servo.h>

#define distancesensorPin 3
#define leftFmotor 6 //M1 Speed Control 
#define rightFmotor 5 //M2 Speed Control
#define leftRmotor 8 //M1 Direction Control
#define rightRmotor 7 //M2 Direction Control
#define xservoPin 9
#define yservoPin 10

int leftmotorSpeed=255;
int rightmotorSpeed=255;
long count = 0;
int rawsensorData = 0;
int sensorData1= 0;
int sensorData0 = 0;
boolean movingForward=false;
boolean crash=false;
int crashzone=8; //Value in centimiters
int backstepCount=5;
int uturnCount=25; //Default is 35 Value; change to 25 because of backstepcount
int lturnCount=25;
int rturnCount=25;
int xservoCenter=1100;
int xservoLeft=1650;
int xservoRight=550;
int yservoCenter=1650;

Servo xservo;
Servo yservo;

void setup()
{
	Serial.begin(9600);
	pinMode(distancesensorPin, OUTPUT);
	for(int i=5; i<=8; i++) {
		pinMode(i, OUTPUT);
	}
	xservo.attach(xservoPin);
	yservo.attach(yservoPin);
}

void loop()
{
	centerServos();
	forward();		
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
	int backstep=((encoderCount()) - backstepCount);
	if((getDistance()) < crashzone) {
                crash=true;
        }
        if(crash == false) {
            analogWrite(leftFmotor, leftmotorSpeed);
			analogWrite(rightFmotor, rightmotorSpeed);
			digitalWrite(leftRmotor, LOW);
			digitalWrite(rightRmotor, LOW);
        }
        if(crash == true) {
			do {
				reverse();
			} while((encoderCount()) > backstep);
			stop();
			delay(2000);
			wheretoGo();
			crash=false;
        }	
        //Serial.println(encoderCount());
}
		
void reverse()
{
    analogWrite(leftFmotor, leftmotorSpeed);
    analogWrite(rightFmotor, rightmotorSpeed);
    digitalWrite(leftRmotor, HIGH);
    digitalWrite(rightRmotor, HIGH);
}

void left()
{
        int lturn=((encoderCount()) + lturnCount);
        do {
        	analogWrite(leftFmotor, leftmotorSpeed);
        	analogWrite(rightFmotor, rightmotorSpeed);
        	digitalWrite(leftRmotor, HIGH);
        	digitalWrite(rightRmotor, LOW);
        }
        while((encoderCount()) < lturn);
}

void right()
{
		int rturn=((encoderCount()) + rturnCount);
		do {
			analogWrite(leftFmotor, leftmotorSpeed);
    		analogWrite(rightFmotor, rightmotorSpeed);
    		digitalWrite(leftRmotor, LOW);
    		digitalWrite(rightRmotor, HIGH);
		}
		while((encoderCount()) < rturn);
}

void centerServos()
{
	xservo.writeMicroseconds(xservoCenter);
	yservo.writeMicroseconds(yservoCenter);
}

int encoderCount()
{
	rawsensorData=analogRead(0);
	if(rawsensorData < 600 && (digitalRead(leftRmotor) == LOW || digitalRead(rightRmotor) == LOW)) {
            movingForward=true;
            sensorData1 = 1;
    }
    else if(rawsensorData < 600 && digitalRead(leftRmotor) == HIGH && digitalRead(rightRmotor) == HIGH) {
            movingForward=false;
            sensorData1 = 1;
    }
    else {
    		sensorData1 = 0;
    }
    
    if(sensorData1 != sensorData0 && movingForward == true) {
            count ++;
    }
    else if (sensorData1 != sensorData0 && movingForward == false) {
      		count --;
    }
    sensorData0 = sensorData1;
    return count;
}

int getDistance()
{
        int distance;
        unsigned long pulse =0;

        pinMode(distancesensorPin, OUTPUT);
        digitalWrite(distancesensorPin, LOW);
        delayMicroseconds(5);

        digitalWrite(distancesensorPin, HIGH);
        delayMicroseconds(5);
        digitalWrite(distancesensorPin, LOW);

        pinMode(distancesensorPin, INPUT);
        pulse=pulseIn(distancesensorPin, HIGH);
        pulse=pulse/2;
        distance = int(pulse/29);
        return distance;
}

void wheretoGo()
{
	xservo.writeMicroseconds(xservoLeft);
	delay(1000);
	int leftdistanceValue=getDistance();
	xservo.writeMicroseconds(xservoRight);
	delay(1000);
	int rightdistanceValue=getDistance();
	centerServos();
	if(leftdistanceValue > rightdistanceValue) {
		left();
	}
	else {
		right();
	}
}
void leftDistance()
{
	xservo.writeMicroseconds(xservoLeft);
	delay(1000);
	int leftdistanceValue=getDistance();
	//return leftdistanceValue;
	Serial.print(leftdistanceValue);
}

void  rightDistance()
{
	xservo.writeMicroseconds(xservoRight);
	delay(1000);
	int rightdistanceValue=getDistance();
	//return rightdistanceValue;
	Serial.println(rightdistanceValue);
}
