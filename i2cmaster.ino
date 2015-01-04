#include <Wire.h>

void setup()
{
	Wire.begin();
}

void loop()
{
	Wire.beginTransmission(10);
	Wire.write('H');
	Wire.endTransmission();
	delay(5000);

	Wire.beginTransmission(10);
	Wire.write('L');
	Wire.endTransmission();
	delay(5000);
}
