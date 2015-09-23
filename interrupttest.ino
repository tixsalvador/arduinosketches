int data;
void setup()
{
	Serial.begin(9600);
	digitalWrite(2,HIGH);
	attachInterrupt(0,light,FALLING);
}

void loop()
{
	data=(analogRead(A0));
	if(data<500){
		digitalWrite(2,LOW);
	}
	else {
		digitalWrite(2,HIGH);
		Serial.print("WALANG ILAW");
		Serial.print("\t");
		Serial.println(data);
	}
	
}

void light()
{
		Serial.println("Gumagana");
		Serial.print("\t");
                Serial.println(data);
}
	
