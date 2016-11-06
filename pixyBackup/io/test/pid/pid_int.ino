uint32_t lastTime;
const int Setpoint=15;
int kp,ki,kd;
long  Output;
long lastErr=0;
long errSum;

void setup()
{
	Serial.begin(9600);
}


void loop()
{
	tunings(100,100,200);
	compute();
}

void compute()
{
	int16_t Input=analogRead(A0);
	Input=map(Input,0,1024,0,50);
	Input=constrain(Input,0,50);
	long now=millis();
	long timeChange=(long)(now-lastTime);

	int error=Setpoint-Input;
	errSum+=(error*timeChange);
	double dErr=(error-lastErr)/timeChange;

	
	long proportional=kp*error;
	long integral=ki*errSum;
	double derivative=kd*dErr;
	Output=(proportional+integral+derivative);
//	Output=kp*error+ki*errSum+kd*dErr;
		
	lastErr=error;
	lastTime=now;
	
	Serial.print(Input);
	Serial.print("\t");
	Serial.print(proportional);
	Serial.print("\t");
	Serial.print(error);
	Serial.print("\t");
	Serial.print(lastErr);
	Serial.print("\t");
	Serial.println(timeChange);
}

void tunings(int Kp,int Ki,int Kd)
{
	kp = Kp;
	ki = Ki;
	kd = Kd;
}
