// Real Time Telemetry data transmitter
#include <SoftwareSerial.h>

#define SERIALBAUD  115200
#define IGNORE_BMP_ALT true

// Structures
// GPS data
typedef struct GPS_t {
  int hour;
  int minute;
  int seconds;
  int day;
  int month;
  int year;
  
  double lat;
  double lon;
  double latDeg;
  double lonDeg;
  double alt;
  int check;
  int err;
} GPS_t;

// Pressure + Altimeter data
typedef struct BMP_t {
  double altitude;
  double pressure;
} BMP_t;

//
typedef struct data_t {
  double temperature;
} data_t;


void setup()
{
  int err;
  
  Serial.begin(SERIALBAUD);
  Serial.println("Starting");
  gps_init();
  err = BMP_init();
  if (err) {
    Serial.println("BMP Error");
    while(1);
  }
  
}

void loop()
{
  int err;
  GPS_t gps;
  BMP_t bmp;
  data_t data;
  
  // Get Temperature data
  err = getTempC(&data.temperature);
  if (err) {
    Serial.println("TEMP GET ERROR");
  } else {
    Serial.print("Temp(C) : ");
    Serial.println(data.temperature);  
  } 
  // Get GPS data
  gps.err = getGPS(&gps);
  if (!gps.err) {
    Serial.print(gps.hour);
    Serial.print(":");
    Serial.print(gps.minute);
    Serial.print(":");
    Serial.println(gps.seconds);
    Serial.print(gps.month);
    Serial.print("-");
    Serial.print(gps.day);
    Serial.print("-");
    Serial.println(gps.year);
    
    Serial.print("Lat : ");
    Serial.println(gps.lat);
    Serial.print("Long : ");
    Serial.println(gps.lon);
    Serial.print("LatDeg : ");
    Serial.println(gps.latDeg);
    Serial.print("LongDeg : ");
    Serial.println(gps.lonDeg);
    if (gps.alt) {
      Serial.print("GPS Alt : ");
      Serial.println(gps.alt);
    }
  // -1 For no GPS message received
  } else if (gps.err == -1) {
    Serial.println("GPS ERROR");
  }
  // Get pressure
  err = getPressure(&bmp.pressure);
  if (!err) {
    Serial.print("Pressure : ");
    Serial.println(bmp.pressure);
  }
  if ((gps.err) && (!IGNORE_BMP_ALT)) {  
    // Get Altitude
    err = getAltitude(&bmp.altitude);
    if (!err) {
      Serial.print("BMP Alt : ");
      Serial.println(bmp.altitude);
    } else {
      Serial.println("NO ALT");
    }
  }
  Serial.println();
  delay(1000);
  
}

