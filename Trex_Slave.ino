//TREX SLAVE

#include<EEPROM.h>
#include<Wire.h>

byte I2Caddress;
int Xaxis;
int Yaxis;
int Zaxis;
int voltage;
int leftMotorCurrent;
int rightMotorCurrent;

void setup()
{
	Serial.begin(9600);
	check_I2C_address();
	Wire.begin(I2Caddress);
	Wire.onRequest(send_Sensor_Data);
	Wire.onReceive(receive_Data);
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
	leftMotorCurrent=analogRead(A6);
	rightMotorCurrent=analogRead(A7);
}

void send_Sensor_Data()
{
	byte buffer[12];
	buffer[0]=Xaxis >> 8;
	buffer[1]=Xaxis & 0xFF;
	buffer[2]=Yaxis >> 8;
	buffer[3]=Yaxis & 0xFF;
	buffer[4]=Zaxis >> 8;
	buffer[5]=Zaxis & 0xFF;
	buffer[6]=voltage >> 8;
	buffer[7]=voltage & 0xFF;
	buffer[8]=leftMotorCurrent >> 8;
	buffer[9]=leftMotorCurrent & 0xFF;
	buffer[10]=rightMotorCurrent >> 8;
	buffer[11]=rightMotorCurrent & 0xFF;
	Wire.write(buffer,12);
}

void receive_Data(int receiveDataBytes)
{
	byte startByte=0x0F;
	byte x;
	x=Wire.read();
	if(x!=startByte){
		Serial.print("ERROR");	
		return;
	}
	else {
		Serial.print(x);
	}
}

void loop()
{
	delay(100);
	check_Trex_Sensors();	
}
