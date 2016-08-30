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
  // Set-up communications to DS1682 and serial console
  Wire.begin();
  Serial.begin(9600);

  // Reset event count
  resetEvents();
}

void loop() {
  unsigned int event_count = getEventCount();
  //Print event count every second
  Serial.println(event_count);
  delay(1000);
}

/* Return total number of events since last reset events*/
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

/* Zero event registers to restart counts */
void resetEvents()
{
  set_register(DS1682_ADDR,EVENT_COUNTER,0);
  delay(10);
  set_register(DS1682_ADDR,EVENT_COUNTER+1,0);
  delay(10);
}

/*******************************************/
/* This global reset zeros all registers   */
/* and makes config read only, CAUTION !!!!*/
/*******************************************/
void resetAll()
{
  Wire.beginTransmission(DS1682_ADDR);
  Wire.write(CONFIG_REG);
  Wire.requestFrom(DS1682_ADDR,1);   // Read CONFIG Reg
  byte config = Wire.read();
  Wire.endTransmission();
  
  byte config_re = config | 1 << RE_BIT;
  set_register(DS1682_ADDR,CONFIG_REG,config_re);
  delay(10);
  set_register(DS1682_ADDR,RST_REG,0x55);
  delay(10);
  set_register(DS1682_ADDR,RST_REG,0x55);
  delay(10);
}

void set_register(byte address, unsigned char r, unsigned char v) {
  Wire.beginTransmission(address);
  Wire.write(r);
  Wire.write(v);
  Wire.endTransmission();
}

