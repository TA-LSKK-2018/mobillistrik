#include <SoftwareSerial.h>
#include <TimeLib.h>
#include <TinyGPS.h>

/* This sample code demonstrates the normal use of a TinyGPS object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/

TinyGPS gps;
//SoftwareSerial ss(15, 14);

// Offset hours from gps time (UTC)
const int offset = 7;   // Central European 

time_t prevDisplay = 0; // when the digital clock was displayed


    bool connect_gps;
    float flat, flon;
    unsigned long age;
    int Year;
    byte Month, Day, Hour, Minute, Second;

void setup()
{
  Serial.begin(115200);
  Serial1.begin(9600);
}

void loop(){
  lat_and_lon();
  tampilkan_serial_gps();
}

void lat_and_lon (){

  bool newData = false;
  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (Serial1.available())
    {
      char c = Serial1.read();
      //Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }

  if (newData)
  {
    connect_gps = true;
    gps.f_get_position(&flat, &flon);
    gps.crack_datetime(&Year, &Month, &Day, &Hour, &Minute, &Second, NULL, &age);

    if (age < 500) {
        // set the Time to the latest GPS reading
        setTime(Hour, Minute, Second, Day, Month, Year);
        adjustTime(offset * SECS_PER_HOUR);
    }
    
    if (timeStatus()!= timeNotSet) {
      if (now() != prevDisplay) { //update the display only if the time has changed
        prevDisplay = now();
        
      }
    }
  }
  else {
    connect_gps = false;
  }
}

void tampilkan_serial_gps(){
  //if (connect_gps){
        // digital clock display of the time
        Serial.print(hour());
        //printDigits(minute());
        Serial.print(":");
        if(minute() < 10)
        Serial.print('0');
        Serial.print(minute());
        
        //printDigits(second());
        Serial.print(":");
        if(second() < 10)
        Serial.print('0');
        Serial.print(second());
        Serial.print(" ");
        
        Serial.print(day());
        Serial.print("/");
        Serial.print(month());
        Serial.print("/");
        Serial.print(year());
        Serial.print("  ");

        Serial.print("LAT=");
        Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
        Serial.print(" LON=");
        Serial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
        Serial.println();
  //}
  //else{
  //Serial.println("invalid gps");
  //}
}


