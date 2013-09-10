import procontroll.*;
import java.io.*;
import processing.serial.*;

ControllIO controll;
ControllDevice device;

ControllStick stick1;
ControllStick stick2;
ControllButton button1;
ControllButton button2;

Serial port;

byte x_left;
byte y_left;
byte y_right;
byte buttonmap8;
byte buttonmap9;
char forward = 'w';
char backward = 's';
char left = 'a';
char right = 'd';
char speedup = 'p';
char speeddown = 'l';
char PacketStart = '~';


void setup()
{
  size(100,100);
  port = new Serial(this, Serial.list()[4], 9600); 
  controll = ControllIO.getInstance(this);
  device = controll.getDevice("Wireless 360 Controller");
  stick1 = device.getStick("y x");
  stick2 = device.getStick("rz z");
  button1 = device.getButton(8); 
  button2 = device.getButton(9);
  stick1.setTolerance(0.10f);
  stick2.setTolerance(0.10f);
  fill(0);
  rectMode(CENTER);
}


void draw()
{
  background(255); 
  y_left = (byte) stick1.getX();
  x_left = (byte) stick1.getY();
  y_right = (byte) map(stick2.getX(), 1, -1, -100, 100) ;
  buttonmap8 = (byte) button1.getValue();
  buttonmap9 = (byte) button2.getValue();
  
  port.write( PacketStart );
  if(y_left == -1) {
    port.write(forward);
  }
  else if(y_left == 1){
    port.write(backward);
  }
  else if(x_left == 1){
    port.write(right);
  }
  else if(x_left == -1){
    port.write(left);
  }
  
 if(buttonmap8 == 8){
    port.write(speedup);
  }
  else if(buttonmap9 == 8) {
    port.write(speeddown);
  }
  
  ellipse( (width/2 - 10), (((y_left*100)/5) + height/2), 10, 10);    
  ellipse( (width/2 + 10), (-(y_right/5) + height/2), 10, 10);
  
/*
  print(y_left);
  print("  ");
  println(x_left);
  //println(y_right);
*/
}
