#define DATA 6
#define LATCH 8
#define CLOCK 10
int digits[] = {252, 96, 218, 242, 102, 182, 190, 224, 254, 246, 238, 62, 156,
122, 158, 142};

void setup() {
	pinMode(DATA, OUTPUT);
	pinMode(LATCH, OUTPUT);
	pinMode(CLOCK, OUTPUT);
}

void count(int num) {
	if(num < 10) {
	digitalWrite(LATCH, LOW);
	shiftOut(DATA,CLOCK,LSBFIRST, 252);
	shiftOut(DATA,CLOCK,LSBFIRST, digits[num]);
	digitalWrite(LATCH, HIGH);
	delay(1000);
	}
	else if (num >=10) {
	int up=(num/10);
	int down=(num%10);
	digitalWrite(LATCH, LOW);
	shiftOut(DATA,CLOCK,LSBFIRST, digits[up]);
	shiftOut(DATA,CLOCK,LSBFIRST, digits[down]);
	digitalWrite(LATCH, HIGH);
	}
}

void loop() {
	for(int i=0;i<100;i++) {
	count(i);
	delay(1000);
	}
}
