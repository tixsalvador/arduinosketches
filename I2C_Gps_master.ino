#include <Wire.h>

union lat2bytes
{
	float latf;
	char latb[sizeof(float)];
};

union lon2bytes
{
	float lonf;
	char lonb[sizeof(float)];
};

float latitude;
float longitude;

void setup()
{
	Serial.begin(9600);
	Wire.begin();
}

void loop()
{
	lat2bytes la2b;
	lon2bytes lo2b;
	Wire.requestFrom(2,8);
	while(Wire.available())
	{
		la2b.latb[0]=Wire.read();
		la2b.latb[1]=Wire.read();
		la2b.latb[2]=Wire.read();
		la2b.latb[3]=Wire.read();
		latitude=la2b.latf;
		lo2b.lonb[0]=Wire.read();
		lo2b.lonb[1]=Wire.read();
		lo2b.lonb[2]=Wire.read();
		lo2b.lonb[3]=Wire.read();
		longitude=lo2b.lonf;
	}
	Serial.print(latitude,6);
	Serial.print("\t");
	Serial.println(longitude,6);
}
