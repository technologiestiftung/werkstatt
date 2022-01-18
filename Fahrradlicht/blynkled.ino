#define BLYNK_TEMPLATE_ID ""
#define BLYNK_DEVICE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN ""

// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

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
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Setup a function to be called every second
  
  pinMode(ledPin, OUTPUT);
  pinMode(gndPin, OUTPUT);
  digitalWrite(gndPin,LOW);
} 


// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  Blynk.syncVirtual(V0);  // will cause BLYNK_WRITE(V0) to be executed

  // Change Web Link Button message to "Congratulations!

}


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
