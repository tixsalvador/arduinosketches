/* Changes
090913 	Remove Serial.print on bots movement
091013	Fix encoder pair count value
*/

#define leftFmotor 6 //M1 Speed Control 
#define rightFmotor 5 //M2 Speed Control
#define leftRmotor 8 //M1 Direction Control
#define rightRmotor 7 //M2 Direction Control
int leftmotorSpeed = 200;
int rightmotorSpeed = 200;
long count = 0;
int rawsensorData = 0;
int sensorData1= 0;
int sensorData0 = 0;

void setup()
{
	Serial.begin(9600);
  	for(int i=5;i<=8;i++) {
  		pinMode(i, OUTPUT);
  	} 
}

void loop()
{
	rawsensorData = analogRead(0);
	encoderCount();
	while(Serial.available() > 0) {
		char key = Serial.read();
		switch(key) {
			case 'w':
				forward();
				break;
			case 's':
				reverse();
				break;
			case 'a':
				left();
				break;
			case 'd':
				right();
				break;
			case 'p':
				speedUp();
				break;
			case 'l':
				speedDown();
				break;
			default:
				stop();
				break;
		}
	}
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
	analogWrite(leftFmotor, leftmotorSpeed);
	analogWrite(rightFmotor, rightmotorSpeed);
	digitalWrite(leftRmotor, LOW);
	digitalWrite(rightRmotor, LOW);
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

void speedUp()
{
	leftmotorSpeed = leftmotorSpeed + 5;
	rightmotorSpeed = rightmotorSpeed + 5;
	leftmotorSpeed = constrain(leftmotorSpeed,100,255);
	rightmotorSpeed = constrain(rightmotorSpeed,100,255);
	Serial.println(leftmotorSpeed);
	Serial.println(rightmotorSpeed);
}

void speedDown()
{
	leftmotorSpeed = leftmotorSpeed - 5;
	rightmotorSpeed = rightmotorSpeed - 5;
	leftmotorSpeed = constrain(leftmotorSpeed,100,255);
	rightmotorSpeed = constrain(rightmotorSpeed,100,255);
	Serial.println(leftmotorSpeed);
	Serial.println(rightmotorSpeed);
}

void encoderCount()
{
	if(rawsensorData < 600) {
                sensorData1 = 1;
        }
        else {
                sensorData1 =0;
        }
        if(sensorData1 != sensorData0) {
                count ++;
        }
        sensorData0 = sensorData1;
	Serial.println(count);
}
