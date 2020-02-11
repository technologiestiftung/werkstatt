 // Wire.h und RTClib.h finden sich in der Arduino IDE oben im „Sketch“-Menü => Bibliothkek einbinden => Bibliotheken verwalten. Wire.h ist standardmäßig installiert, RTClib.h findet sich in den addierbaren Bibliotheken.
// Wer sich dabei unsicher ist, kann hier auf der Arduino-Website nachlesen, wie Libraries zu finden und einzubinden sind.
#include <Wire.h>
#include "RTClib.h"

RTC_Millis rtc;
//Arduino Pin Nummer 10 wird initialisiert 
int Ausgang = 10;

void setup (){
//startet den Seriellen Monitor, um sich die Zeit auf dem Display ausgeben zu lassen
Serial.begin(57600);
//setzt Zeit, Datum auf den Moment, in dem der Sketch hochgeladen wird
rtc.begin(DateTime(__DATE__, __TIME__));
//Arduino Pin Nummer 10 wird als Output definiert und aktiviert über MOSFET oder Relais- karte die Pumpe
pinMode(Ausgang, OUTPUT);
 }
 
void loop () {
DateTime now = rtc.now();
//Die Schleife aktiviert Pin 10, wenn der Stundenwert gleich 5 ist und der Minutenwert kleiner als 15. Um die Laufzeit der Pumpe und so die Wassermenge zu regulieren, kann dieser Wert variiert werden. //danach wird Pin 10 wieder deaktiviert
if ( now.hour() == 5 && now.minute() < 15) digitalWrite(Ausgang, HIGH);
else
digitalWrite(Ausgang, LOW);
//Die folgenden Befehle sind Anzeige der Uhrzeit auf dem Seriellen Monitor zuständigSerial.print(now.hour(), DEC); Serial.print(‚:‘); Serial.print(now.minute(), DEC); Serial.print(‚:‘); Serial.print(now.second(), DEC); Serial.println(); Serial.println(now.unixtime()); Serial.println();
delay(3000);
}
