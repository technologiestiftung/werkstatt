 // Beispiel: Temperatur eines Sensors ins TTN senden 
 // benötigt: Arduino, Dragino-Shield (mit Antenne!!) Sensor TMP36 an Analogeingang0 und LED mit Vorwiderstand (!) an PIN 5 (oder Piezolautsprecher)
 // Zum Senden wird die Library LMIC genutzt. (Muss aber über die Arduino-IDE installiert werden, falls das nicht klappt, ist sie hier zu finden:
 // https://github.com/matthijskooijman/arduino-lmic )

// zusätzlich hier enthalten: zur Kontrolle einen Piezolautsprecher oder eine LED einschalten, wenn die Temperatur über 30° liegt, damit man auch
// ohne Laptop mit seriellem Monitor prüfen kann, ob der Aufbau richtig misst. Wenn im TTN lauter "richtige" Werte ankommen, braucht man die nicht.
// Einschalten dieser Funktion: Zeile 272 entkommentieren (alle nötigen Definitionen und Variablen sind im Code uns stören auch bei Nichtbenutzung
// nicht, da der Code klein ist und auch mit ungenutzten Funktionen in den Speicher passt.

// Aufbauhinweise Schaltung:
 
// LED: Das kürzere Bein ist '-' und muss auf GND (ground), das lange Bein ist '+' und bekommt 5V vom Pin5
 
// Vorwiderstände (bei 5V und Standard-LEDs): blaue LED: 120 Ohm; weiße LED: 60 Ohm; grün,gelb,rot: 220 Ohm; wenn wir die nicht haben, nehmen wir den nächsthöheren
// Vorwiderstandsrechner: https://www.elektronik-kompendium.de/sites/bau/1109111.htm (Durchlassspannung ggf. überprüfen!)
 
// Temperatursensor TMP36: Der Sensor hat drei Anschlüsse. Beim Blick auf die flache Seite des Sensors: links +5V, rechts GND und in der Mitte der Pin für
// das Temperatursignal. Auf diesem Pin gibt der Sensor eine Spannung zwischen 0 und 2,0 Volt aus. Wobei 0V -50°C entsprechen, 2,0V entspricht 150°C. 


// Uebersicht Programmteile:
// includes, Definition von Konstanten und Variablen einschliesslich TTN-Schluessel und deren Uebergabe an LMIC, eigene Globals
// void setup () wird einmal durchlaufen und konfiguriert z.B. Pinouts und die LMIC
// void loop () wird als Schleife ausgeführt
// Im code nach dem loop: Sammlung von Funktionen für Senden, Messen, Schalten usw., werden von anderer Stelle aus aufgerufen
 

#include <lmic.h>
#include <hal/hal.h>

// HIER EIGENE INCLUDES ERGÄNZEN
//***************************************

#include <SPI.h>

//***************************************




// FÜR JEDEN NODE NEU AUSFÜLLEN 
//***************************************
// Die Schlüssel liefert TTN bei Anmeldung über ABP
// TTN-Konsole Anwendung anlegen, device anlegen, Settings, ABP, generiert Schlüssel und Device-ID

// Syntaxbeispiel:  static const u1_t NWKSKEY[16] = { 0x6D, 0x40, 0x9D, 0x04, 0xE1, 0x29, 0x35, 0x17, 0x1D, 0xB7, 0x0D, 0x07, 0xED, 0xE3, 0xC5, 0xAD }; 
// Euer Schlüssel ist zu finden in TTN | Console | Application | device | overview (MSB-Ansicht ist kopierbar)
 static const u1_t NWKSKEY[16] = { 0x6D, 0x40, 0x9D, 0x04, 0xE1, 0x29, 0x35, 0x17, 0x1D, 0xB7, 0x0D, 0x07, 0xED, 0xE3, 0xC5, 0xAD };
 
// Syntaxbeispiel: static const u1_t APPSKEY[16] = { 0x99, 0x79, 0x1E, 0x44, 0x22, 0x5A, 0x47, 0x1F, 0x04, 0x56, 0xFA, 0xF9, 0x55, 0xBA, 0xC8, 0xF4 }; 
// Euer Schlüssel ist zu finden in TTN | Console | Application | device | overview
 static const u1_t APPSKEY[16] = { 0x99, 0x79, 0x1E, 0x44, 0x22, 0x5A, 0x47, 0x1F, 0x04, 0x56, 0xFA, 0xF9, 0x55, 0xBA, 0xC8, 0xF4 };

