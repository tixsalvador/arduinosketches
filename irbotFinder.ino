int motorforwardPin1=5;
int motorforwardPin2=6;
int motorbackwardPin1=7;
int motorbackwardPin2=8;

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
	forward();
	Serial.println(encoderCount());
}

int encoderCount()
{
	int encoderPin=A0;
	int encoderData=analogRead(encoderPin);
	int encoderValue1=0;
	int encoderValue2=0;
	int count=0;
	if(encoderData < 600){
		encoderValue1=1;
	}
	else {
		encoderValue1=0;
	}
	if(encoderValue1 != encoderValue2){
		count ++;
	}
	encoderValue2=encoderValue1;
	return count;
}

void forward()
{
	analogWrite(5,255);
        analogWrite(6,255);
        digitalWrite(7,LOW);
        digitalWrite(8,LOW);

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
