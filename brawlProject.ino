/* */

#include <Servo.h>

// Motor pins
#define leftFmotor 6
#define rightFmotor 5 
#define leftRmotor 8 
#define rightRmotor 7 

// Servo pins
#define xservoPin 9
#define yservoPin 10

// Ultra Sensor pins
#define ultrasensorPin 3

// Compound Infra Sensor pins
#define infraLeds 11
#define infraUp A4
#define infraLeft A5
#define infraDown A2
#define infraRight A3

// Motor variables
int leftmotorSpeed = 200;
int rightmotorSpeed = 200;

// Servo variables
Servo xservo;
Servo yservo;
int xservoCenter=50;
int xservoLeft=100;
int xservoRight=0;
int yservoCenter=110;
int yservoUp=10;
int yservoDown=170;

//Check left variable
int leftultradistanceValue;
int leftcompounddistanceValue;
int yleftcompounddistanceValue;

//Check right variable
int rightultradistanceValue;
int rightcompounddistanceValue;
int yrightcompounddistanceValue;

void setup()
{
	Serial.begin(9600);
	for(int i=5; i <= 8; i++) {
		pinMode(i, OUTPUT);
	}
	xservo.attach(xservoPin);
	xservo.write(xservoCenter);
	yservo.attach(yservoPin);
	yservo.write(yservoCenter);
}

void getMoving() {
	if((ultraDistance() < 10) ||  (compoundDistance() > 300)) {
		stop();
		checkLeft();
		checkRight();
	}
	else {
		forward();
	}
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
	xservo.write(xservoCenter);
	yservo.write(yservoCenter);
	analogWrite(leftFmotor, leftmotorSpeed);
	analogWrite(rightFmotor, rightmotorSpeed);
	digitalWrite(leftRmotor, LOW);
	digitalWrite(rightRmotor, LOW);
}

void left()
{
	const int lturnCount=30;
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
	const int rturnCount=30;
        int rturn=((encoderCount()) + rturnCount);
        do {
                analogWrite(leftFmotor, leftmotorSpeed);
                analogWrite(rightFmotor, rightmotorSpeed);
                digitalWrite(leftRmotor, LOW);
                digitalWrite(rightRmotor, HIGH);
        }
        while((encoderCount()) < rturn);
}

void checkLeft()
{
	xservo.write(xservoLeft);
	yservo.write(yservoCenter);
	delay(500);
	leftultradistanceValue=ultraDistance();
	leftcompounddistanceValue=compoundDistance();
	yservo.write(yservoUp);
	delay(500);
	yleftcompounddistanceValue=compoundDistance();	
}

void checkRight()
{
        xservo.write(xservoRight);
        yservo.write(yservoCenter);
        delay(500);
        rightultradistanceValue=ultraDistance();
        rightcompounddistanceValue=compoundDistance();
        yservo.write(yservoUp);
        delay(500);
        yrightcompounddistanceValue=compoundDistance();
}

int encoderCount()
{
    int sensorData1= 0;
    int sensorData0 = 0;
    int rawsensorData;
    int count;
    boolean movingForward;
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

int ultraDistance()
{
        int ultrasensorDistance;
        unsigned long pulse =0;

        pinMode(ultrasensorPin, OUTPUT);
        digitalWrite(ultrasensorPin, LOW);
        delayMicroseconds(5);

        digitalWrite(ultrasensorPin, HIGH);
        delayMicroseconds(5);
        digitalWrite(ultrasensorPin, LOW);

        pinMode(ultrasensorPin, INPUT);
        pulse=pulseIn(ultrasensorPin, HIGH);
        pulse=pulse/2;
        ultrasensorDistance = int(pulse/29);
        return ultrasensorDistance;
}

int compoundDistance()
{
	int infraUpvalue;
	int infraLeftvalue;
	int infraDownvalue;
	int infraRightvalue;
	int compoundinfraDistance;
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
        compoundinfraDistance=(infraUpvalue + infraLeftvalue + infraDownvalue + infraRightvalue) / 4;
	return compoundinfraDistance;
}

void loop()
{
	
	getMoving();
	Serial.print(ultraDistance());
	Serial.print("\t");
	Serial.println(compoundDistance());
}
