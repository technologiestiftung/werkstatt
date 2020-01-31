//Unter der Variablen "messwert" wird später der Messwert des Sensors gespeichert.
int messwert=0; 
int Pump=5;

#define Sensor        A0 
void setup() 

{ ////Hier beginnt das Setup.

  Serial.begin(9600); //Die Kommunikation mit dem seriellen Port wird gestartet. Das benötigt man, um sich den ausgelesenen Wert im serial monitor anzeigen zu lassen.
  pinMode (5, OUTPUT);
  pinMode(Sensor,INPUT_PULLUP); 
  
   
}

void loop()

{ //Hier beginnt der Hauptteil  

 messwert=analogRead(Sensor); //Die Spannung an dem Sensor wird ausgelesen und unter der Variable „messwert“ gespeichert.
 temp = messwert; //speichere aktuellen mewsswert in temporärer variable

  Serial.print("Feuchtigkeits-Messwert:"); //Ausgabe am Serial-Monitor: Das Wort „Feuchtigkeits-Messwert:"
  Serial.println(messwert); //und im Anschluss der eigentliche Messwert.
  
  Serial.println("Hello world!");

  delay(100); //Zum Schluss noch eine kleine Pause, damit nicht zu viele Zahlenwerte über den Serial-Monitor rauschen.

  
if (messwert>960) { 
digitalWrite(Pump, HIGH); //...soll die Pumpe pumpen.
}

delay(20000); //20sec 

// ---START ELSE IF
//die else if schleife ist optiona und kann entfernt werden
else if (messwert = temp) // wenn sich wert nicht verändert hat (also wassertank leer ist)
digitalWrite(Pump, LOW); //...soll die Pumpe aus sein.
// ---ENDE ELSE IF
 
else {
digitalWrite(Pump, LOW); //...soll die Pumpe aus sein.
}

}
