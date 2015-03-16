//I2C slave

#include<Wire.h>

void setup()
{
	Wire.begin(5);
	Wire.onRequest(requestValue);
}

void loop()
{
	delay(100);
}

void requestValue()
{
	int sensorValue=analogRead(A0);
	int turnValue=analogRead(A1);
	byte buffer[4];
	buffer[0]=highByte(sensorValue);
	buffer[1]=lowByte(sensorValue);
	buffer[2]=highByte(turnValue);
	buffer[3]=lowByte(turnValue);
	Wire.write(buffer,4);
}
