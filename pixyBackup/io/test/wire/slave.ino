#include <Wire.h>

byte dataSize;
int poten;

void setup()
{
	Wire.begin();
	Serial.begin(9600);
}

void loop()
{
	delay(50);
	potenValue();
	Serial.println(poten);
}

void potenValue()
{
	Wire.requestFrom(9,2);
	if(Wire.available() == 2){
		poten=Wire.read() <<8 | Wire.read();
	}
	else{
		Serial.println("error");
	}
	
}
