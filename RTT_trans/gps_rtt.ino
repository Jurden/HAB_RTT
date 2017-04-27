
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>

//SoftwareSerial mySerial(1, 0);
Adafruit_GPS GPS(&Serial1);

#define GPSECHO  false

boolean usingInterrupt = false;
void useInterrupt(boolean);


void gps_init()
{
	// 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(9600);
  

	// uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  
  // Set the update rate
	GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
	//GPS.sendCommand(PMTK_API_SET_FIX_CTL_1HZ);
	// 5 Hz update rate- for 9600 baud you'll have to set the output to RMC or RMCGGA only (see above)
  //GPS.sendCommand(PMTK_SET_NMEA_UPDATE_5HZ);
  //GPS.sendCommand(PMTK_API_SET_FIX_CTL_5HZ);
  
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

int getGPS(GPS_t *c)
{
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    if (!GPS.parse(GPS.lastNMEA())) {
      return -2;
    }
    // Time data
    c->hour = GPS.hour;
    c->minute = GPS.minute;
    c->seconds = GPS.seconds;
    c->day = GPS.day;
    c->month = GPS.month;
    c->year = GPS.year;

    // Check for gps fix
    if (!GPS.fix) {
  	  Serial.println("NO FIX");
      Serial.println(GPS.fix);
      return -1;
    }

    // Location data
    c->lat = GPS.latitude;
    c->lon = GPS.longitude;
    c->latDeg = GPS.latitudeDegrees;
    c->lonDeg = GPS.longitudeDegrees;
    c->alt = GPS.altitude;
  
    return 0;
  } else {
    Serial.print("NO MESSAGE --");
    return -1;
  }
}
