unsigned long start, finish, elapsed, over;
int sec;

void setup(){
	Serial.begin(9600);
	pinMode(3,INPUT);
	pinMode(4,INPUT);

}

void loop() {
	if (digitalRead(3) == HIGH) {
		start=millis();
	}
	if (digitalRead(4) == HIGH) {
		finish=millis();
		get_time();
		Serial.println(sec);
	}
}

void get_time() {

	elapsed=finish-start;
	over=(elapsed % 3600000);
	sec=(over / 1000);

}
