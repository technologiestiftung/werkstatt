 // Dieses Beispiel testet die Kommunikation zwischen Lappi und Arduino sonst nichts

 // Wenn euer Arduino damit penetrant die Zahl 25 auf dem seriellen Monitor ausgibt, ist alles richtig angeschlossen.


int temperatur = 25; //



//**************************************************
// BEGINN SETUP (wird einmal durchlaufen)

void setup() {
    Serial.begin(115200);  //Im Setup beginnt die serielle Kommunikation. Über die serielle Kommunikation sendet das Board später die Messwerte über USB. In der Arduino-Software kann man unter „Werkzeuge“ den „Seriellen Monitor“ starten um die Messwerte zu sehen.
    Serial.println(F("Starting..."));
}

// ***** ENDE SETUP ********




// *** Anfang LOOP ***********

void loop() 
{
Serial.println(temperatur); // Nun wird der Wert „temperatur“ über die serielle Kommunikation an den PC gesendet. 
delay(2000); //wartet zwei Sekunden, damit man den Output lesen kann
}

// ENDE LOOP-SCHLEIFE

// ************* Fehlermeldung beim Hochladen etwa in deisem Stil: *************
//
//  avrdude: stk500v2_ReceiveMessage(): timeout
//  avrdude: stk500v2_getsync(): timeout communicating with programmer
//  Beim Hochladen des Sketches ist ein Fehler aufgetreten

//**** Dann ist vermutlich ein Dragino-Shield mit GPS in Gebrauch. 
//**** GPS nutzt die selben seriellen Pins wie die Arduino-IDE. 
//**** Lösung: während des Hochladens das GPS-Reset-Knöpfchen 
//**** gedrückt halten, dann geht es.
//**** Quelle und Bild: 
//**** http://wiki.dragino.com/index.php?title=Lora/GPS_Shield#Pin_Definition


