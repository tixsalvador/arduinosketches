/*
Compoung IR Calibration
*/


#define infraUp A2
#define infraLeft A3
#define infraDown A0
#define infraRight A1
#define infraLeds 11

int infraUpvalue;
int infraLeftvalue;
int infraDownvalue;
int infraRightvalue;

void setup()
{
        Serial.begin(9600);
        pinMode(infraLeds, OUTPUT);
}

void infraRead()
{
        digitalWrite(infraLeds, HIGH);
        delay(50);
        infraUpvalue=analogRead(infraUp);
        infraLeftvalue=analogRead(infraLeft);
        infraDownvalue=analogRead(infraDown);
        infraRightvalue=analogRead(infraRight);
}


void loop()
{
        infraRead();
        Serial.print(infraLeftvalue);
        Serial.print(" ");
        Serial.print(infraRightvalue);
        Serial.print(" ");
        Serial.print(infraUpvalue);
        Serial.print(" ");
        Serial.println(infraDownvalue);

}
