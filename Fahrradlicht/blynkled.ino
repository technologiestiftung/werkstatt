//die ersten 3 Zeilen kommen aus der Blynk App ID, Name and Token können dort kopiert werden.
#define BLYNK_TEMPLATE_ID ""
#define BLYNK_DEVICE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN ""
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
//das lange Bein der LED in D1 das kurze Bein in D2
int ledPin = D1;
int gndPin = D2;
char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "";
char pass[] = "";

void setup()
{
  // Debug console
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  //D1,D2 werden Output Pins
  pinMode(ledPin, OUTPUT);
  pinMode(gndPin, OUTPUT);
  digitalWrite(gndPin,LOW);
} 
// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  Blynk.syncVirtual(V0);  // will cause BLYNK_WRITE(V0) to be executed
}
//immer wenn sich der Zustand des virtuellen Pins (DatastreamV0) ändert, wird der Code in Blynke_Write ausgeführt
BLYNK_WRITE(V0)
{
  if(param.asInt() ==1)
  {
    digitalWrite(ledPin,HIGH);    
  }
    else
    {
      digitalWrite(ledPin,LOW);     
    }
}
void loop()
{
  Blynk.run();  
  }
