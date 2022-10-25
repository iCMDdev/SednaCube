// SednaCube Flight Code
// This is the code used to record data from sensors during the flight, and send the location using the SIM
// Module after the device is ejected from the rocket.

// Some statistics:
//  - 96% Flash (program) memory occupied when compiled for a standard Arduino Nano (ATMEGA328p)
//  -
//  - 

#include <SD.h>
#include <SoftwareSerial.h> 
#include <Adafruit_BMP280.h>
#include <DHT.h>
#include <TinyGPS.h>
#include <EEPROM.h>
#include <MPU9250_asukiaaa.h>

// EJECT

bool ejected = false;
bool ejectEnabled;
uint8_t altitudeCnt = 0;
float maxAltitude = -100; 
float maxAltitudeCandidate = -100;
float altitude = 0;

// GPS
SoftwareSerial gpsSerial(5, 4); // GPS RX & TX
TinyGPS gps;
float flat, flon;

// SIM
SoftwareSerial simSerial(3, 2); // SIM800L RX & TX
unsigned long lastSMS = 0;

// Blinking LED
unsigned long lastLEDblink = 0;
bool ledOn = true;

// MISSION ONE
Adafruit_BMP280 bmp;

// MISSION 2
DHT dht(3, DHT11);
MPU9250_asukiaaa mpu;


void printComma(File &file) {
  // potentially save program space by reusing this function
  file.print(F(","));
}

void missionOne() {
  // this subroutine is responsible for recording data for SednaCube's first mission
  // as well as detecting if the device was ejected from the rocket (altitude has decreased by at least 1000m from the highest recorded altitude)
  File file;
  file = SD.open(F("m1.csv"), O_CREAT | O_APPEND | O_WRITE);
  if (file) {
    file.print(millis()); // time since boot (millis)
    printComma(file);
    file.print(bmp.readPressure());
    printComma(file);
    file.print(bmp.readTemperature());
    printComma(file);
    altitude = bmp.readAltitude(1013.25);
    if(ejectEnabled && !isnan(altitude) && altitude>100+maxAltitude) {
      maxAltitudeCandidate=altitude;
      altitudeCnt = 1;
    } else if (ejectEnabled && altitude>=maxAltitudeCandidate-100) {
      if(altitudeCnt >= 10) {
        maxAltitude = maxAltitudeCandidate;
        EEPROM.put(250, maxAltitude); // Save maximum altitude (just in case a power loss occurs)
      } else {
        altitudeCnt++;
        EEPROM.put(300, altitudeCnt); // Save maximum altitude count (just in case a power loss occurs)
      }
    } else if (ejectEnabled && !ejected) {
      maxAltitude = altitude;
      maxAltitudeCandidate = altitude;
      altitudeCnt = 0;
      EEPROM.put(250, maxAltitude); // Save maximum altitude (just in case a power loss occurs)
      EEPROM.put(300, altitudeCnt); // Save maximum altitude counter (just in case a power loss occurs)
    }
    if (!ejected && altitude < maxAltitude-1000 && altitudeCnt >= 10) {
      ejected = true;
      EEPROM.write(200, 1); // LOGGING: eject mode was enabled during the flight
    }
    file.println(altitude);
    file.close();
  }
}

void missionTwo() {
  // this subroutine is responsible for recording data for SednaCube's second mission
  File file;
  file = SD.open(F("m2.csv"), O_CREAT | O_APPEND | O_WRITE);
  mpu.accelUpdate();
  mpu.gyroUpdate();
  mpu.magUpdate();
  if (gpsSerial.available())
  {
    // read and encode GPS serial data
    gps.encode(gpsSerial.read());
  }
  gps.f_get_position(&flat, &flon);
  if (file) {
    file.print(millis()); // time since boot (millis)
    file.print(",\"");
    file.print(flat);
    file.print("\",");
    file.print(flon);
    printComma(file);
    file.print(dht.readHumidity());
    printComma(file);
    file.print(dht.readTemperature());
    printComma(file);
    file.print(mpu.accelX(), 2);
    printComma(file);
    file.print(mpu.accelY(), 2);
    printComma(file);
    file.print(mpu.accelZ(), 2);
    printComma(file);
    file.print(mpu.gyroX(), 2);
    printComma(file);
    file.print(mpu.gyroY(), 2);
    printComma(file);
    file.print(mpu.gyroZ(), 2);
    printComma(file);
    file.print(mpu.magX(), 2);
    printComma(file);
    file.print(mpu.magY(), 2);
    printComma(file);
    file.println(mpu.magZ(), 2);
    file.close();
  }
}

