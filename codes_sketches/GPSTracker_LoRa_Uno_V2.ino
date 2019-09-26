//  Sketch für einen GPS-Tracker mit LoRaWAN
//  
//  Der Tracker sendet nur dann, wenn er gültige GPS-Daten hat und wenn diese sich nicht nennenswert geändert haben.
//  
//  Ausführlich kommentiert, da der Sketch für Neueinsteiger-Workshops gedacht ist
//
//  Dieser Code, ein weiteres Beispiel für einen (wenstlich einfacheren) Sensor für Anfänger und eine Präsentation
//  für LoRaWAN-Workshops sind im github der Technologiestiftung hinterlegt: https://github.com/technologiestiftung/workshops 
//  
//  
//  Hardware:
//  Das Beispiel ist für den Arduino Uno (!).
//  
//  Doku zum Shield: https://wiki.dragino.com/index.php?title=Lora/GPS_Shield 
//      Hinweis: Wenn der Arduino stromlos war, muss der GPS-Empfänger sich den kompletten Almanach vom Satelliten holen. 
//      Das dauert selbst bei gutem Empfang eine reichliche Viertelstunde! (Mobiltelefone holen den aus dem Internet und sind deshalb sehr viel schneller.)
//  
//  Auf dem Arduino Uno müssen die Jumper bei GPSRx und Tx auf dem LoRaWANShield entfernt werden.
//  GPS-Rx muss mit Pin D4 und GPS-Tx muss mit Pin D3 verdrahtet werden, damit das GPS per SoftwareSerial ausgelesen werden kann: 
//  https://wiki.dragino.com/index.php?title=File:GPS_connection_new.png
//  So lange diese Verdrahtung noch nicht erfolgt ist, muss beim Hochladen eines Sketches das Knöpfchen auf dem Shield gedrückt sein.
// 
//      Hinweis für den Arduuino Mega: 
//      Auf dem Arduino Mega kann dessen Hardware-Serial (auf PIN 18 und 19.) genutzt werden.
//      Das GPS-Shield muss entsprechend verdrahtet werden (Rx an D18, Tx an D19). 
//      SoftwareSerial-Aufrufe wie ss.read() sind dann durch Serial1.read() zu ersetzen, die SoftwareSerial Library ist unnötig.
//      Eine Pin-Definition für Hardware-Serial entfällt auch, da das bereits auf dem Board so verdrahtet ist.
//
//      genereller Hinweis / ACHTUNG:
//      Der Sketch funktioniert mit Original-Arduinos oder Nachbauten, die den selben seriellen Chip (Atmel Mega 18u2) haben.
//      Auf Klonen mit anderen seriellen Chips, namentlich dem CH340 kann es passieren, dass die serielle Komunikation so nicht funktioniert.
//      
//  
//  
//  
//  Libraries
//  Verwendete Libraries sind über die Arduino-IDE verfügbar. 
//  Dokumentation u.a. direkt in den Bibliotheksordnern.
//  
//  LMIC: 
//  Die Library IBM LMIC (Ver.1.5) findet sich in der Arduino-Version von Matthijs Kooijman hier: https://github.com/matthijskooijman/arduino-lmic. 
//  LMIC steht unter der (relativ) offenen Lizenz Eclipse Public License v1.0. Doku im Bibliotheksordner, Tipps hier: https://www.thethingsnetwork.org/forum/c/nodes/lmic
//  
//  Hal: 
//  ist Bestandteil der LMIC
//  
//  TinyGPS:
//  Die Library von Mikal Hart findet sich hier: https://github.com/mikalhart/TinyGPS/releases/tag/v13, Anleitung hier: http://arduiniana.org/libraries/tinygps/
//  
//  SoftwareSerial: 
//  Mit der IDE mitgeliefert (darüber komuniziert das GPS mit dem Arduino)
//
//  SPI:
//  Mit der IDE mitgeliefert (darüber komuniziert das LoRa-Modul mit dem Arduino)
//  
//  
//  
//  Code-Quellen:
//  
//  Code, der die LMIC anspricht, stammt weitgehend aus dem ABP-Beispiel von Thomas Telkamp and Matthijs Kooijman, das mit der LMIC ausgeliefert wird. 
//  Modifikationen (Senden bei Bedarf statt zeitgesteuert) sind von mir und folgen diesem Hinweis von Jac Kersing im TTN-Forum: 
//  https://www.thethingsnetwork.org/forum/t/sending-asynchrously-uplink-data/14275
//  
//  Die smartdelay-Funktion, die das GPS ausliest, stammt aus einem Beispiel von Edwin Chen von Dragino, das GPS-Daten seriell ausgibt: 
//  https://github.com/dragino/Arduino-Profile-Examples/blob/master/libraries/Dragino/examples/GPS/tinygps_example/tinygps_example.ino 
//
//  Der Javascript-Code für den TTN-payload-Decoder (ganz unten) ist angelehnt an den Decoder von Sebastian Rauer für seinen mobilen
//  Feinstaubsensor: https://github.com/technologiestiftung/Sense10
//  
//  Der Rest und die Kommentare stammen von mir.
//  Kommentare und Quellen sind ausführlich, da der Code für einen Anfänger-Workshop gedacht ist.
//  
//  Christian Hammel, Technologiestiftung Berlin, 2019
// 
//
// 
//  Nice to have:
//  - Port auf aktuellere LMIC-Versionen?
//  - Stromsparfunktionen? https://github.com/matthijskooijman/arduino-lmic/issues/76





