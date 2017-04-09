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
  char gps;

  err = getTempF(&temperature);
  if (err) {
    Serial.println("TEMP GET ERROR");
  } else {
    Serial.println(temperature);
    
  }
  err = getGPS(&gps);
  if (!err) {
    Serial.println(gps);
  }

  delay(1000);
  
}
