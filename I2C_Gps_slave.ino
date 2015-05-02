#include <TinyGPS.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <SD.h>

SoftwareSerial sS(2,3);
TinyGPS gps;

File myFile;

float latitude,longitude,altitude,course,speed_mps;
unsigned long time,date;

byte buffer[8];

void setup()
{
	Serial.begin(9600);
	sS.begin(9600);
	pinMode(6,OUTPUT);
	pinMode(10,OUTPUT);
	digitalWrite(6,LOW);
	check_SD_Card();
	Wire.begin(2);
	Wire.onRequest(get_GPS_Data);
}

void check_SD_Card()
{
	if(!SD.begin(10)){
		Serial.println("Failed to initialize card");
		return;
	}
	if(!SD.exists("gpsLog.txt")){
		myFile=SD.open("gpsLog.txt",FILE_WRITE);
		myFile.close();
		Serial.println("Created new file");
	}
}


void gps_Data()
{
	byte a;
	if(sS.available()>0){
		a=sS.read();
		if(gps.encode(a)){
			gps.get_datetime(&date,&time);
			gps.f_get_position(&latitude,&longitude);
			altitude=gps.f_altitude();
			course=gps.f_course();
			myFile=SD.open("gpsLog.txt",FILE_WRITE);
			if(myFile){
				myFile.print(date);
				myFile.print(", ");
				myFile.print(time);
				myFile.print(", ");
				myFile.print(latitude,8);
				myFile.print(", ");
				myFile.print(longitude,8);
				myFile.print(", ");
				myFile.print(altitude);
				myFile.print(", ");
				myFile.print(course);
				myFile.print(", ");
				myFile.println(speed_mps);
				myFile.close();
			}
			Serial.print(date);
			Serial.print("\t");
			Serial.print(time);
			Serial.print("\t");
			Serial.print(latitude,8);
            Serial.print("\t");
            Serial.print(longitude,8);
            Serial.print("\t");
            Serial.print(altitude);
            Serial.print("\t");
            Serial.print(course);
            Serial.print("\t");
            Serial.println(speed_mps);
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

