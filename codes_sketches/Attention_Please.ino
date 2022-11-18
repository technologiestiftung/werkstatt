//ultrasonic sensor triggers speech output "Please, stop! Thank you", if distance is under 10 cm, using the ESP8266 Wemos D1 Mini

#include <Arduino.h>
#include <ESP8266SAM.h>
#include <AudioOutputI2S.h>

AudioOutputI2S *out = NULL;

#define echoPin D7 // Echo Pin
#define trigPin D6 // Trigger Pin
 
long duration, distance; // Duration used to calculate distance
 
void setup()
{
  //reihenfolge-sensibel, achte darauf welche Funktion im Setup abgearbeitet werden, diese sollten in der gleichen Reihenfolge im Loop abgearbeitet werden
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  //definiert Kommunikation
  out = new AudioOutputI2S();
  out->begin();
  
}
 
void loop()
{
  /* The following trigPin/echoPin cycle is used to determine the
  distance of the nearest object by bouncing soundwaves off of it. */
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  //Calculate the distance (in cm) based on the speed of sound.
  distance = duration/58.2;
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  //Delay 50ms before next reading.
  delay(50);

if (distance<10){
  Serial.print("Please, stop");
  Serial.println("Thank you");
  
  ESP8266SAM *sam = new ESP8266SAM;
  sam->Say(out, "Please, stop");
  delay(500);
  sam->Say(out, "Thank you");
  delete sam;
  }
}
