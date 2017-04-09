
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(3, 2);
Adafruit_GPS GPS(&mySerial);

#define GPSECHO  false

boolean usingInterrupt = true;
void useInterrupt(boolean);


void gps_init()
{
	// 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(9600);

	// uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // Set the update rate
	GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
	// Request updates on antenna status, comment out to keep quiet
  GPS.sendCommand(PGCMD_ANTENNA);

  useInterrupt(true);
  delay(1000);
}

// Interrupt is called once a millisecond, looks for any new GPS data, and stores it
SIGNAL(TIMER0_COMPA_vect) {
 	char c = GPS.read();
 	// if you want to debug, this is a good time to do it!
#ifdef UDR0
	if (GPSECHO)
		if (c) UDR0 = c;  
    	// writing direct to UDR0 is much much faster than Serial.print 
    	// but only one character can be written at a time. 
#endif
}

void useInterrupt(boolean v) {
  	if (v) {
    	// Timer0 is already used for millis() - we'll just interrupt somewhere
    	// in the middle and call the "Compare A" function above
    	OCR0A = 0xAF;
    	TIMSK0 |= _BV(OCIE0A);
    	usingInterrupt = true;
  	} else {
    	// do not call the interrupt function COMPA anymore
    	TIMSK0 &= ~_BV(OCIE0A);
    	usingInterrupt = false;
  	}
}
uint32_t timer = millis();

int getGPS(char *c)
{
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.fix) {
	  *c = GPS.read();
    Serial.println(GPS.fix);
    return 0;
  } else {
    Serial.println("No Fix");
    return 3;
  }
}
