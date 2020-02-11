// Date and time functions using just software, based on millis() & timer
//Funktioniert über die i2c Schnittstelle:
//Arduino Nano und Uno haben dafür die Pins A4 und A5 reserviert
//A4 wird mit MISO bzw. I/O oder SDA vom RTC verkabelt (Die Bezeichnung variiert je nach Hersteller)
//A5 wird mit SCK bw. SCLK oder SDL vom RTC verkabelt (auch hier varriert )
//D2 kommt an den Input vom Relais


#include <Wire.h>
#include "RTClib.h"

RTC_Millis rtc;
int Ausgang = 2;


void setup () {
    Serial.begin(57600);
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.begin(DateTime(__DATE__, __TIME__));
    pinMode(Ausgang, OUTPUT); 
}

void loop () {
    DateTime now = rtc.now();
    //Licht ist an zwischen 8-18uhr
if ( now.hour() >= 8 && now.hour() < 18) {
digitalWrite(Ausgang, HIGH);
Serial.print("An");
}
else
{
digitalWrite(Ausgang, LOW);
Serial.print("Aus");
}
 
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    Serial.println(now.unixtime());
    Serial.println();
    delay(3000);
}