#include <lmic.h>
#include <hal/hal.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <SPI.h>

TinyGPS gps;
SoftwareSerial ss(3, 4); // Arduino TX, RX ,  // Pinmapping für Softwareserial, das heißt, dass man das Tx vom GPS mit (digitalem) Pin3 und das Rx vom GPS mit Pin 4 verdrahten muss.



// FÜR JEDEN NODE NEU AUSFÜLLEN 
//***************************************
// In der Downloadversion stehen hier ungültige Schlüssel.
static u1_t NWKSKEY[16] = { 0xCC, 0xCC, 0xCC, 0xCC, 0x59, 0x87, 0xFC, 0xDD, 0x80, 0xB6, 0xA0, 0xF6, 0xDE, 0xFB, 0xCC, 0xCC };  //  <<=== eintragen  Den Wert bekomt Ihr aus der TTN-Console, einzutragen ist das Format msb!
static u1_t APPSKEY[16] = { 0xCC, 0xCC, 0xCC, 0xCC, 0x6E, 0xCA, 0xC3, 0x10, 0x73, 0x8F, 0x0A, 0x3B, 0x67, 0xBA, 0xCC, 0xCC };  //  <<=== eintragen  Den Wert bekomt Ihr aus der TTN-Console, einzutragen ist das Format msb!

// ACHTUNG DEVICE ADRESSE MIT PRÄFIX 0x
static const u4_t DEVADDR = 0X260C1C6C;  //  <<=== eintragen  Den Wert bekomt Ihr aus der TTN-Console, einzutragen ist das Format im hex-Style!

const unsigned long senderate = 60; //   <<=== eintragen Sendeintervall in Sekunden (ganzzahlig). Spielregeln zur Airtime beachten! 

                                 // Frequenzzuweisung: 1% Airtime, TTN fair-use: 30 s downlink pro Tag und Node, 10 uplinks pro Tag)
                                 // Der Wert ist Basis für den Loop-Timer ('interval').
                                 // Das "echte" Sendeintervall ist dann interval + Durchlaufzeiten des Rest-Programmes (was vergleichsweise sehr schnell geht).
                                 // Beim aktuellen Sketch mit 14byte-Message liegt die Airtime bei SF7 bei 67 ms. Mit einer Frequenz von alle 200s und einem Kanal wäre fair use erreicht.
                                 // Im aktuellen Sketch mit 14byte-Message vergehen um 3,5 Sekunden zwischen "Sendepuffer schreiben" und "gesendet". 

