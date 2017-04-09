//
#include <SoftwareSerial.h>

#define SERIALBAUD  9600

void setup()
{
  int err;
  
  Serial.begin(SERIALBAUD);
  Serial.println("Starting");
  gps_init();
  err = BMP_init();
  if (err) {
    Serial.println("BMP Error");
   // while(1);
  }
  
}

void loop()
{
  double temperature;
  int err;
  double pressure;
  char gps;
  
  // Get Temperature data
  err = getTempF(&temperature);
  if (err) {
    Serial.println("TEMP GET ERROR");
  } else {
    Serial.println(temperature);  
  } 
  // Get GPS data
  err = getGPS(&gps);
  if (!err) {
    Serial.println(gps);
  }
  // Get pressure
  err = getPressure(&pressure);
  if (!err) {
    Serial.println(pressure);
  }

  delay(1000);
  
}