// ACHTUNG DEVICE ADRESSE MIT PRÄFIX 0x eingeben
// Syntaxbeispiel:  static const u4_t DEVADDR = 0X26011FB4; 
// Euer Schlüssel ist zu finden in TTN | Console | Application | device | overview | Device Adress

 static const u4_t DEVADDR = 0X26011FB4;

// Die vorstehenden Schlüssel sind fake und stehen hier als Kontrolle für die Syntax 

int id = 3; // zusätzliche ID, die vom Node gesendet wird

//****************************************





// Hier einige Konstanten und Funktionen für die LMIC (so lassen)
//****************************************

//******** nächster Block: hier holt sich die LMiC die Kommunikationssschlüssel, so lassen**********
void os_getArtEui (u1_t* buf) { }
void os_getDevEui (u1_t* buf) { }
void os_getDevKey (u1_t* buf) { }


//*****nächste Zeile: Sendeintervall in sec************
//**** da könnt ihr etwas eintragen ***********
//**** 20 sec ist zum Testen meist ein guter Kompromiss
//**** was auf Batterie läuft oder große Pakete sendet, sollte so selten wie möglich senden

const unsigned TX_INTERVAL = 20;


//******* nächster Block: Hardwaremapping und Sendeanweisungen für LMIC, so lassen *******
const lmic_pinmap lmic_pins = {
    .nss = 10,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 9,
    .dio = {2, 6, 7},
};


static osjob_t sendjob;

//********Ende LMIC-FOO*********
//******************************



// ********* Globals ***********
// HIER EIGENE KONSTANTEN- UND VARIABLENDEFINITIONEN UND ÄHNLICHES ERGÄNZEN
//*****************************************

int TMP36 = A0; //Der Sensor soll am analogen Pin A0 angeschlossen werden. Wir nennen den Pin ab jetzt "TMP36"
int sensorwert;
int temperatur = 25; //Unter der Variablen "temperatur" wird später der Temperaturwert abgespeichert.
int t=1; //Der Wert für „t“ gibt im Code die zeitlichen Abstände zwischen den einzelnen Messungen vor, wenn man mit Wartezeiten
         // im Loop arbeitet, statt mit dem Sendeintervall der LMIC.
int piezo=5; //Das Wort „piezo“ steht jetzt für die Zahl 5, also wird an Pin5 der Speaker angeschlossen.
// statt einem Piezo kann man natürlich auch eine Kontroll-LED (mit Vorwiderstand!) anschliessen

//****************************************





//**************************************************
// BEGINN SETUP (wird einmal durchlaufen)



void setup() {
    Serial.begin(115200);  //Im Setup beginnt die serielle Kommunikation. Über die serielle Kommunikation sendet das Board später die Messwerte über USB. In der Arduino-Software kann man unter „Werkzeuge“ den „Seriellen Monitor“ starten um die Messwerte zu sehen.
    Serial.println(F("Starting..."));
    


// AB HIER EIGENE SETUP-ANWEISUNGEN
//****************************************************
    
    pinMode (piezo, OUTPUT); //Der Pin für den Piezo-Lautsprecher oder die LED wird als Ausgang definiert, da hier um zu piepsen/zu leuchten eine Spannung benötigt wird.




// AB HIER LMIC-Foo
// stehen lassen, wenn ihr nicht sicher wisst, was ihr macht
//****************************************************

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
    LMIC_setLinkCheckMode(0);

    // TTN uses SF9 for its RX2 window. (Rx ist der Downlink vom Netz zum Mote, den nutzen wir hier nicht)
    // Wenn wir ihn nutzen wollen, muss er so bleiben.
    LMIC.dn2Dr = DR_SF9;


//************* hier sollte DR_SF7 stehen bleiben! alles andere braucht sehr viel Airtime und ist *********
//              nur im Einzelfall und nach Nutzung eines Airtimerechners angesagt                 *********
// Set data rate and transmit power for uplink (note: txpow seems to be ignored by the library)

    LMIC_setDrTxpow(DR_SF7,14);

//***********************************
    

    // Start job
    do_send(&sendjob);
}

// ***** ENDE SETUP ********
// *************************





//*********************************************************************
//*********************************************************************
//********** AB HIER DER LOOP-TEIL ************************************



void loop() 
{

// AB HIER EIGENE LOOP-BEFEHLE 
//*****************************

// EIGENER LOOP-BEFEHL 
// Was hier steht wird wesentlich öfter ausgeführt
// als das LMIC-Sendeintervall, kostet also mehr Rechenzeit und Strom
// als Anweisungen, die nur einmal mit dem Sendeintervall ausgeführt werden  

 
 //***********************************************************
    os_runloop_once();
}

