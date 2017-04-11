#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

#define SEALEVEL 1017.467

Adafruit_BMP280 bmp;

int BMP_init()
{
  if(!bmp.begin()) {
    return -1;
  } else {
    return 0;
  }
}

int getPressure(double *pressure)
{
  *pressure = bmp.readPressure();
  return 0;
}

int getAltitude(double *altitude)
{
  *altitude = bmp.readAltitude(SEALEVEL);
  return 0;
}
