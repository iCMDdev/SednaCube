// EEPROM RESET
// This program "factory" resets the SednaCube by erasing and rewriting the EEPROM.

// This program may be used to set a new switch state for the EJECT switch
// and to remove previously saved logging data.

#include <EEPROM.h>

uint16_t a = 0; // address that is erased; incremented each time an address has ben erasedin loop()

void setup()
{
  Serial.begin(9600);
  Serial.println("Starting EEPROM erase program.");
}

void loop()
{
  if(EEPROM.read(a) != 255) {
    EEPROM.write(a, 255);
  }
  a = a + 1;
  if (a == 512) {
    float f = 0f;
    uint8_t cnt = 0;
    Serial.println("EEPROM ERASED SUCCESSFULLY");
    EEPROM.put(200, f); // maximum altitude init
    EEPROM.put(250, cnt); // maximum altitude count init
    Serial.println("Done! Device's EEPROM was fully reset.");
  }
}