//*********************************
// ENDE LOOP-SCHLEIFE


//************************************************************************************************

//************* ab hier: verschiedene Funktionen, die von irgendwoher
//************* aufgerufen werden, zunächst die LMIC-Funktionen, die
//************* LoRAWAN-Pakete bauen und versenden



//***** Statusmeldungen konfigurieren, die von der LMIC seriell ausgegeben werden ******
//*** Dafür gibt es zB im Flussbad-Beispiel noch eine very extended version zum debuggen***

void onEvent (ev_t ev) {
    if (ev == EV_TXCOMPLETE) {
        Serial.println(F("EV_TXCOMPLETE (includes waiting for RX windows)"));
        // Schedule next transmission
        os_setTimedCallback(&sendjob, os_getTime()+sec2osticks(TX_INTERVAL), do_send);
    }
}




// ANFANG LORAWAN-SENDEANWEISUNGEN******************************************************
 //********* Sendung zusammenbauen und absenden *************************

void do_send(osjob_t* j){
    
 //******** was hier direkt unterhalb steht, wird von der LMIC einmal pro Sendeintervall ausgeführt
 //********* also seltener als im Haupt-Loop

     //******* Temperatur auslesen
     //******* Die Auslesebefehle könnte man auch hier ausführen, sie **********************
     //******* stehen aber in der Funktion readtemp, weil es bei vielen Funktionalitäten  ** 
     //********übersichtlicher wird, die Funktionen in eigene Blöcke zu schreiben. *************
   
     readtemp(); //rufe die gesondert notierte Auslesefunktion (weiter unten) auf


 // Nachstehende auskommentierte Funktion wieder einschalten, wenn ihr eine offline-LED-Kontrolle für
 // die Messung braucht.    
 // piezoschalten(); //rufe die Funktion auf, die ab 30° piepst oder leuchtet
    

 
 
 //********* ab hier: message-bytes für das LoRaWAN-Paket zusammenbasteln *******
 
    // static: außer dieser do_send-Funktion kann keine andere Fuktion die Variable payload ändern ***
    // unit8_t: heißt unsigned (vorzeichenloser) integer mit 8bit (Syntax von c) also 0 bis 255*****
    // int8_t: 8bit integer von -128 bis 127 **************
    // payload[4] ist keine Variable, sondern ein array mit 4 Elementen (array: Datenliste mit Indexnummer beginnend bei 0) ********
    // wir teilen unsere Sensor-ID und unsere Messwerte (je 16 bit = 2 byte) in bytes auf, die wir später nacheinander senden.
    static int8_t payloadA[4];
      payloadA[0] = highByte(id);
      payloadA[1] = lowByte(id);
      payloadA[2] = highByte(temperatur);
      payloadA[3] = lowByte(temperatur);



//************** Einnererung: so decodieren wir das später wieder ***********************
//*** (nicht entkommentieren, dieser Block  ist javascript, das die Arduino-IDE nicht versteht):***
    // der Temperaturwert kann auch negativ sein. Ob er negativ ist, steht in payloadA(3) (wenn=0 ist er positiv, wenn er negativ ist, ist er 255)
    // in der TTN-Konsole decodieren wir das später so:
    //function Decoder(bytes) {
    //var decoded_tsbid = (bytes[0]<<8) | bytes[1] ;
    //var temp_vorzeichen = (bytes[2]<<24>>16) ;
    //var temp_wert = (bytes[3]) ;
    //var temp = temp_vorzeichen + temp_wert ; 
    //return {
    //  Id: decoded_tsbid ,
    //  Temperatur: temp ,
    //};
    //}
    //*** mit den Feldnamen Id und Temperatur können dann die Werte bei TTN abgeholt / ausgelesen werden**
//***********ENDE Decodierblock in javascript ************************************************



  
    // Check if there is not a current TX/RX job running
    if (LMIC.opmode & OP_TXRXPEND) {
        Serial.println(F("OP_TXRXPEND, not sending"));
    } else {
        // Prepare upstream data transmission at the next possible time.

// *****mit dem folgenden LMIC-Befehl senden wir unsere bytes nacheinander weg ***************
        LMIC_setTxData2(1, payloadA, sizeof(payloadA), 0);
   
        Serial.println(F("ID wird gesendet...")); //und geben etwas auf den Monitor aus, damit wir eine Kontrollmöglichkeit haben
        Serial.print(temperatur); // Nun wird der Wert „temperatur“ über die serielle Kommunikation an den PC gesendet. Durch öffnen des seriellen Monitors in der Arduino-Software kann die Temperatur abgelesen werden.
        Serial.println(" Grad Celsius"); // Im seriellen Monitor wird hinter der Temperatur die Einheit eingeblendet.

 
 //***** nachstehend: für debug-Zwecke kann man die Payload auch byteweise seriell ausgeben, hilft beim Test eines Decoders ******************   
     //       Serial.print(payloadA[0]); // Erstes byte der Payload an den seriellen Monitor senden.
     //       Serial.println(" byte0"); // und beschriften.
     //       Serial.print(payloadA[1]); 
     //       Serial.println(" byte1"); 
     //       Serial.print(payloadA[2]);
     //       Serial.println(" byte2"); 
     //       Serial.print(payloadA[3]);
     //       Serial.println(" byte3"); 
//*******************   
    
    
          }
    // Next TX is scheduled after TX_COMPLETE event.
}


