// MAX31855 Thermocouple sensor

#include <SPI.h>
#include "Adafruit_MAX31855.h"

// Arduino pins
#define MAXDO 	12
#define MAXCS 	10
#define MAXCLK	13

#define READATTEMPTS	3

// initialize the Thermocouple
Adafruit_MAX31855 thermocouple(MAXCLK, MAXCS, MAXDO);


// Read temperature C
int getTempC(double *temp)
{
  int i;
  double c;

  i = 0;

  c = thermocouple.readCelsius();
  while(isnan(c)) {
    delay(500);
    c = thermocouple.readCelsius();
    
    i++;
    if (i == READATTEMPTS) {
      temp = NULL;
      return -1;
    }
  }
  *temp = c;
  return 0;
}

// Read temperature F
int getTempF(double *temp)
{
  int i;
  double c;

  i = 0;

  c = thermocouple.readFarenheit();
  while(isnan(c)) {
    delay(500);
    c = thermocouple.readFarenheit();
    
    i++;
    if (i == READATTEMPTS) {
      temp = NULL;
      return -1;
    }
  }
  *temp = c;
  return 0;
}
