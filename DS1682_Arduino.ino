#include <Wire.h>

#define DS1682_ADDR 0x6B

#define EVENT_COUNTER 0x09
#define CONFIG_REG 0x00
#define ALM_REG 0x01
#define ETC_REG 0x05
#define RST_REG 0x1D
#define WD_REG 0x1E
#define WMD_REG 0x1F

//Config register bits
#define ECMSB 0x00
#define AP_BIT 0x01
#define RE_BIT 0x02
#define AOS_BIT 0x03
#define WMDF_BIT 0x04
#define WDF_BIT 0x05
#define AF_BIT 0x06


void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int event_count = getEventCount();

  Serial.println(event_count);
  delay(1000);
}

unsigned int getEventCount()
{
  Wire.beginTransmission(DS1682_ADDR);
  Wire.write(EVENT_COUNTER);      
  Wire.requestFrom(DS1682_ADDR,2);   // Read both bytes
  byte LSB = Wire.read();        // Read LSB
  byte MSB = Wire.read();        // Read MSB
  Wire.endTransmission();        // Finish transmission

  unsigned int event_count = MSB << 8 | LSB;

  return event_count;
}

void resetAll()
{
  
}

