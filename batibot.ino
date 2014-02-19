#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
  #include "SoftwareSerial.h"
  SoftwareSerial port(12,13);
#else // Arduino 0022 - use modified NewSoftSerial
  #include "WProgram.h"
  #include "NewSoftSerial.h"
  NewSoftSerial port(12,13);
#endif

#include "EasyVR.h"
EasyVR easyvr(port);

//Groups and Commands
enum Groups
{
  GROUP_0  = 0,
  GROUP_1  = 1,
};

enum Group0 
{
  G0_BATIBOT = 0,
};

enum Group1 
{
  G1_ABANTE = 0,
  G1_ATRAS = 1,
  G1_KALIWA = 2,
  G1_KANAN = 3,
  G1_HINTO = 4,
};


EasyVRBridge bridge;

int8_t group, idx;
#define leftFmotor 6 //M1 Speed Control 
#define rightFmotor 5 //M2 Speed Control
#define leftRmotor 8 //M1 Direction Control
#define rightRmotor 7 //M2 Direction Control
int leftmotorSpeed = 200;
int rightmotorSpeed = 200;

void setup()
{
  // bridge mode?
  if (bridge.check())
  {
    cli();
    bridge.loop(0, 1, 12, 13);
  }
  // run normally
  Serial.begin(9600);
  port.begin(9600);

  if (!easyvr.detect())
  {
    Serial.println("EasyVR not detected!");
    for (;;);
  }

  easyvr.setPinOutput(EasyVR::IO1, LOW);
  Serial.println("EasyVR detected!");
  easyvr.setTimeout(5);
  easyvr.setLanguage(0);

  group = EasyVR::TRIGGER; //<-- start group (customize)
  
  for(int i=5;i<=8;i++) {
  		pinMode(i, OUTPUT);
  	} 
}

void action();

void loop()
{
  easyvr.setPinOutput(EasyVR::IO1, HIGH); // LED on (listening)

  Serial.print("Say a command in Group ");
  Serial.println(group);
  easyvr.recognizeCommand(group);

  do
  {
    // can do some processing while waiting for a spoken command
  }
  while (!easyvr.hasFinished());
  
  easyvr.setPinOutput(EasyVR::IO1, LOW); // LED off

  idx = easyvr.getWord();
  if (idx >= 0)
  {
    // built-in trigger (ROBOT)
    // group = GROUP_X; <-- jump to another group X
    return;
  }
  idx = easyvr.getCommand();
  if (idx >= 0)
  {
    // print debug message
    uint8_t train = 0;
    char name[32];
    Serial.print("Command: ");
    Serial.print(idx);
    if (easyvr.dumpCommand(group, idx, name, train))
    {
      Serial.print(" = ");
      Serial.println(name);
    }
    else
      Serial.println();
    easyvr.playSound(0, EasyVR::VOL_FULL);
    // perform some action
    action();
  }
  else // errors or timeout
  {
    if (easyvr.isTimeout())
      Serial.println("Timed out, try again...");
    int16_t err = easyvr.getError();
    if (err >= 0)
    {
      Serial.print("Error ");
      Serial.println(err, HEX);
    }
  }
}

void action()
{
    switch (group)
    {
    case GROUP_0:
      switch (idx)
      {
      case G0_BATIBOT:
        group = GROUP_1;
        break;
      }
      break;
    case GROUP_1:
      switch (idx)
      {
      case G1_ABANTE:
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        forward();
        break;
      case G1_ATRAS:
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        reverse();
        break;
      case G1_KALIWA:
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        left();
        break;
      case G1_KANAN:
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        right();
        break;
      case G1_HINTO:
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        stop();
        break;
      }
      break;
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
