#include <TinyGPS.h>
#include <SoftwareSerial.h>

SoftwareSerial sS(2,3);
TinyGPS gps;

void setup()
{
	Serial.begin(9600);
	sS.begin(9600);
	pinMode(6,OUTPUT);
	digitalWrite(6,LOW);
}

void loop()
{
	byte a;
	if(sS.available()>0){
		a=sS.read();
		if(gps.encode(a)){
			float latitude,longitude;
			gps.f_get_position(&latitude,&longitude);
			Serial.print(latitude,5);
			Serial.print("\t");
			Serial.println(longitude,5);
		}
	}
}
