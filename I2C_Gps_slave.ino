#include <TinyGPS.h>
#include <SoftwareSerial.h>
#include <Wire.h>

SoftwareSerial sS(2,3);
TinyGPS gps;

float latitude,longitude;
int year;
byte month,day,hour,minutes,second,hundredths;
byte buffer[8];

void setup()
{
	Serial.begin(9600);
	sS.begin(9600);
	pinMode(6,OUTPUT);
	digitalWrite(6,LOW);
	Wire.begin(2);
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
		float latf;
		byte latb[sizeof(float)];
	};
	lat2bytes la2b;
	la2b.latf=latitude;
	buffer[0]=la2b.latb[0];
	buffer[1]=la2b.latb[1];
	buffer[2]=la2b.latb[2];
	buffer[3]=la2b.latb[3];
	
	union lon2bytes
	{
		float lonf;
		byte lonb[sizeof(float)];
	};
	lon2bytes lo2b;
	lo2b.lonf=longitude;
	buffer[4]=lo2b.lonb[0];
	buffer[5]=lo2b.lonb[1];
	buffer[6]=lo2b.lonb[2];
	buffer[7]=lo2b.lonb[3];
	
	Wire.write(buffer,8);
}

void loop()
{
	gps_Data();
}
