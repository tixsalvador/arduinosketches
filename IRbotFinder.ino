int northPin=A5;
int eastPin=A4;
int southPin=A3;
int westPin=A2;

const int motorforwardPin1=5;
const int motorforwardPin2=6;
const int motorbackwardPin1=7;
const int motorbackwardPin2=8;

int motorSpeed=255;
unsigned int count=0;

const int ultrasensorPin=3;

void setup()
{
	Serial.begin(9600);
	pinMode(A2, INPUT);
	pinMode(A3, INPUT);
	pinMode(A4, INPUT);
	pinMode(A5, INPUT);
	pinMode(5, OUTPUT);
	pinMode(6, OUTPUT);
	pinMode(7, OUTPUT);
	pinMode(8, OUTPUT);
	pinMode(11, OUTPUT);

}

void loop()
{
	int irledPin=11;
	int leftsensorPin=A0;
	int rightsensorPin=A1;
	digitalWrite(irledPin, HIGH);
	delayMicroseconds(100);
	int leftpinValue=analogRead(A0);
	int rightpinValue=analogRead(A1);
	digitalWrite(irledPin, LOW);
	delayMicroseconds(100);
	leftpinValue=leftpinValue - analogRead(leftsensorPin);
	rightpinValue=rightpinValue - analogRead(rightsensorPin);
	int ircompoundDistance=(leftpinValue + rightpinValue) / 2;
	Serial.print(leftpinValue);
	Serial.print("	");
	Serial.print(rightpinValue);
	Serial.print("	");
	Serial.println(ircompoundDistance);
	
	if(distance()>10){
		move();
	}
	else if((distance()<7)||(ircompoundDistance>400)){
		stop();
	}	
}

void move()
{
	if(direction()==1){
		forward();
	}
	else if(direction()==2){
		rightTurn();
	}
	else if(direction()==3){
		leftTurn();
	}
	else if(direction()==4){
		int turnLeft=analogRead(westPin);
		int turnRight=analogRead(eastPin);
		if(turnLeft < turnRight){
			leftTurn();
		}
		else if(turnLeft > turnRight){
			rightTurn();
		}
	}
	else
	stop();
}

int direction()
{
	struct sensorData {
		int northData;
		int eastData;
		int westData;
		int southData;
	};
	int northReading;
	int eastReading;
	int southReading;
	int westReading;
	int N=0;
	int E=0;
	int W=0;
	int S=0;
	int irDirection=0;
	struct sensorData sensorReading[25];
	for(int i=0;i<=4;i++){
		northReading=analogRead(northPin);
        	eastReading=analogRead(eastPin);
        	southReading=analogRead(southPin);
        	westReading=analogRead(westPin);
		sensorReading[i].northData=northReading;
		sensorReading[i].eastData=eastReading;
		sensorReading[i].westData=westReading;
		sensorReading[i].southData=southReading;
		if(sensorReading[i].northData < 100){
			N += 1;
		}
		else if(sensorReading[i].eastData < 100){
			E += 1;
		}
		else if(sensorReading[i].westData < 100){
			W += 1;
		}
		else if(sensorReading[i].southData < 100){
			S += 1;
		}
		delay(25);
	}
	if((N>E) && (N>W) && (N>S)) {
		irDirection=1;
	}
	else if((E>N) && (E>W) && (E>S)){
		irDirection=2;
	}
	else if((W>N) && (W>E) && (W>S)){
		irDirection=3;
	}
	else if((S>N) && (S>E) && (S>W)){
		irDirection=4;
	}
	else
		irDirection=0;
	return irDirection;
}


int distance()
{
	int duration;
       	pinMode(ultrasensorPin, OUTPUT);
       	digitalWrite(ultrasensorPin, LOW);
       	delayMicroseconds(5);
       	digitalWrite(ultrasensorPin,HIGH);
       	delayMicroseconds(5);
       	digitalWrite(ultrasensorPin, LOW);
       	pinMode(ultrasensorPin, INPUT);
       	duration=pulseIn(ultrasensorPin, HIGH);
       	duration=(duration/29)/2;
	return duration;
}

void forward()
{
		analogWrite(5,motorSpeed);
        	analogWrite(6,motorSpeed);
        	digitalWrite(motorbackwardPin1,LOW);
        	digitalWrite(motorbackwardPin2,LOW);

}

void reverse()
{
	analogWrite(5,motorSpeed);
        analogWrite(6,motorSpeed);
	digitalWrite(7,HIGH);
        digitalWrite(8,HIGH);
}

void left()
{
	 analogWrite(5,motorSpeed);
        analogWrite(6,100);
	digitalWrite(7,LOW);
        digitalWrite(8,HIGH);
}

void leftTurn(){
        analogWrite(5,motorSpeed);
        analogWrite(6,motorSpeed);
        digitalWrite(motorbackwardPin1,LOW);
        digitalWrite(motorbackwardPin2,HIGH);
}


void right()
{
        analogWrite(5,100);
        analogWrite(6,motorSpeed);
        digitalWrite(7,LOW);
        digitalWrite(8,LOW);
}

void rightTurn(){
	analogWrite(5,motorSpeed);
        analogWrite(6,motorSpeed);
        digitalWrite(motorbackwardPin1,HIGH);
        digitalWrite(motorbackwardPin2,LOW);
}

void stop()
{
	digitalWrite(5,LOW);
        digitalWrite(6,LOW);
	digitalWrite(7,LOW);
        digitalWrite(8,LOW);
}

/*
int encoderCount()
{
        int encoderData1=analogRead(A0);
        int encoderData2=analogRead(A1);
        int motorBreak1=digitalRead(motorbackwardPin1);
        int motorBreak2=digitalRead(motorbackwardPin2);
        int encoderValue1=0;
        int encoderValue2=0;
        boolean movingForward=false;

        if((encoderData1 < 500) && (encoderData2 < 500) && ((motorBreak1 == LOW) || (motorBreak2 == LOW))){
                encoderValue1=1;
                movingForward=true;
        }
        else {
                encoderValue1=0;
        }
        if((encoderValue1 != encoderValue2) && (movingForward == true)){
                count ++;
        }
        encoderValue2=encoderValue1;
        return count;
}
*/
