int northPin=A5;
int eastPin=A4;
int southPin=A3;
int westPin=A2;

int motorforwardPin1=5;
int motorforwardPin2=6;
int motorbackwardPin1=7;
int motorbackwardPin2=8;
unsigned int count=0;

void setup()
{
	Serial.begin(9600);
	pinMode(A2, INPUT);
	pinMode(A3, INPUT);
	pinMode(A4, INPUT);
	pinMode(A5, INPUT);
	pinMode(5, OUTPUT);
	pinMode(6, OUTPUT);
	pinMode(7, OUTPUT);
	pinMode(8, OUTPUT);

}

void loop()
{
	struct sensorData {
		int northData;
		int eastData;
		int westData;
		int southData;
	};
	int northReading=analogRead(northPin);
	int eastReading=analogRead(eastPin);
	int southReading=analogRead(southPin);
	int westReading=analogRead(westPin);
	struct sensorData sensorReading[40];
	for(int i=0;i<=9;i++){
		sensorReading[i].northData=northReading;
		sensorReading[i].eastData=eastReading;
		sensorReading[i].westData=westReading;
		sensorReading[i].southData=southReading;
		delay(300);
		if(sensorReading[i].northData < 100){
			Serial.println("North");
		}
		else if(sensorReading[i].eastData < 100){
			Serial.println("East");
		}
		else if(sensorReading[i].westData < 100){
			Serial.println("West");
		}
		else if(sensorReading[i].southData < 100){
			Serial.println("South");
		}
		else
			Serial.println("I'm LOST");
	}

	
}

int encoderCount()
{
	int encoderData1=analogRead(A0);
	int encoderData2=analogRead(A1);
	int motorBreak1=digitalRead(motorbackwardPin1);
	int motorBreak2=digitalRead(motorbackwardPin2);
	int encoderValue1=0;
	int encoderValue2=0;
	boolean movingForward=false;

	if((encoderData1 < 500) && (encoderData2 < 500) && ((motorBreak1 == LOW) || (motorBreak2 == LOW))){
		encoderValue1=1;
		movingForward=true;
	}
	else {
		encoderValue1=0;
	}
	if((encoderValue1 != encoderValue2) && (movingForward == true)){
		count ++;
	}
	encoderValue2=encoderValue1;
	return count;
}

void forward()
{
	analogWrite(5,255);
        analogWrite(6,255);
        digitalWrite(motorbackwardPin1,LOW);
        digitalWrite(motorbackwardPin2,LOW);

}

void reverse()
{
	analogWrite(5,255);
        analogWrite(6,255);
	digitalWrite(7,HIGH);
        digitalWrite(8,HIGH);
}

void left()
{
	 analogWrite(5,255);
        analogWrite(6,255);
	digitalWrite(7,LOW);
        digitalWrite(8,HIGH);
}

void right()
{
        analogWrite(5,100);
        analogWrite(6,255);
        digitalWrite(7,LOW);
        digitalWrite(8,LOW);
}

void stop()
{
	digitalWrite(5,LOW);
        digitalWrite(6,LOW);
	digitalWrite(7,LOW);
        digitalWrite(8,LOW);
}
