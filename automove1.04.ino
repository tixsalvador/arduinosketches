/*
091513	Configure Ultrasonic Sensor 
091613	Configure bot movement relative to encoder pair
091813	Added bot Uturn
092313	Added bot Left and Right Turn
*/

#define leftFmotor 6 //M1 Speed Control 
#define rightFmotor 5 //M2 Speed Control
#define leftRmotor 8 //M1 Direction Control
#define rightRmotor 7 //M2 Direction Control
#define distancesensorPin 3
int leftmotorSpeed=255;
int rightmotorSpeed=255;
long count = 0;
int rawsensorData = 0;
int sensorData1= 0;
int sensorData0 = 0;
boolean movingForward=false;
boolean crash=false;
int crashzone=5; //Value in centimiters
int backstepCount=10;
int uturnCount=25; //Default is 35 Value; change to 25 because of backstepcount
int lturnCount=8;
int rturnCount=5;

void setup()
{
	Serial.begin(9600);
	pinMode(distancesensorPin, OUTPUT);
	for(int i=5; i<=8; i++) {
		pinMode(i, OUTPUT);
	}
}

void loop()
{
	forward();
	Serial.println(encoderCount());
}

void stop()
{
	digitalWrite(leftFmotor, LOW);
        digitalWrite(rightFmotor, LOW);
        digitalWrite(leftRmotor, LOW);
        digitalWrite(rightRmotor, LOW);
}

void forward()
{
	int backstep=((encoderCount()) - backstepCount);
	int uturn=((encoderCount()) + uturnCount);
	int lturn=((encoderCount()) + lturnCount);
	int rturn=((encoderCount()) + rturnCount);
        if((getDistance()) < crashzone) {
                crash=true;
        }
        if(crash == false) {
            analogWrite(leftFmotor, leftmotorSpeed);
			analogWrite(rightFmotor, rightmotorSpeed);
			digitalWrite(leftRmotor, LOW);
			digitalWrite(rightRmotor, LOW);
        }
        if(crash == true) {
			do {
                	reverse();
			}	
			while((encoderCount()) >  backstep);
			do {
			left();
			}
			while((encoderCount()) < rturn);
			crash=false;
        }	
        //Serial.println(encoderCount());
}
		
void reverse()
{
    analogWrite(leftFmotor, leftmotorSpeed);
    analogWrite(rightFmotor, rightmotorSpeed);
    digitalWrite(leftRmotor, HIGH);
    digitalWrite(rightRmotor, HIGH);
}

void left()
{
        analogWrite(leftFmotor, leftmotorSpeed);
        analogWrite(rightFmotor, rightmotorSpeed);
        digitalWrite(leftRmotor, HIGH);
        digitalWrite(rightRmotor, LOW);
}

void right()
{
	analogWrite(leftFmotor, leftmotorSpeed);
    	analogWrite(rightFmotor, rightmotorSpeed);
    	digitalWrite(leftRmotor, LOW);
    	digitalWrite(rightRmotor, HIGH);
}

int encoderCount()
{
	rawsensorData=analogRead(0);
	if(rawsensorData < 600 && (digitalRead(leftRmotor) == LOW || digitalRead(rightRmotor) == LOW)) {
            movingForward=true;
            sensorData1 = 1;
    }
    else if(rawsensorData < 600 && digitalRead(leftRmotor) == HIGH && digitalRead(rightRmotor) == HIGH) {
            movingForward=false;
            sensorData1 = 1;
    }
    else {
    		sensorData1 = 0;
    }
    
    if(sensorData1 != sensorData0 && movingForward == true) {
            count ++;
    }
    else if (sensorData1 != sensorData0 && movingForward == false) {
      		count --;
    }
    sensorData0 = sensorData1;
    return count;
}

int getDistance()
{
        int distance;
        unsigned long pulse =0;

        pinMode(distancesensorPin, OUTPUT);
        digitalWrite(distancesensorPin, LOW);
        delayMicroseconds(5);

        digitalWrite(distancesensorPin, HIGH);
        delayMicroseconds(5);
        digitalWrite(distancesensorPin, LOW);

        pinMode(distancesensorPin, INPUT);
        pulse=pulseIn(distancesensorPin, HIGH);
        pulse=pulse/2;
        distance = int(pulse/29);
        return distance;
}
