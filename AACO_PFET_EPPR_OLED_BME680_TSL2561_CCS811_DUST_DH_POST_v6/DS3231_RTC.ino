#include <Wire.h>
#include "RTClib.h"
RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void RTC_DS3231_setup()
{
  if (! rtc.begin())
  { DEBUG_Sprintln("Couldn't find RTC"); //    while (1);
  }

  //  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  //  rtc.adjust(DateTime(2020, 2, 12, 16, 25, 30));
  

}

void TimeNow()
{
  DateTime now = rtc.now();

  timeStamp = String(now.year(), DEC); // 2020-01-14T20:25:34 // DEVICE HIVE FORMAT -> UTC TIME EXPECTED
  timeStamp = timeStamp + String("-") + String(now.month(), DEC); // String(":") +
  timeStamp = timeStamp + String("-") + String(now.day(), DEC); // ":" +
  timeStamp = timeStamp + String("T") + String(now.hour(), DEC);
  timeStamp = timeStamp + String(":") + String(now.minute(), DEC);
  timeStamp = timeStamp + String(":") + String(now.second(), DEC); // + String(".000Z")
  //  timeStamp = timeStamp + String(".") + String(now.centisecond(), DEC);

  //  timeStamp = String(now.hour(), DEC); //16.10.34-14-1-2020
  //  timeStamp = timeStamp + String(".") + String(now.minute(), DEC); // String(":") +
  //  timeStamp = timeStamp + String(".") + String(now.second(), DEC); // ":" +
  //  timeStamp = timeStamp + String("-") + String(now.day(), DEC);
  //  timeStamp = timeStamp + String("-") + String(now.month(), DEC);
  //  timeStamp = timeStamp + String("-") + String(now.year(), DEC);

  //  timeStamp = String(now.hour(), DEC); // 16:10:34Hrs-14/1/2020
  //  timeStamp = timeStamp + String(":") + String(now.minute(), DEC); // String(":") +
  //  timeStamp = timeStamp + String(":") + String(now.second(), DEC); // ":" +
  //  timeStamp = timeStamp + String("Hrs-") + String(now.day(), DEC);
  //  timeStamp = timeStamp + String("/") + String(now.month(), DEC);
  //  timeStamp = timeStamp + String("/") + String(now.year(), DEC);

    timeHMS = String(now.hour(), DEC); // 16:10:34Hrs-14/1/2020
    timeHMS = timeHMS + String(":") + String(now.minute(), DEC); // String(":") +
    timeHMS = timeHMS + String(":") + String(now.second(), DEC); // ":" + + String("Hrs-")
    
    Date = String(now.day(), DEC);
    Date = Date + String("-") + String(now.month(), DEC);
    Date = Date + String("-") + String(now.year(), DEC);

  Serial.println(String("\n\tTime Stamp : ") + timeStamp + String("\n"));
  Serial.println(String("\n\tTime : ") + timeHMS + String("\n"));
  Serial.println(String("\n\tDate : ") + Date + String("\n"));
  
}

/*

  if (rtc.lostPower())
  { 
//    DEBUG_Sprintln("RTC lost power, lets set the time!");

//    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // sets the RTC to the date & time this sketch was compiled

    //    rtc.adjust(DateTime(GPSData.YY, GPSData.MM, GPSData.DD, GPSData.hh, GPSData.mm, GPSData.ss));
    //    rtc.adjust(DateTime(2019, 12, 18, 10, 34, 0));
    // to set January 27 2017 at 12:56 you would call:
    // rtc.adjust(DateTime(2017, 1, 27, 12, 56, 0)); // sets the RTC with an explicit date & time
  }
 
  void loop ()
  {
  DateTime now = rtc.now(); // returns current date & time.
  // Its return value is usually stored in the variable of datatype 'DateTime'.

  Serial.println("Current Date & Time: ");
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" (");
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]); // index of a 2D character array
  Serial.print(") ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  Serial.println("Unix Time: ");
  Serial.print("elapsed ");
  Serial.print(now.unixtime());
  Serial.print(" seconds/");
  Serial.print(now.unixtime() / 86400L);
  // unixtime() function returns unix time in seconds
  // since 00:00:00(Coordinated Universal Time – Thursday, 1 January 1970).
  Serial.println(" days since 1/1/1970");

  // calculate a date which is 7 days & 30 seconds into the future
  // TimeSpan(days,hours, minutes, seconds)
  DateTime future (now + TimeSpan(7, 0, 0, 30)); // returns the future time
  //  DateTime past (now - TimeSpan(7, 0, 0, 30)); // returns the past time

  Serial.println("Future Date & Time (Now + 7days & 30s): ");
  Serial.print(future.year(), DEC);
  Serial.print('/');
  Serial.print(future.month(), DEC);
  Serial.print('/');
  Serial.print(future.day(), DEC);
  Serial.print(' ');
  Serial.print(future.hour(), DEC);
  Serial.print(':');
  Serial.print(future.minute(), DEC);
  Serial.print(':');
  Serial.print(future.second(), DEC);
  Serial.println();

  Serial.println();
  delay(1000);
  }
*/