const long bewegung = 12;  //  <<=== eintragen: Entfernung in Metern (ganzzahlig) von letzten Ort, ab der gesendet wird
                                    // 0.000010 Dezimalgrad (liefert das GPS) ist ungefähr = 1 m (ungefähr, weil das Gradnetz über einer Kugel liegt, die obendrein nicht ganz rund ist)
                                    // Mit wenig Statlliten ist die Position oft nicht genauer als 10-15 Meter, Scheingenauigkeit nützt hier also wenig.

//****************************************





// Daten für Zähler und Zeiten

// Da wir keinen zeitgesteuerten Aufruf der LMIC nutzen, steuern wir die LoRa-Tx-Intervalle über die Hauptschleife (das ist sogar genauer als via LMIC-Timer).
// Die Steuerung der LMIC über fixe Intervalle nutzen wir nicht, da wir nur dann senden wollen, wenn wir valide Daten haben und wenn diese sich ändern. 
// Das spart nicht nur ein bischen Strom und Airtime, sondern sorgt dafür, dass auch nur "richtige" Daten in Frontends wie dem TTN-Mapper ankommen.

static void smartdelay(unsigned long ms);
unsigned long ms = 1000; //  Auslesezeit der smartdelayfunktion für das GPS in Millisekunden, nur Einträge wenn Tests auf valide GPS-Daten nicht funktionieren 
                         // (Erklärung: Unmengen serieller Zeichen auszulesen braucht Zeit, GPS-Daten sollten einmal pro Sekunde kommen, also ist 1000 gut)
unsigned long previousmillis = 0;    //nichts eintragen, wird für den Loop-Timer benötigt
unsigned long interval = senderate * 1000;     //nichts eintragen, wird für den Loop-Timer benötigt


// Variablen für die GPS-Daten
// noch mehr Möglichkeiten sind in der Doku zur TinyGPS erklärt: http://arduiniana.org/libraries/tinygps/
float flat, flon, falt;
unsigned long age, fix_age, date, time, hdop, chars = 0;
unsigned long lati, longi, alti;
unsigned int sats = 0;
unsigned short sentences, failed_checksum;
long lat, lon;
long previouslati = 0;
long previouslongi = 0;
long praezision = bewegung * 10L;  //da wir mit unsigned longs rechnen, statt mit Dezimalzahlen, müssen wir mit 10 multiplizieren und nicht durch 100000 teilen!

// Dieses Array enthält später die zu sendenden Daten, wer mehr senden will, macht es größer und belädt es dann entsprechend
// Welche GPS-Werte man noch ausgeben könnte, steht in der Doku von TinyGPS und im Beispiel von Dragino, das diese seriell ausgibt.
uint8_t message[14];

// Ab hier Konfigurationen für LMIC / LoRaWAN

// These callbacks are only used in over-the-air activation, so they are
// left empty here (we cannot leave them out completely unless
// DISABLE_JOIN is set in config.h, otherwise the linker will complain).
void os_getArtEui (u1_t* buf) { }
void os_getDevEui (u1_t* buf) { }
void os_getDevKey (u1_t* buf) { }

static osjob_t sendjob;

const lmic_pinmap lmic_pins = {
    .nss = 10,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 9,
    .dio = {2, 6, 7},
    };



