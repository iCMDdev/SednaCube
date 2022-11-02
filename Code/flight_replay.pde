// SednaCube | Flight Replay
// This program is used to simulate the flight, by using the recorded data.
// Drag and drop the M2.CSV file

int lf = 10;
int comma = 44;
float yaw = 0;
float pitch = 0;
float roll = 0;
String altitude = " ";
boolean playing = true;
int i = 0;
String[] lines;

void setup() {
    size(800, 500, P3D);
    lines = loadStrings("M2.CSV");
    
   
}

void mousePressed() {
  if(mouseX <= 725 && mouseX >= 700 && mouseY <=40 && mouseY >= 10)
    playing = !playing;
  if(mouseX <= 725 && mouseX >= 698 && mouseY >=40 && mouseY <= 70)
    i = 0;
  if(mouseX <= 685 && mouseX >= 665 && mouseY >=20 && mouseY <= 30)
  {   
    i -= 10;
    if(i<0) i=0;
  }
  if(mouseX <= 760 && mouseX >= 740 && mouseY >=20 && mouseY <= 30)
  {   
    i += 10;
    if(i>lines.length-1)
      i = lines.length-1;
  }
}

void draw() {
  String[] entrydata = split(lines[i], ',');
  
  background(0);
  fill(255);
  

  text("Reset", 698, 60);
  
  triangle(685, 20, 685, 30, 675, 25);
  triangle(675, 20, 675, 30, 665, 25);
  
  triangle(740, 20, 740, 30, 750, 25);
  triangle(750, 20, 750, 30, 760, 25);
  
  if(playing) {
    rect(700, 10, 7, 30);
    rect(725, 10, -7, 30);
  } else {
    triangle(700, 10, 700, 40, 725, 25);
  }
  text("LAT: "+entrydata[1] +", LON: " + entrydata[2], 20, 480);
  text("Time: "+ entrydata[0] +" ms", 20, 460);
  //text("Altitude: "+altitude+" m", 20, 20);
  //text("Temperature1: "+altitude+" C", 20, 40);
  text("Humidity: "+entrydata[3]+" %", 20, 30);
  text("TemperatureDHT: "+entrydata[4]+" C", 20, 60);
  //text("Pressure: "+entrydata[4]+" hPa", 20, 80);
  text("magX: "+entrydata[11]+" T", 20, 100);
  text("magY: "+entrydata[12]+" T", 20, 120);
  text("magZ: "+entrydata[13]+" T", 20, 140);
  text("Compass XY: " + atan2(60, 30)*180/(PI) + " deg" , 20, 160);
  text("Compass XZ: " + atan2(30, 30)*180/(PI) + " deg" , 20, 180);
  text("Compass YZ: " + atan2(10, 30)*180/(PI) + " deg" , 20, 200);
  translate(width/2, height/2, 0);
  stroke(255);
  rotateX(PI/2);
  //rotateZ(-PI/6);
  noFill();
  // Rotate using data provided from the sensor
  
  // calculate using accelerometer data (yaw is not absolute!)
  pitch = atan (float(entrydata[5])/sqrt(float(entrydata[6])*float(entrydata[6]) + float(entrydata[7])*float(entrydata[7])));
  roll = atan (float(entrydata[6])/sqrt(float(entrydata[5])*float(entrydata[5]) + float(entrydata[7])*float(entrydata[7])));
  yaw = atan (float(entrydata[7])/sqrt(float(entrydata[5])*float(entrydata[5]) + float(entrydata[7])*float(entrydata[7])));
  
  rotateX(roll);
  rotateY(pitch);
  rotateZ(yaw);
  
  beginShape(/*QUADS*/);
  fill(255); vertex(-100, -100, -100);
  fill(255); vertex( 100, -100, -100);
  fill(255); vertex(   0,    0,  100);
  
  fill(255); vertex( 100, -100, -100);
  fill(255); vertex( 100,  100, -100);
  fill(255); vertex(   0,    0,  100);
  
  fill(255); vertex( 100, 100, -100);
  fill(255); vertex(-100, 100, -100);
  fill(255); vertex(   0,   0,  100);
  
  fill(255); vertex(-100,  100, -100);
  fill(255); vertex(-100, -100, -100);
  fill(255); vertex(   0,    0,  100);
  endShape(); 
  if(playing) {
    if(i<lines.length-1)
      i++;
    else playing = false;
  }
  
  //delay(100);
}
