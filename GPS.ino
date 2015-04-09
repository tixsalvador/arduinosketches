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
			int year;
			byte month,day,hour,minutes,second,hundredths;
			gps.crack_datetime(&year,&month,&day,&hour,&minutes,&second,&hundredths);

			gps.f_get_position(&latitude,&longitude);
			Serial.print(latitude,8);
                        Serial.print("\t");
                        Serial.print(longitude,8);
                        Serial.print("\t");
			Serial.print(year);
			Serial.print("\t");
			Serial.print(month);
			Serial.print("\t");
			Serial.print(day);
                        Serial.print("\t");
                        Serial.print(hour);
			Serial.print("\t");
                        Serial.print(minutes);
                        Serial.print("\t");
                        Serial.println(second);
		}
	}
}
