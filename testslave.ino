#include<Wire.h>

void setup()
{
	Wire.begin(2);
	Wire.onRequest(lightLevel);
}

void lightLevel()
{
	int lightSensor=analogRead(A0);
	Wire.write(lightSensor);
}

void loop()
{}
