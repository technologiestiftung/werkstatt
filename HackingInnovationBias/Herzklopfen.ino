#define echoPin 2
#define trigPin 3
long duration; 
int distance; 

void setup() {
  //Startet den Kommunikationskanal für Daten zwischen Arduino und PC. Funktioniert nur mit Kabel
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT); // Trigger Pin ist OUTPUT
  pinMode(echoPin, INPUT);  // Echo Pin ist INPUT
  }

void loop() {

  digitalWrite(trigPin, LOW); // Trigger sendet 10 ms Schallwelle aus
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH); // EchoPin wartet auf Rückkehr der ausgesendeten Schallwelle
  distance = duration * 0.034 / 2;  // Abstand = (Dauer in ms mal Schallgeschwindigkeit in cm/ms)/2 

  if(distance<10){
    
  // Herz klopft, wenn Du näher kommst
Serial.println("------------------------------------------------------------------");
Serial.println("------------------------------------------------------------------");
Serial.println("------------------------------------------------------------------");
Serial.println("-------------------------+++++-------+++++------------------------");
Serial.println("-----------------------++-----++---++-----++----------------------");
Serial.println("----------------------++--------+++--------++---------------------");
Serial.println("---------------------++----------+----------++--------------------");
Serial.println("--------------------++-----------------------++-------------------");
Serial.println("--------------------++-----------------------++-------------------");
Serial.println("--------------------++-----------------------++-------------------");
Serial.println("---------------------++---------------------++--------------------");
Serial.println("----------------------++-------------------++---------------------");
Serial.println("-----------------------++-----------------++----------------------");
Serial.println("-------------------------++-------------++------------------------");
Serial.println("---------------------------++---------++--------------------------");
Serial.println("-----------------------------++-----++----------------------------");
Serial.println("-------------------------------+++++------------------------------");
Serial.println("------------------------------------------------------------------");
}
}
