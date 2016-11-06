uint32_t lastTime;
const int Setpoint=15;
double kp,ki,kd;
double  Output,lastErr;
double errSum;

void setup()
{
	Serial.begin(9600);
}


void loop()
{
	tunings(100,100,100);
	compute();
}

void compute()
{
	int16_t Input=analogRead(A0);
	Input=map(Input,0,1024,0,50);
	Input=constrain(Input,0,50);
	uint32_t now=millis();
	double timeChange=(double)(now-lastTime);

	double error=Setpoint-Input;
	errSum+=(error*timeChange);
	double dErr=(error-lastErr)/timeChange;

	
	double proportional=kp*error;
	double integral=ki*errSum;
	double derivative=kd*dErr;
	Output=(proportional+integral+derivative);
//	Output=kp*error+ki*errSum+kd*dErr;
		
	lastErr=error;
	lastTime=now;
	
	delay(500);
	Serial.print(Input);
	Serial.print("\t");
	Serial.print(proportional);
	Serial.print("\t");
	Serial.print(integral);
	Serial.print("\t");
	Serial.print(derivative);
	Serial.print("\t");
	Serial.println(Output);
}

void tunings(double Kp,double Ki,double Kd)
{
	kp = Kp;
	ki = Ki;
	kd = Kd;
}
