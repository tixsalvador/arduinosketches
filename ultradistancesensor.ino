int distancesensorPin = 3;

void setup()
{
	Serial.begin(9600);
	pinMode(distancesensorPin, OUTPUT);
}

int getDistance()
{
	int distance;
	unsigned long pulse =0;
	
	pinMode(distancesensorPin, OUTPUT);
	digitalWrite(distancesensorPin, LOW);
	delayMicroseconds(5);

	digitalWrite(distancesensorPin, HIGH);
	delayMicroseconds(5);
	digitalWrite(distancesensorPin, LOW);

	pinMode(distancesensorPin, INPUT);
	pulse=pulseIn(distancesensorPin, HIGH);
	pulse=pulse/2;
	distance = int(pulse/29);
	return distance;	
}

void loop()
{
	Serial.println(getDistance());
}
