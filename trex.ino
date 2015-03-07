#define batteryPin A3;

#define leftMotorPin 2
#define leftBreakPin 4
#define leftSpeedPin 3
#define leftCurrentPin A6

#define rightMotorPin 10
#define rightBreakPin 9
#define	rightSpeedPin 11
#define rightCurrentPin A7

int motorSpeed=100;

int leftMotorCurrent;
int rightMotorCurrent;

void setup()
{
	Serial.begin(9600);
	int motorPins[]={2,3,4,9,10,11};
	for(int i=0;i<6;i++){
		pinMode(motorPins[i], OUTPUT);
	}
}

void reverse()
{	
	digitalWrite(rightBreakPin,LOW);
	analogWrite(rightSpeedPin, motorSpeed);
	digitalWrite(rightMotorPin,LOW);
	digitalWrite(leftBreakPin, LOW);
	analogWrite(leftSpeedPin, motorSpeed);
	digitalWrite(leftMotorPin,LOW);
}

float batVoltReading()
{
	int batVolt=analogRead(batteryPin);
	batVolt=(batVolt/33.57);
	return batVolt;	
}

void loop()
{
	reverse();
	
	leftMotorCurrent=analogRead(leftCurrentPin);
        rightMotorCurrent=analogRead(rightCurrentPin);
        Serial.print(leftMotorCurrent);
        Serial.print("  ");
        Serial.println(rightMotorCurrent);
}
