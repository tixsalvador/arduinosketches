#define irledPin 11

int irupValue;
int irdownValue;

void setup()
{
	Serial.begin(9600);
	pinMode(irledPin, OUTPUT);
}

void loop()
{
	irupValue=irDistance(A4);
	irdownValue=irDistance(A2);
	Serial.print(irupValue);
	Serial.print(" ");
	Serial.println(irdownValue);
}

int irDistance(int irPin)
{
	digitalWrite(irledPin, HIGH);
	delayMicroseconds(100);
	int irpinDistance=analogRead(irPin);
	digitalWrite(irledPin, LOW);
	delayMicroseconds(100);
	irpinDistance=irpinDistance - analogRead(irPin);
	return irpinDistance;
}
