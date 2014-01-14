#define irupPin A4
#define irdownPin A2
#define irleftPin A5
#define irrightPin A3
#define irledPin 11

int irupValue;
int irdownValue;
int irleftValue;
int irrightValue;
int irDistance;

void setup()
{
	Serial.begin(9600);
	pinMode(irledPin, OUTPUT);
}

void getirDistance()
{
        digitalWrite(irledPin, HIGH);
        delayMicroseconds(100);
        irdownValue=analogRead(irdownPin);
        irrightValue=analogRead(irrightPin);
        irupValue=analogRead(irupPin);
        irleftValue=analogRead(irleftPin);
        digitalWrite(irledPin, LOW);
        delayMicroseconds(100);
        irdownValue=irdownValue - analogRead(irdownPin);
        irrightValue=irrightValue - analogRead(irrightPin);
        irupValue=irupValue - analogRead(irupPin);
        irleftValue=irleftValue - analogRead(irleftPin);
        irDistance=(irdownValue + irrightValue + irupValue + irleftValue) / 4;
}

void loop()
{
	Serial.print(irDistance);
	Serial.print(" ");
	Serial.print(irupValue);
	Serial.print(" ");
	Serial.print(irdownValue);
	Serial.print(" ");
	Serial.print(irleftValue);
	Serial.print(" ");
	Serial.println(irrightValue);
}
