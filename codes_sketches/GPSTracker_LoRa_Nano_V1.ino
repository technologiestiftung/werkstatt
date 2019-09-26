#include <Adafruit_GFX.h>  // Include core graphics library for the display
#include <Adafruit_SSD1306.h>  // Include Adafruit_SSD1306 library to drive the display

#include <Fonts/FreeMonoBold12pt7b.h>  // Add a custom font
#include <Fonts/FreeMono9pt7b.h>  // Add a custom font

#include <TinyGPS++.h>
#include <Wire.h>
#include <SoftwareSerial.h>

#define rxPin 10 // wird mit TX des GPS verbunden
#define txPin 11 // wird mit RX des GPS verbunden

// Objekt für GPS
TinyGPSPlus gps;

//Objekt für OLED-Display
Adafruit_SSD1306 display(128, 64);  // Create display

// V24 Schnittstelle
#define Baudrate 9600

// Variablen für die Koordinaten
//Nice To Know: Die Koordinaten werden in Dezimalgrad und nicht in GMS angegeben
String lat_Dez, lng_Dez, lat_Grad, lng_Grad;
double lat_gps, lng_gps; // Die vom GPS zurückgegebenen Kommazahlen der Koordinaten
String anzeige_lat = "N:"; // Wenn die Gradzahl > 0 per default "N:"
String anzeige_lng = "E:"; // Wenn die Gradzahl > 0: per default "E:"

SoftwareSerial gpsSerial(rxPin, txPin);

void setup()  // Start of setup
{                

  delay(100);  // This delay is needed to let the display to initialize

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // Initialize display with the I2C address of 0x3C
 
  display.clearDisplay();  // Clear the buffer

  display.setTextColor(WHITE);  // Set color of the text

  display.setRotation(0);  // Set orientation. Goes from 0, 1, 2 or 3

  display.setTextWrap(false);  // By default, long lines of text are set to automatically “wrap” back to the leftmost column.
                               // To override this behavior (so text will run off the right side of the display - useful for
                               // scrolling marquee effects), use setTextWrap(false). The normal wrapping behavior is restored
                               // with setTextWrap(true).

  display.dim(1);  //Set brightness (0 is maximun and 1 is a little dim)
  
gpsSerial.begin(Baudrate); 
//display.clearDisplay();  // Clear the display so we can refresh
display.setFont(&FreeMono9pt7b);  // Set a custom font
display.setTextSize(0);  // Set text size. We are using a custom font so you should always use the text size of 0
// Print text:
  display.setCursor(0, 10);  // (x,y)
  display.println("Willkommen");  // Text or value to print
//display.display();
}  // End of setup


void loop() {
  
  // Erst wenn die Daten ankommen ...
  while (gpsSerial.available() > 0)
    if (gps.encode(gpsSerial.read())) displayInfo();
    
  if (millis() > 5000 && gps.charsProcessed() < 10){
    display.clearDisplay();
    display.setCursor(0,10);
    display.setFont(&FreeMono9pt7b);  // Set a custom font
    display.setTextSize(0);  // Set text size. We are using a custom font so you should always use the text size of 0
    display.println(" Kein GPS-Empf\xE1nger ");
    //display.display();
    while (true);
  }

  display.display(); 
} // Ende Hauptschleife

void displayInfo(){
  
  if (gps.location.isValid()){
    lat_gps = gps.location.lat();  //Breite einlesen
    lng_gps = gps.location.lng();  //Länge einlesen
    
    if (lat_gps < 0) {             //Anpassen, wenn auf der Südhalbkugel
      anzeige_lat = "S:";
      lat_gps = abs(lat_gps);      //abs-Funktion returned Wert in Klammern als POSITIVEN Wert
    }
    
    if (lng_gps < 0){              //Anpassen, wenn westlich von Greenwich
      anzeige_lng = "W:";
      lng_gps = abs(lng_gps);      //abs-Funktion returned Wert in Klammern als POSITIVEN Wert
    }

    
    //Ergebnis Anzeigen
    display.clearDisplay();
    display.setTextSize(0);
    display.setCursor(0,10);
    display.print("Satelliten");
    display.setCursor(0,24);
    display.print("gefunden");
    
    //Achtung: wenn println(lat_gps), dann startet Programm nicht neu

    // Ergebnis anzeigen
    display.setCursor(0, 44);
    display.print(anzeige_lat); //N bzw. S
    display.println(gps.location.lat(), 6); // mit 6 Nachkommestellen
    display.setCursor(0, 60);
    display.print(anzeige_lng);  //E bzw. W
    display.println(gps.location.lng(), 6); // mit 6 Nachkommastellen
  }
  
  else // Keine Koordinaten wurden übermittelt
  {
    display.clearDisplay();
    display.setCursor(0,10);
    display.print("Satelliten");
    display.setCursor(0,24);
    display.print("suchen");
    delay(1500);
  }
    display.display();

} // end zeigeKoordinaten
