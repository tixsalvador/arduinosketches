//TREX SLAVE

#include<EEPROM.h>
#include<Wire.h>

byte I2Caddress;
int Xaxis;
int Yaxis;
int Zaxis;
int voltage;

void setup()
{
	Serial.begin(9600);
	check_I2C_address();
	Wire.begin(I2Caddress);
	Wire.onRequest(send_Sensor_Data);
}

void check_I2C_address()
{
	byte n=EEPROM.read(0);
	if(n==0x55){
		I2Caddress=EEPROM.read(1);
	}
	else {
		EEPROM.write(0,0x55);
		EEPROM.write(1,0x07);
		I2Caddress=EEPROM.read(1);
	}
}

void check_Trex_Sensors()
{
	Xaxis=analogRead(A0);
	Yaxis=analogRead(A1);
	Zaxis=analogRead(A2);
	voltage=analogRead(A3);
}

void send_Sensor_Data()
{
	byte buffer[6];
	buffer[0]=Xaxis >> 8;
	buffer[1]=Xaxis & 0xFF;
	buffer[2]=Yaxis >> 8;
	buffer[3]=Yaxis & 0xFF;
	buffer[4]=Zaxis >> 8;
	buffer[5]=Zaxis & 0xFF;
	Wire.write(buffer,6);
}

void loop()
{
	delay(100);
	check_Trex_Sensors();	
}
