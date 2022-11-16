// ---------------------------------------------------------------- //
// based on ultraschall.ino re-writed by Arbi Abdul Jabbaar
// ---------------------------------------------------------------- //

#define echoPin 2 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 3 //attach pin D3 Arduino to pin Trig of HC-SR04

#include <Wire.h>
#include "rgb_lcd.h"

// defines variables
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement

rgb_lcd lcd;

const int colorR = 50;
const int colorG = 255;
const int colorB = 20;

void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  
  // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);  
    lcd.setRGB(colorR, colorG, colorB);
}

void loop() {
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  
  // Displays the distance on LCD display
   // set the cursor to column 0, line 0
    // (note: line 0 is the first row, since counting begins with 0):
    lcd.setCursor(0, 0);
    // print the number of seconds since reset:
     lcd.print("Distance: ");
     lcd.print(distance);
     lcd.print(" cm");

    delay(500);
}
