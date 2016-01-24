#include <Wire.h>

int16_t acX,acY,acZ,Tmp,gyX,gyY,gyZ;

const int AccelSen=16384;
const int GyroSen=131;

void setup()
{
        Serial.begin(9600);
        Wire.begin();
        Wire.beginTransmission(0x68);
        Wire.write(0x6B);
        Wire.write(0);
        Wire.endTransmission(true);
}

void loop()
{
        Wire.beginTransmission(0x68);
        Wire.write(0x3B);
        Wire.endTransmission(false);
        Wire.requestFrom(0x68,14,true);
        acX=Wire.read()<<8|Wire.read();
        acY=Wire.read()<<8|Wire.read();
        acZ=Wire.read()<<8|Wire.read();
        Tmp=Wire.read()<<8|Wire.read();
        gyX=Wire.read()<<8|Wire.read();
        gyY=Wire.read()<<8|Wire.read();
        gyZ=Wire.read()<<8|Wire.read();
        int acXsen=acX/AccelSen;
        int acYsen=acY/AccelSen;
        int acZsen=acZ/AccelSen;
        gyX=gyX/GyroSen;
        gyY=gyY/GyroSen;
        gyZ=gyZ/GyroSen;
        Serial.print(acXsen);
        Serial.print("  ");
        Serial.print(acYsen);
        Serial.print("  ");
        Serial.print(acZsen);
        Serial.print("  ");
        Serial.print(Tmp/340.00+36.53);
        Serial.print("  ");
        Serial.print(gyX);
        Serial.print("  ");
        Serial.print(gyY);
        Serial.print("  ");
        Serial.println(gyZ);
}
