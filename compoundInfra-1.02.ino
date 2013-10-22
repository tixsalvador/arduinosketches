/*
10-01-13        Configure Compound Infrared Sensor
10-9-13			Compound Eye calibration using multipliers
*/

#include <Servo.h>

#define infraUp A2
#define infraLeft A3
#define infraDown A0
#define infraRight A1
#define xservoPin 9
#define yservoPin 10
#define infraLeds 11
#define leftFmotor 6
#define rightFmotor 5 
#define leftRmotor 8 
#define rightRmotor 7 

byte xscaleFactor=5;
byte yscaleFactor=5;
byte servoMultiplier=10;
int infraUpvalue;
int infraLeftvalue;
int infraDownvalue;
int infraRightvalue;
int xservoCenter=50;
int xservoLeft=100;
int xservoRight=0;
int yservoCenter=110;
int yservoUp=10;
int yservoDown=170;
int distance;
int maxDistance=180;
int pan=xservoCenter;
int tilt=yservoCenter;
int xscale;
int yscale;
int x;
int y;
int turn;
int leftmotorSpeed=255;
int rightmotorSpeed=255;

Servo xservo;
Servo yservo;

void setup()
{
        Serial.begin(9600);
        pinMode(infraLeds, OUTPUT);
        xservo.attach(xservoPin);
        xservo.write(xservoCenter);
        yservo.attach(yservoPin);
        yservo.write(yservoCenter);
        digitalWrite(leftFmotor, LOW);
    	digitalWrite(rightFmotor, LOW);
    	digitalWrite(leftRmotor, LOW);
    	digitalWrite(rightRmotor, LOW);
}

void infraRead()
{
        digitalWrite(infraLeds, HIGH);
        delay(50);
        infraUpvalue=analogRead(infraUp);
        infraLeftvalue=analogRead(infraLeft);
        infraDownvalue=analogRead(infraDown);
        infraRightvalue=analogRead(infraRight);
        digitalWrite(infraLeds, LOW);
        delay(50);
        infraUpvalue=infraUpvalue - analogRead(infraUp);
        infraLeftvalue=infraLeftvalue - analogRead(infraLeft);
        infraDownvalue=infraDownvalue - analogRead(infraDown);
        infraRightvalue=infraRightvalue - analogRead(infraRight);
        distance=(infraUpvalue + infraLeftvalue + infraDownvalue + infraRightvalue) / 4;
}

void infraFollow()
{
        if(distance < maxDistance) {
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
                        xscale=(infraLeftvalue + infraRightvalue) * xscaleFactor / 10;
                        yscale=(infraUpvalue + infraDownvalue) * yscaleFactor / 10;
                        if(infraLeftvalue > infraRightvalue) {
                                x=(infraLeftvalue - infraRightvalue) * servoMultiplier / xscale;
                                pan=pan - x;
                        }
                        if(infraLeftvalue < infraRightvalue) {
                                x=(infraRightvalue - infraLeftvalue) * servoMultiplier / xscale;
                                pan=pan + x;
                        }
                        if(infraUpvalue > infraDownvalue) {
                                y=(infraUpvalue - infraDownvalue) * servoMultiplier / yscale;
                                tilt=tilt - x;
                        }
                        if(infraUpvalue < infraDownvalue) {
                                y=(infraDownvalue - infraUpvalue) * servoMultiplier / yscale;
                                tilt=tilt + x;
                        }
        	}
	if(pan < xservoRight) {
			pan=xservoRight;
	}
	if(pan > xservoLeft) {
			pan=xservoLeft;
	}
	if(tilt < yservoUp) {
			tilt=yservoUp;
	}
	if(tilt > yservoDown) {
			tilt=yservoDown;
	}
	turn=xservoLeft - pan;
	if(turn < 20) {
		
               	analogWrite(leftFmotor, leftmotorSpeed);
               	analogWrite(rightFmotor, rightmotorSpeed);
               	digitalWrite(leftRmotor, HIGH);
               	digitalWrite(rightRmotor, LOW);
	}
	if(turn > 80) {
		analogWrite(leftFmotor, leftmotorSpeed);
                analogWrite(rightFmotor, rightmotorSpeed);
                digitalWrite(leftRmotor, LOW);
                digitalWrite(rightRmotor, HIGH);
	}
}

void loop()
{
        xservo.write(pan);
        yservo.write(tilt);
        infraFollow();
        infraRead();
        Serial.print(distance);
        Serial.print(" ");
        Serial.print(pan);
        Serial.print(" ");
        Serial.print(tilt);
        Serial.print(" ");
        Serial.print(infraLeftvalue);
        Serial.print(" ");
        Serial.print(infraRightvalue);
        Serial.print(" ");
        Serial.print(infraUpvalue);
        Serial.print(" ");
        Serial.print(infraDownvalue);
		Serial.print(" ");
		Serial.println(turn);
}
