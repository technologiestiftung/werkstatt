#include <SoftwareSerial.h>

#define LED1  4  

SoftwareSerial loraSerial(2, 3); // TX, RX

void setup() {
  pinMode(LED1, OUTPUT);
  Serial.begin(9600);
  loraSerial.begin(9600);  
}

void loop() { 
  if(loraSerial.available() > 1){
    String input = loraSerial.readString();
    Serial.println(input);  
    if(input == "off") {
      digitalWrite(LED1, LOW);  
    } 
    else{
  digitalWrite(LED1, HIGH);
    }
    
  }
  delay(20);
}