void setup() {
   Serial.begin(9600);
   Serial.println(F("Starting..."));
   ss.begin(9600); // SoftSerial port to get GPS data. 

// Auskommentiert: für Debug-Zwecke, damit man ein Paket aus der initialen Sendung (sinnfreier) Daten im Setup von
// anderen Paketen unterscheiden kann. Schreibt die Message voller 0F (sonst wäre sie voller = 00)
//      message[0] = (0x0F);
//      message[1] = (0x0F);
//      message[2] = (0x0F);
//      message[3] = (0x0F);
//      message[4] = (0x0F);
//      message[5] = (0x0F);
//      message[6] = (0x0F);
//      message[7] = (0x0F);
//      message[8] = (0x0F);
//      message[9] = (0x0F);
//      message[10] = (0x0F);
//      message[11] = (0x0F);
//      message[12] = (0x0F);
//      message[13] = (0x0F);
    

// LMIC init
   os_init();

// Reset the MAC state. Session and pending data transfers will be discarded.
   LMIC_reset();

// Set static session parameters.
   LMIC_setSession (0x1, DEVADDR, NWKSKEY, APPSKEY);

// Set up the channels used by the Things Network, which corresponds
// to the defaults of most gateways. Without this, only three base
// channels from the LoRaWAN specification are used, which certainly
// works, so it is good for debugging, but can overload those
// frequencies, so be sure to configure the full frequency range of
// your network here (unless your network autoconfigures them).
// Setting up channels should happen after LMIC_setSession, as that
// configures the minimal channel set.
   LMIC_setupChannel(0, 868100000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
   LMIC_setupChannel(1, 868300000, DR_RANGE_MAP(DR_SF12, DR_SF7B), BAND_CENTI);      // g-band
   LMIC_setupChannel(2, 868500000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
   LMIC_setupChannel(3, 867100000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
   LMIC_setupChannel(4, 867300000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
   LMIC_setupChannel(5, 867500000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
   LMIC_setupChannel(6, 867700000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
   LMIC_setupChannel(7, 867900000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
   LMIC_setupChannel(8, 868800000, DR_RANGE_MAP(DR_FSK,  DR_FSK),  BAND_MILLI);      // g2-band
// TTN defines an additional channel at 869.525Mhz using SF9 for class B
// devices' ping slots. LMIC does not have an easy way to define set this
 // frequency and support for class B is spotty and untested, so this
// frequency is not configured here.

// Disable link check validation
// Das lasst Ihr bitte disabled, sonst verbraucht Ihr einen Downlink pro Uplink. 
// Wenn es benötig wird, finden sich im TTN-Forum elegantere Alternativen
  LMIC_setLinkCheckMode(0);

// TTN uses SF9 for its RX2 window.
  LMIC.dn2Dr = DR_SF9;

// Set data rate and transmit power for uplink (note: txpow seems to be ignored by the library)
// Das sollte auch so bleiben! Datenraten > 7 brauchen mehr Airtime. Wer hier spielt, sollte
// die Airtime-Regeln kennen.
  LMIC_setDrTxpow(DR_SF7,14);

// Start job
   do_send(&sendjob);  //zwingend
//   Serial.println(F("Setup beendet"));
}





void loop() 
{
unsigned long currentmillis = millis();

//Dieser Serial-Block ist hilfreich, um das loop-Timing auf die EV_TXCOMPLETE-Intervalle abzustimmen, kann in der Produktivvariante weg.
//Serial.println(F("Looping..., "));
//Serial.print("currentmillis: ");  
//Serial.print(currentmillis);
//Serial.print(", currentmillis - previousmillis: ");
//Serial.print(currentmillis - previousmillis);
//Serial.print(", interval: ");
//Serial.print(interval);
//Serial.print(" ");
    
//Die folgende if - Abfrage sorgt für ein non-blocking delay (einfaches delay() würde Senden und GPS-Auslesung blockiern) und loopt ohne Senden bis das Timing-Intervall durch ist.
    if (currentmillis - previousmillis > interval)
    {
        //Die folgende if-Abfrage prüft, ob der letzte Datensatz gesendet wurde und damit Platz im Sendepuffer ist 
        //(ansonsten muss man weder etwas in den Puffer schreiben noch senden)
        if (EV_TXCOMPLETE)
        {
          smartdelay(ms);  //die smartdelay-Funktion liest das GPS aus und speichert es in ein Objet, aus dem wir dann abrufen
          
          //Hier folgt dann GPS-Auswertung mit weiteren if-Konstruktionen, die auf valide Daten prüfen (ohne valide Daten muss nicht gesendet werden)          
          //Serial.println(F("GPS auswerten")); 
          gps.f_get_position(&flat, &flon, &age);
          falt = gps.f_altitude();
          sats = gps.satellites();
          lati = flat * 1000000;
          longi = flon * 1000000;
          alti = falt * 1000000;
          //Wir zeigen uns das seriell *1 Mio an, weil wir diese Werte später auch senden und erst im TTN wieder auf die richtigen Kommastellen bringen
          Serial.print(F("sats: "));   //Das (F()) legt die Strings in den Programmspeicher, das spart Arbeitsspeicher, falls wir das ganze noch erweitern wollen (Auslastung zeigt die IDE nach dem Kompilieren an).
          Serial.print(sats);
          Serial.print(F(", longi:"));
          Serial.print(longi);
          Serial.print(F(", lati:"));
          Serial.print(lati);
          Serial.print(F(", alti:"));
          Serial.println(alti);

              if (sats <= 3) 
              {
              Serial.println(F("kein Fix"));
              Serial.print("sats: ");
              Serial.println(sats);
              //Den Wartezeitzähler müssen wir natürlich auch dann zurücksetzen, wenn mangels Fix nicht gesendet wurde  
              previousmillis = currentmillis;
              Serial.println(F("Zaehler zurueckgesetzt, kein Tx gesetzt"));
              longi = 0;  //Das sorgt dafür, dass die "hat er sich bewegt-Prüfung" in der nächsten Schleife auch dann positiv ausfällt, wenn zwischenzeitlich kein Fix vorhanden war aber noch alte longi-Werte da sind.
              }
              else if (sats == 255) 
              {
              Serial.println(F("kein Fix"));
              Serial.print("sats: ");
              Serial.println(sats);
              //Den Wartezeitzähler müssen wir natürlich auch dann zurücksetzen, wenn mangels Fix nicht gesendet wurde  
              previousmillis = currentmillis;
              Serial.println(F("Zaehler zurueckgesetzt, kein Tx gesetzt"));
              longi = 0; //Das sorgt dafür, dass die "hat er sich bewegt-Prüfung" in der Folgeschleife auch dann positiv ausfällt, wenn zwischenzeitlich kein Fix vorhanden war aber noch alte longi-Werte da sind.
              }
              else  
              {
                    if(long(lati) - previouslati > praezision || previouslati - long(lati) > praezision || long(longi) - previouslongi > praezision || previouslongi - long(longi) > praezision)  
                    {
                   // || bedeutet ODER
                   // Warum 4 Abfragen? Wenn Wert und previous sich unterscheiden, wird je eine der Subtraktionen negativ, die andere positiv. 
                   // Wenn die positive über unserer Schwelle liegt, reicht das als Auslöser aus. Nutzung der abs() Funktion wäre deutlich umständlicher.
                   // Warum Umwandlung in long()? So wird eine der Differenzen sicher negativ. Bliebe man bei unsigned long, kann einen Überlauf
                   // der Variablen entstehen, der dann einen sher großen positiven Wert ergibt.
                   //
                   // wenn das Mote weiter als festgelegt bewegt wurde, dann....
                  //  prepare upstream data                
              //        Auskommentiert: Wenn das GPS nicht mag, müssen wir anders ein paar Daten erzeugen, die sich ändern, 
              //        message[0] = (currentmillis & 0xFF000000) >> 24;
              //        message[1] = (currentmillis & 0x00FF0000) >> 16;
              //        message[2] = (currentmillis & 0x0000FF00) >> 8;
              //        message[3] = (currentmillis & 0x000000FF);
                      message[0] = (lati & 0xFF000000) >> 24;
                      message[1] = (lati & 0x00FF0000) >> 16;
                      message[2] = (lati & 0x0000FF00) >> 8;
                      message[3] = (lati & 0x000000FF);
                      message[4] = (longi & 0xFF000000) >> 24;
                      message[5] = (longi & 0x00FF0000) >> 16;
                      message[6] = (longi & 0x0000FF00) >> 8;
                      message[7] = (longi & 0x000000FF);
                      message[8] = (alti & 0xFF000000) >> 24;
                      message[9] = (alti & 0x00FF0000) >> 16;
                      message[10] = (alti & 0x0000FF00) >> 8;
                      message[11] = (alti & 0x000000FF);
                      message[12] = (sats & 0xFF00) >> 8;
                      message[13] = (sats & 0x00FF);
                      //Serial.println(F("Sendjob aufrufen"));
                      do_send(&sendjob);
                      //Serial.println(F("zurueck von sendjob"));
                      previousmillis = currentmillis;
                      //Serial.println(F("Zaehler zurueckgesetzt, Tx gesetzt")); 
//                      Serial.print("lati-previous: ");
//                      Serial.print(abs(long(lati)-previouslati));
//                      Serial.print(", longi-previous: ");
//                      Serial.print(abs(long(longi)-previouslongi));
//                      Serial.print(", praezision: ");
//                      Serial.println(praezision);
//                      Serial.print("previous-lati: ");
//                      Serial.print(abs(previouslati-long(lati)));
//                      Serial.print(", previous-longi: ");
//                      Serial.print(abs(previouslongi-long(longi)));
//                      Serial.print(", praezision: ");
//                      Serial.println(praezision);
                    }
                    else 
                    {
                      //Den Wartezeitzähler müssen wir natürlich auch dann zurücksetzen, wenn mangels Bewegung nicht gesendet wurde  
                      previousmillis = currentmillis;
                      Serial.println(F("nicht bewegt, Zaehler zurueckgesetzt, kein Tx gesetzt"));
//                      Serial.print("lati-previous: ");
//                      Serial.print(abs(long(lati)-previouslati));
//                      Serial.print(", longi-previous: ");
//                      Serial.print(abs(long(longi)-previouslongi));
//                      Serial.print(", praezision: ");
//                      Serial.println(praezision);
//                      Serial.print("previous-lati: ");
//                      Serial.print(abs(previouslati-long(lati)));
//                      Serial.print(", previous-longi: ");
//                      Serial.print(abs(previouslongi-long(longi)));
//                      Serial.print(", praezision: ");
//                      Serial.println(praezision);
                    }
              }        
        }
        else 
        {
          //Den Wartezeitzähler müssen wir natürlich auch dann zurücksetzen, wenn mangels TXCOMPLETE nicht gesendet wurde  
          previousmillis = currentmillis;
          Serial.println(F("kein TXCOMPLETE, Zaehler zurueckgesetzt, kein Tx gesetzt"));
        }
    }
    else
    {
    // Wenn der Sendeintervallzähler nicht erreicht ist, mache nichts, sondern loope weiter bis der er erreicht ist
    } 

os_runloop_once();  //Das ist der job-scheduler, der periodisch aufzurufen ist. Den Scheduler müssen wir in jeder Schleife machen, sonst läuft LMIC auch dann nicht, wenn der Tx-Puffer gefüllt wird (und unser Sketch sendet nie, weil es seine Nachricht nicht los wird)
//Serial.println(F("Loop durchgelaufen")); 
// befüllen von previous mit den letzen Werten, damit wir in der nächsten Schleife etwas zu vergleichen haben.
previouslati = long(lati);
previouslongi = long(longi);
}





void do_send(osjob_t* j)
{

 // Check if there is not a current TX/RX job running
    if (LMIC.opmode & OP_TXRXPEND) 
      {
      //"Eigentlich" sollte der Puffer nicht voll sein, da wir ja nur bei TXComplete aufrufen. Timing-Probleme können aber bewirken, dass das trotzdem auftritt, deshalb prüfen wir hier.
      Serial.println(F("OP_TXRXPEND, not sending"));
      } 
    else 
      {
//      LMIC_setDrTxpow(DR_SF7,14);
      LMIC_setTxData2(1, message, sizeof(message), 0);  //Schreibt die message byteweise in den Sendepuffer und LMIC sendet dann "automatisch" zum nächsten möglichen Zeitpunkt
      Serial.println("TxData2 gesetzt");
      // Next TX is scheduled after TX_COMPLETE event.
      }
}




static void smartdelay(unsigned long ms) 
{
unsigned long start = millis();   
//  Auskommentierte helfen bei Debuggen der Schleife und der Zeiten
//  Serial.print("smartdelay ms:");  
//  Serial.println(ms);
//  Serial.print("start: ");  
//  Serial.println(start);
//  Serial.print("millis - start: ");  
//  Serial.println(millis()-start);
 
do 
  {
      while (ss.available())   // so lange ss (also das GPS) Daten im Puffer zur Verfügung stellt
        {
          // Die beiden folgenden auskommentierten werfen die seriellen Rohdaten also die GPS-Datensätze direkt auf den seriellen Monitor
          // Nutzung zur Kontrolle, ob überhaupt Datensätze kommen. Zweimal ss.read hintereinander füttert gps.encode aber mit falschen Daten
          // (ss.read leert beim Lesen den Puffer - man kann ihn nicht zweimal lesen!). Deshalb müssen die beiden Zeilen außerhalb von 
          // Fehlersuchen wieder auskommentiert werden. Während einer Ausgabe auf den seriellen Monitor muss gps.encode auskommentiert
          // werden, sonst kommen auch hier keine validen Werte an.
          //  char ausgabe = ss.read();
          //  Serial.print(ausgabe);
          gps.encode(ss.read());  //    <<=== befülle hier das gps-Objekt mit den Daten, die wir später abrufen!
        }
  } while (millis() - start < ms); while ((millis() - start) < ms);
      // so lange die Wartezeit in ms ("Jetzt" abzüglich "Startzeit") kleiner als die Soll-Wartezeit (ms) ist, mache was in der inneren Schleife steht, ansonsten gehe weiter.

//    Die nachstehend auskommentierten Befehle dienen nur zur Kontrolle, ob innerhalb von smartdelay richtige Daten im gps.encode-Obkekt ankommmen und zur Kontrolle
//    der Funktions-Laufzeit, wenn erforderlich. Wenn im Haupt-Loop ankommt, was erwartet wird, wird die Information nicht benötigt.
//    gps.stats(&chars, &sentences, &failed_checksum);
//    Serial.print("chars aus smartdelay: ");
//    Serial.println(chars);
//    Serial.print("sentences aus smartdelay: ");
//    Serial.println(sentences);
//    Serial.print("failed_checksum aus smartdelay: ");
//    Serial.println(failed_checksum);
//    gps.get_position(&lat, &lon, &fix_age);
//    Serial.print("lat aus smartdelay: ");
//    Serial.println(lat);
//    Serial.print("lon aus smartdelay: ");
//    Serial.println(lon);
//    Serial.print("fix_age aus smartdelay: ");
//    Serial.println(fix_age);
//
//    Serial.print("millis : ");  
//    Serial.println(millis());
//    Serial.print("millis - start: ");  
//    Serial.println(millis()-start);
}



void onEvent (ev_t ev) {
// da 6 Zeilen tiefer der sendjob-Aufruf über den LMIC-Timer auskommentiert ist, den wir sonst benutzen, ist strenggenommen die ganze Funktion überflüssig.
// Wir lassen sie stehen, da sie kaum Platz braucht, zeigt wie man die Event-Meldungen der LMIC nutzt und weil wir den seriellen Output gut gebrauchen können,
// um zu sehen, ob wirklich gesendet wurde und ob das Timing stimmt.
    if (ev == EV_TXCOMPLETE){
    Serial.print(F("Arduino-Time: "));
    Serial.print(os_getTime());
    Serial.print(F(": "));
    Serial.println(F("EV_TXCOMPLETE (includes waiting for RX windows)"));
        // Schedule next transmission
//       os_setTimedCallback(&sendjob, os_getTime()+sec2osticks(TX_INTERVAL), do_send);
        // der Befehl in der Zeile oberhalb, der hier als Kommentar steht, ist gelöscht. Er ruft normalerweise die zeitgesteuerte LMIC-Ausführung auf, die wir hier nicht nutzen. 
        // nur auskommentiert, damit der Unterschied zu vielen Muster-Sketches nachvollziehbar ist. Da do_send hier auskommentiert ist, wird es im Hauptloop "händisch" aufgerufen.
    }
}


// Die nachstehende Funktion wird nicht benutzt, kann aber zum Fehlersuchen hilfreich sein
//void onEvent (ev_t ev) {
//    Serial.print("Arduino-Time: ");
//    Serial.print(os_getTime());
//    Serial.print(": ");
//    switch(ev) {
//        case EV_SCAN_TIMEOUT:
//            Serial.println(F("EV_SCAN_TIMEOUT"));
//            break;
//        case EV_BEACON_FOUND:
//            Serial.println(F("EV_BEACON_FOUND"));
//            break;
//        case EV_BEACON_MISSED:
//            Serial.println(F("EV_BEACON_MISSED"));
//            break;
//        case EV_BEACON_TRACKED:
//            Serial.println(F("EV_BEACON_TRACKED"));
//            break;
//        case EV_JOINING:
//            Serial.println(F("EV_JOINING"));
//            break;
//        case EV_JOINED:
//            Serial.println(F("EV_JOINED"));
//            break;
//        case EV_RFU1:
//            Serial.println(F("EV_RFU1"));
//            break;
//        case EV_JOIN_FAILED:
//            Serial.println(F("EV_JOIN_FAILED"));
//            break;
//        case EV_REJOIN_FAILED:
//            Serial.println(F("EV_REJOIN_FAILED"));
//            break;
//        case EV_TXCOMPLETE:
//            Serial.println(F("EV_TXCOMPLETE (includes waiting for RX windows)"));
//           
//            // Schedule next transmission
//            //os_setTimedCallback(&sendjob, os_getTime()+sec2osticks(TX_INTERVAL), do_send);
//            break;
//        case EV_LOST_TSYNC:
//            Serial.println(F("EV_LOST_TSYNC"));
//            break;
//        case EV_RESET:
//            Serial.println(F("EV_RESET"));
//            break;
//        case EV_RXCOMPLETE:
//            // data received in ping slot
//            Serial.println(F("EV_RXCOMPLETE"));
//            break;
//        case EV_LINK_DEAD:
//            Serial.println(F("EV_LINK_DEAD"));
//            break;
//        case EV_LINK_ALIVE:
//            Serial.println(F("EV_LINK_ALIVE"));
//            break;
//         default:
//            Serial.println(F("Unknown event"));
//            break;
//    }
//}


//*****************************************************************************************
//****** Ab hier Decoderfunktion für TTN **************************************************
// **** Alles ab hier ist Javascript. Die Arduino-IDE versteht das nicht. *****************
//***** Deshalb nicht entkommentieren ****************************************************
//***** Zweck: Das könnt ihr als payload-Decoder in der TTN-Konsole verwenden.************
//
//     function Decoder(bytes, port) {
//       // Decode an uplink message from a buffer
//       // (array) of bytes to an object of fields.
//     var decoded = {};
//     decoded.latitude = ((bytes[0] << 24) + (bytes[1] << 16) + (bytes[2] << 8) + (bytes[3])) / 1000000;
//     decoded.longitude = ((bytes[4] << 24) + (bytes[5] << 16) + (bytes[6] << 8) + (bytes[7])) / 1000000;
//     decoded.altitude = ((bytes[8] << 24) + (bytes[9] << 16) + (bytes[10] << 8) + (bytes[11])) / 1000000;
//     decoded.sats = (bytes[12] << 8) + (bytes[13]);
  
//       // if (port === 1) decoded.led = bytes[0];

//       return decoded;
//     }
//
//'***************************************************************************************
