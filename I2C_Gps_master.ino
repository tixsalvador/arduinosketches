#include <Wire.h>

union lat2bytes
{
	float f;
	char b[sizeof(float)];
};

float latitude;

void setup()
{
	Serial.begin(9600);
	Wire.begin();
}

void loop()
{
	lat2bytes la2b;
	Wire.requestFrom(7,4);
	while(Wire.available())
	{
		la2b.b[0]=Wire.read();
		la2b.b[1]=Wire.read();
		la2b.b[2]=Wire.read();
		la2b.b[3]=Wire.read();
		latitude=la2b.f;
	}
	Serial.println(latitude,6);
}
