// SednaCube | 3D IMU Orientation Simulator
// This program is used to simulate the SednaCube's orientation, by using the IMU.
// It communicates with the device over serial. Device should be running the GY-91-test.ino program.

import processing.serial.*;

Serial myPort;
int lf = 10;
int comma = 44;
float yaw = 0;
float pitch = 0;
float roll = 0;
String altitude = " ";

void setup() {
    size(500, 500, P3D);
    // print(Serial.list()); /* uncomment to see a list of serial ports */
    myPort = new Serial(this, "/dev/cu.usb...", 115200); // change with your own serial port
}

void draw() {
  if (myPort.available() > 0) {
    String myString = myPort.readStringUntil(comma);
    if (myString != null) {
      yaw = float(myString.substring(0, myString.length()-2));
      // println(yaw);
    }
    
    myString = myPort.readStringUntil(comma);
    if (myString != null) {
      pitch = float(myString.substring(1, myString.length()-2));
      // println(pitch);
    }
    
    myString = myPort.readStringUntil(comma);
    if (myString != null) {
      roll = float(myString.substring(1, myString.length()-2));
      // println(roll);
    }
    
    myString = myPort.readStringUntil(lf);
    if (myString != null) {
      altitude = myString.substring(1, myString.length()-2);
      // println(altitude);
    }
  }
  background(0);
  text("Altitude: "+altitude+" m", 20, 20);
  translate(width/2, height/2, 0);
  stroke(255);
  rotateX(PI/2);
  //rotateZ(-PI/6);
  noFill();
  
  // Rotate using data provided from the sensor
  rotateX(radians(roll));
  rotateY(radians(pitch));
  rotateZ(radians(yaw));
  
  beginShape();
  vertex(-100, -100, -100);
  vertex( 100, -100, -100);
  vertex(   0,    0,  100);
  
  vertex( 100, -100, -100);
  vertex( 100,  100, -100);
  vertex(   0,    0,  100);
  
  vertex( 100, 100, -100);
  vertex(-100, 100, -100);
  vertex(   0,   0,  100);
  
  vertex(-100,  100, -100);
  vertex(-100, -100, -100);
  vertex(   0,    0,  100);
  endShape(); 
}
