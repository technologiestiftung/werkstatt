//Unter der Variablen "messwert" wird später der Messwert des Sensors gespeichert.
int messwert=0; 
int temp = 0; //temporäre Variable
int Pump=5;

#define Sensor        A0 
void setup() { ////Hier beginnt das Setup.

  Serial.begin(9600); //Die Kommunikation mit dem seriellen Port wird gestartet. Das benötigt man, um sich den ausgelesenen Wert im serial monitor anzeigen zu lassen.
  pinMode (5, OUTPUT);
  pinMode(Sensor,INPUT_PULLUP); 
}


void loop() { //Hier beginnt der Hauptteil  

 messwert=analogRead(Sensor); //Die Spannung an dem Sensor wird ausgelesen und unter der Variable „messwert“ gespeichert.

  Serial.print("Feuchtigkeits-Messwert:"); //Ausgabe am Serial-Monitor: Das Wort „Feuchtigkeits-Messwert:"
  Serial.println(messwert); //und im Anschluss der eigentliche Messwert.
  Serial.println(temp);
  

  delay(100); //Zum Schluss noch eine kleine Pause, damit nicht zu viele Zahlenwerte über den Serial-Monitor rauschen.

  
if (messwert>960) { 
digitalWrite(Pump, HIGH); //...soll die Pumpe pumpen.
delay(10000); //pumpe fuer 10 sec
temp=analogRead(Sensor); //neuen Sensorwert nach pumpen messen
}

else if (temp>960){ // wenn neuer Sensorwert nach 10sec pumpen immer noch "trocken" sagt ist der Wassertank scheinbar leer
  digitalWrite(Pump, LOW);
}

else{
  digitalWrite(Pump, LOW);
}


}
