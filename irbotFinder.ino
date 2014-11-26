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
	pinMode(5, OUTPUT);
	pinMode(6, OUTPUT);
	pinMode(7, OUTPUT);
	pinMode(8, OUTPUT);
}

void loop()
{
	int north=analogRead(northPin);
	int east=analogRead(eastPin);
	int south=analogRead(southPin);
	int west=analogRead(westPin);
	Serial.print(north);
	Serial.print("	");
	Serial.print(east);
	Serial.print("	");
	Serial.print(south);
	Serial.print("	");
	Serial.println(west);
	delay(1000);

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
