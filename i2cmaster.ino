//I2C Master

#include<Wire.h>

void setup()
{
	Serial.begin(9600);
	Wire.begin();
}

void loop()
{
	int receivedValue;
	int receivedValue2;
	int available=Wire.requestFrom(5,(byte)4);
	if(available==4){
		receivedValue=Wire.read()<<8|Wire.read();
		receivedValue2=Wire.read()<<8|Wire.read();
		Serial.println(receivedValue);
		Serial.print("\t");
		Serial.println(receivedValue2);
	}
	else {
		Serial.print("Error:Unexpected bytes");
		Serial.println(available);
	}
}
