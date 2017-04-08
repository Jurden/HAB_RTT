//
#include <SoftwareSerial.h>

#define SERIALBAUD  9600

void setup()
{
  Serial.begin(SERIALBAUD);
  Serial.println("Starting");
  gps_init();
}

void loop()
{
  double temperature;
  int err;

  err = getTempF(&temperature);
  if (err) {
    Serial.println("TEMP GET ERROR");
  } else {
    Serial.println(temperature);
    
  }
}