// ************** ENDE DER LORAWAN-SENDEFUNKTION *******************
//*******************************************************************




// ************* Ab hier ist ohne Ende Platz für eigene Funktionen, die aus dem Hauptloop oder im LMIC-Teil aufgerufen werden
//


//********* Temperatursensor auslesen *****************

void readtemp()
{
    sensorwert=analogRead(TMP36); //Auslesen des Sensorwertes.
    temperatur= map(sensorwert, 0, 410, -50, 150); //Umwandeln des Sensorwertes mit Hilfe des "map" Befehls.

//**********/Daten bearbeiten, Daten putzen   ************
 
    if (temperatur>=127) //Wenn der Wert für die Temperatur über oder gleich 127 ist, dann…
    {
    temperatur = 127; //…setze Wert auf 127. Der Messebereich des Sensors liegt zw. -50 und 127.
    }

    if (temperatur<=-50) //Wenn der Wert für die Temperatur kleiner oder gleich -50 ist, dann…
    {
    temperatur = -50; //…setze Wert auf -50.
    }

//    temperatur= 256; // wer hier für Testzwecke manuell Temperaturen setzt, muss die Zeile davor auskommentieren.
  
 }



 //************ Piezo / LED einschalten****************

void piezoschalten()
{
 if (temperatur>=30) //Es wird eine IF-Bedingung erstellt: Wenn der Wert für die Temperatur über oder gleich 30 ist, dann…
    {
    digitalWrite(piezo,HIGH); //…fange an zu piepsen. (oder - wenn due eine LED bist - zu leuchten
    }

    else //Und wenn das nicht so ist…
    {
    digitalWrite(piezo,LOW); //…dann sei leise oder aus.
    }  
}







 // Lizenzangaben: Eingeflossen ist code aus der Anleitung Arduino für TTN samt der zugehörigen LMiC-Library:
 // https://github.com/matthijskooijman/arduino-lmic/blob/master/examples/ttn-abp/ttn-abp.ino
 // Lizenz der lmic: weitgehend  Eclipse Public License v1.0, Original-Copyright:
/*******************************************************************************
 * Lizentext LMIC
 * 
 * Copyright (c) 2015 Thomas Telkamp and Matthijs Kooijman
 * 
 * Permission is hereby granted, free of charge, to anyone
 * obtaining a copy of this document and accompanying files,
 * to do whatever they want with them without any restriction,
 * including, but not limited to, copying, modification and redistribution.
 * NO WARRANTY OF ANY KIND IS PROVIDED.
 *
 * This example sends a valid LoRaWAN packet with payload "Hello,
 * world!", using frequency and encryption settings matching those of
 * the The Things Network.
 *
 * This uses ABP (Activation-by-personalisation), where a DevAddr and
 * Session keys are preconfigured (unlike OTAA, where a DevEUI and
 * application key is configured, while the DevAddr and session keys are
 * assigned/generated in the over-the-air-activation procedure).
 *
 * Note: LoRaWAN per sub-band duty-cycle limitation is enforced (1% in
 * g1, 0.1% in g2), but not the TTN fair usage policy (which is probably
 * violated by this sketch when left running for longer)!
 *
 * To use this sketch, first register your application and device with
 * the things network, to set or generate a DevAddr, NwkSKey and
 * AppSKey. Each device should have their own unique values for these
 * fields.
 *
 * Do not forget to define the radio type correctly in config.h.
 *
 *******************************************************************************/

// Eingeflossen sind Teile des Beispiels Temperaturmesung aus Anleitung für Arduino von Funduino GmbH - Ralf Snieders Lizenz creative commons
// https://funduino.de/nr-9-temperatur-messen

// Eingeflossen sind Kommentare und Code von B.Seibel und C.Hammel, Technolgiestiftung Berlin (GPL)
    
