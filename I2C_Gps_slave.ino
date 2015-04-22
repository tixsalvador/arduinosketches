#include <TinyGPS.h>
#include <SoftwareSerial.h>
#include <Wire.h>

SoftwareSerial sS(2,3);
TinyGPS gps;

float latitude,longitude;
int year;
byte month,day,hour,minutes,second,hundredths;
byte buffer[4];

void setup()
{
	Serial.begin(9600);
	sS.begin(9600);
	pinMode(6,OUTPUT);
	digitalWrite(6,LOW);
	Wire.begin(7);
	Wire.onRequest(get_GPS_Data);
}

void gps_Data()
{
	byte a;
	if(sS.available()>0){
		a=sS.read();
		if(gps.encode(a)){
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

void get_GPS_Data()
{
	union lat2bytes
	{
		float f;
		byte b[sizeof(float)];
	};
	lat2bytes la2b;
	la2b.f=latitude;
	buffer[0]=la2b.b[0];
	buffer[1]=la2b.b[1];
	buffer[2]=la2b.b[2];
	buffer[3]=la2b.b[3];
	Wire.write(buffer,4);
}

void loop()
{
	gps_Data();
}
