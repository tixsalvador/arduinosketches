/*
10-01-13        Configure Compound Infrared Sensor
*/

#include <Servo.h>

#define infraUp A2
#define infraLeft A3
#define infraDown A0
#define infraRight A1
#define infraLeds 11
#define xservoPin 9
#define yservoPin 10

byte xscaleFactor=10;
byte yscaleFactor=10;
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
int maxDistance=200;
int pan=xservoCenter;
int tilt=yservoCenter;
int xscale;
int yscale;
int x;
int y;

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
				x=(infraLeftvalue - infraRightvalue) * 15 / xscale;
				pan=pan - x;
			}
			if(infraLeftvalue < infraRightvalue) {
				x=(infraRightvalue - infraLeftvalue) * 15 / xscale;
				pan=pan + x;
			}
			if(infraUpvalue > infraDownvalue) {
				y=(infraUpvalue - infraDownvalue) * 15 / yscale;
				tilt=tilt - x;
			}
			if(infraUpvalue < infraDownvalue) {
				y=(infraDownvalue - infraUpvalue) * 15 / yscale;
				tilt=tilt + x;
			}
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
	Serial.println(infraDownvalue);
	
}
	

