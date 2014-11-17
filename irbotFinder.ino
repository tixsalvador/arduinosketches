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
	left();
	delay(2000);
	right();
	delay(2000);
	stop();
	delay(5000);
/*
	forward();
	delay(5000);
	reverse();
	delay(5000);	
*/
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
        analogWrite(5,255);
        analogWrite(6,255);
        digitalWrite(7,HIGH);
        digitalWrite(8,LOW);
}

void stop()
{
	digitalWrite(5,LOW);
        digitalWrite(6,LOW);
	digitalWrite(7,LOW);
        digitalWrite(8,LOW);
}
