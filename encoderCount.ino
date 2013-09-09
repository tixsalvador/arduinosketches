/* Copy and paste the code below into the Arduino software */
int E1 = 6; //M1 Speed Control
int E2 = 5; //M2 Speed Control
int M1 = 8; //M1 Direction Control
int M2 = 7; //M2 Direction Control
int rawencoderValue=0;
int encoderPin=0;
int encoderValue0 = 0;
int encoderValue1 = 0;
long count = 0;


void setup()
{
  int i;
  for(i=5;i<=8;i++)
  pinMode(i, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  int leftspeed = 255; //255 is maximum speed
  int rightspeed = 255;
  analogWrite (E1,255);
  digitalWrite(M1,LOW);
  analogWrite (E2,255);
  digitalWrite(M2,LOW);
  delay(100);
  rawencoderValue=analogRead(encoderPin);
  if(rawencoderValue < 600) {
	encoderValue1 = 1;
  }
  else {
   	encoderValue1 = 0;
  }

  if(encoderValue1 != encoderValue0) {
	count ++;
  } 
  encoderValue0 = encoderValue1;	
  Serial.println(count);
}
