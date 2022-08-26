#include <Adafruit_BMP280.h>
#include <MPU9250.h>

// SednaCube Debugging Code ----------------------------------
//  -> GY-91 (BM280 + MPU9250) TEST with Serial output

// Use this with the Processing program (3D-IMU.pde) for a 3D live preview

// This code uses the following libraries, installed using Library Manager:
//    -> MPU Library: MPU9250 by hideakitai (https://github.com/hideakitai/MPU9250)
//    -> BMP280 Library: Adafruit BMP280 Library by Adafruit (https://github.com/adafruit/Adafruit_BMP280_Library)

Adafruit_BMP280 bmp;
MPU9250 mpu;

void setup() {
  bmp.begin(0x76);

  Serial.begin(115200);
  Wire.begin();

  if (!mpu.setup(0x68)) {
      while (1) {
          Serial.println("MPU connection failed.");
          delay(5000);
      }
  }
}

uint32_t previousSerialWrite = millis();

void loop() {
    if (mpu.update()) {
      if (millis() > previousSerialWrite + 25) {
        Serial.print(mpu.getYaw(), 2);
        Serial.print(", ");
        Serial.print(mpu.getPitch(), 2);
        Serial.print(", ");
        Serial.print(mpu.getRoll(), 2);
        Serial.print(", ");
        Serial.println(bmp.readAltitude(1013.25), 2);
        previousSerialWrite = millis();
      }
    }   
}
