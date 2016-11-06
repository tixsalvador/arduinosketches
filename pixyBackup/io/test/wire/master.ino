#include <Wire.h>

int poten;

void setup()
{
	Serial.begin(9600);
	Wire.begin(9);
	Wire.onRequest(sendData);
}

void loop()
{
	poten=analogRead(A0);
	delay(100);
	Serial.println(poten);
}

void sendData()
{
	byte buffer[2];
	buffer[0]=poten >> 8;
	buffer[1]=poten & 0xFF;
	Wire.write(buffer,2);
}
