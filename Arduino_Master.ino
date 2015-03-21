//Arduino MASTER

#include<Wire.h>

byte dataAvailable;
int Xaxis;
int Yaxis;
int Zaxis;
float voltage;

void setup()
{
	Serial.begin(9600);
	Wire.begin();
}

void trex_Sensor_Values()
{
	dataAvailable=Wire.requestFrom(0x07,6);
	if(dataAvailable==6){
		Xaxis=Wire.read()<<8|Wire.read();
		Yaxis=Wire.read()<<8|Wire.read();
		Zaxis=Wire.read()<<8|Wire.read();
	//	voltage=Wire.read()<<8|Wire.read();
	//	voltage=voltage/33.57;
		Serial.print(Xaxis);
		Serial.print("\t");
		Serial.print(Yaxis);
		Serial.print("\t");
		Serial.println(Zaxis);
	}
	else {
		Serial.println("ERROR");
	}
}

void loop()
{
	delay(100);
	trex_Sensor_Values();
}
