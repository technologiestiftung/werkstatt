//DS18B20 - ARDUMETER
//github.com/nlpke/DS18B20-Arduino-Tutorial
//ANSCHLUSS / WIRING
//GROVE-LCD RGB BACKLIGHT V2
//LCD         ARDUINO UNO
//GND         GND
//VCC         5V
//SDA         A4    | (I2C)
//SCL         A5    | (I2C)
//OneWire DS18B20
//DS18B20     ARDUINO UNO
//GND         GND
//VCC         3.3V
//DATA        D9

//Benötigte Bibliotheken importieren
#include <OneWire.h>
#include <DallasTemperature.h>
#include <rgb_lcd.h>
//OneWire Schnittstelle auf PIN9
OneWire schnittstelle(9);

//Temperatursensor an der Onewire Schnittstelle
DallasTemperature sensor(&schnittstelle);

//RGB-LCD
rgb_lcd lcd;

void setup() {
  Serial.begin(9600);

  //Sensor initialisieren
  sensor.begin();
  //Genauigkeit einstellen
  sensor.setResolution(TEMP_12_BIT);
  //Display initialisieren
  lcd.begin(16,2); //16 Zeichen in 2 Spalten
  //Farbe nach Wunsch
  lcd.setRGB(120,0,0);
  lcd.print("Ardumeter");
  //Alle Farben durchlaufen
  delay(500);
  lcd.setRGB(0,120,0);
  delay(500);
  lcd.setRGB(0,0,120);
  delay(500);
  lcd.setRGB(120,0,0);
  Serial.println("BEREIT");
  delay(2500);
}

void loop() {
 //Temperaturwerte auslesen
 sensor.requestTemperatures();
 //Zeit Temperatur ausgeben
 Serial.print(" ");
 Serial.print(sensor.getTempCByIndex(0));
 //LCD Inhalt löschen
 lcd.clear();
 //Temperatur auf dem LCD ausgeben
 lcd.print("TEMPERATUR:" + String(sensor.getTempCByIndex(0)));
 //Farbe des LCD in Abhängigkeit zur Temperatur
 if (sensor.getTempCByIndex(0) < 25){
  lcd.setRGB(0,0,120);
 }else{
  lcd.setRGB(120,0,0);
 }
 Serial.println();
 //2 Sek. warten
 delay(2000);


}
