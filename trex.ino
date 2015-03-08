#define batteryPin A3

#define leftMotorPin 2
#define leftBreakPin 4
#define leftSpeedPin 3
#define leftCurrentPin A6

#define rightMotorPin 10
#define rightBreakPin 9
#define	rightSpeedPin 11
#define rightCurrentPin A7

int leftMotorSpeed=100;
int rightMotorSpeed=100;

int leftMotorCurrent;
int rightMotorCurrent;

int lowVoltReading=6;

void setup()
{
	Serial.begin(9600);
	int motorPins[]={2,3,4,9,10,11};
	for(int i=0;i<6;i++){
		pinMode(motorPins[i], OUTPUT);
	}
}

void shutOff()
{
	digitalWrite(leftBreakPin, HIGH);
	digitalWrite(rightBreakPin,HIGH);
	analogWrite(leftSpeedPin, 0);
	analogWrite(rightSpeedPin, 0);
}

void reverse()
{	
	checkVolt();
	digitalWrite(rightBreakPin,LOW);
	analogWrite(rightSpeedPin, rightMotorSpeed);
	digitalWrite(rightMotorPin,LOW);
	digitalWrite(leftBreakPin, LOW);
	analogWrite(leftSpeedPin, leftMotorSpeed);
	digitalWrite(leftMotorPin,LOW);
}

float voltReading()
{
	float voltRead=analogRead(batteryPin);
	voltRead=voltRead/33.57;
	return voltRead;
}

void checkVolt()
{
	if(voltReading()<lowVoltReading){
		shutOff();
	}
}

void loop()
{
		return;
		reverse();
		leftMotorCurrent=analogRead(leftCurrentPin);
        	rightMotorCurrent=analogRead(rightCurrentPin);
        	Serial.print(leftMotorCurrent);
        	Serial.print("  ");
        	Serial.println(rightMotorCurrent);
}
