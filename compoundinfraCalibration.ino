/*
Compoung IR Calibration
*/


#define infraUp A5
#define infraLeft A4
#define infraDown A3
#define infraRight A2
#define infraLeds 10

/*
#define irledPin 10
#define irupPin A5
#define irleftPin A4
#define irdownPin A3
#define irrightPin A2
*/

int infraUpvalue;
int infraLeftvalue;
int infraDownvalue;
int infraRightvalue;
int irDistance;

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
	digitalWrite(infraLeds, LOW);
	delayMicroseconds(100);
	infraUpvalue=infraUpvalue - analogRead(infraUp);
	infraLeftvalue=infraLeftvalue - analogRead(infraLeft);
	infraDownvalue=infraDownvalue - analogRead(infraDown);
	infraRightvalue=infraRightvalue - analogRead(infraRight);
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