void SMSsend() {
  // send location update
  lastSMS = millis();
  simSerial.println(F("AT+CMGS=\"+ZZxxxxxxxxxx\"")); // phone number
  simSerial.print(F("LAT:"));
  simSerial.print(flat);
  simSerial.print(F(", LON:"));
  simSerial.print(flon);
  simSerial.print(F(", ALT:"));
  simSerial.print(altitude);
  simSerial.write(26);
}

void setup() {
  // Serial.begin(9600);

  // The process below is similar to setting up a device (for example, a laptop) for the first time, after the software 
  // was installed at the factory, OR after fully resetting the device (erasing everything and reinstalling the software; in this case,
  // performing an EEPROM RESET)
  
  // You can only enable / disable the EJECT MODE once, when setting up the device, by booting the device with a selected switch position.
  // This is made in order to prevent accidental switch toggles during the flight, even if the SednaCube loses power and restarts the program.

  // Check if eject mode was enabled or not
  // by reading the EEPROM byte at address 100
  pinMode(LED_BUILTIN, OUTPUT);
  if(EEPROM.read(50)==255) {
    delay(1000);
    // If value is 255, nothing has been written to EEPROM. We're in the first boot (just when program was uploaded)
    
    // Serial.println("Program uploaded. Waiting for reboot.");
    
    EEPROM.write(50, 1); // Remember the first boot.
    
    while(true) {
      // WAITING FOR NEXT BOOT.
      // Blink LED: 1s ON, 0.5 OFF
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      digitalWrite(LED_BUILTIN, LOW);
      delay(500);
    }
  } else if(EEPROM.read(100)==255) {
    delay(5000);
    // If value is 255, nothing has been written to EEPROM. Entering first-time setup.

    // Serial.println("Entering first-time setup");

    // This means it is the first time the PocketQube has started after "factory reset" or EEPROM ERASE (full reset),
    // So this is when the eject mode's switch check should e performed.
    pinMode(8, INPUT_PULLUP);
    delay(300);

    ejectEnabled = (digitalRead(8) == 0);
    EEPROM.write(100, ejectEnabled); // read switch state and write it into EEPROM
    
    while(true) {
      // WAITING FOR NEXT BOOT.
      // Blink LED: 0.1s ON, 0.5 OFF
      digitalWrite(LED_BUILTIN, HIGH);
      delay(2000);
      digitalWrite(LED_BUILTIN, LOW);
      delay(500);
    }
    
  } else {
    // The device is in the rocket, and setup was completed during a previous boot. Do NOT perform a switch read, as it's position
    // might've accidentally changed during the flight. Instead, read the data saved in EEPROM
    // Serial.println("Device was rebooted. Reading EEPROM");
    ejectEnabled = (EEPROM.read(100)==1);
    EEPROM.get(200, maxAltitude);
    EEPROM.get(250, altitudeCnt);
  }
  pinMode(9, OUTPUT);
  digitalWrite(9, LOW); // turn off BLUE LED
  bmp.begin(0x76);
  dht.begin();
  if (!SD.begin(10)) {
    // SD card was not init
    while (1);
  }
  Wire.begin();
  mpu.setWire(&Wire);
  mpu.beginAccel();
  mpu.beginGyro();
  mpu.beginMag();
  simSerial.println(F("AT"));
  simSerial.println(F("AT+CMGF=1")); // Configuring SIM TEXT mode
  
}

void ledBlink() {
  // Blink the blue LED at a rate specific to the program's current state
  
  // ---------------- Program states indicated by LED blinks: ---------------
  // - Program uploaded, waiting for next boot:          ON: 0.1s, OFF: 0.5s
  // - Switch position saved, waiting for next boot:     ON:   2s, OFF:   1s
  // - Eject enabled & Device was ejected from rocket:   ON: 0.5s, OFF: 0.5s
  // - Eject enabled & Device was not ejected yet:       ON:   1s, OFF:   1s
  // - Eject disabled:                                   ON:   2s, OFF:   2s
  // ------------------------------------------------------------------------
  
  if(ejected && ejectEnabled && millis()-lastLEDblink>=500) {
    lastLEDblink = millis();
    ledOn = !ledOn;
    digitalWrite(LED_BUILTIN, ledOn);
  } else if(ejectEnabled && millis()-lastLEDblink>=1000) {
    lastLEDblink = millis();
    ledOn = !ledOn;
    digitalWrite(LED_BUILTIN, ledOn);
  } else if (!ejectEnabled && millis()-lastLEDblink>=2000) {
    lastLEDblink = millis();
    ledOn = !ledOn;
    digitalWrite(LED_BUILTIN, ledOn);
  }
}

void loop() {
  missionOne(); // record data for the first mission
  missionTwo(); // record data for the second mission
  if(ejected && (millis()-lastSMS>60000)) {
    // if the device was ejected, send an SMS including locaton updates every minute
    SMSsend();
  }
  ledBlink(); // blink the blue PROGRAM LED at a rate specific to the program's current state
}
