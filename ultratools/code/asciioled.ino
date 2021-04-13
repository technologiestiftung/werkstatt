#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE); 

void setup(void) {
  u8g2.begin();
}


void loop(void) {
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
  u8g2.drawStr(0,10,".......");  // write 7 dots on position (x=0 and Y=10) to the internal memory
  u8g2.drawStr(5,12,"......."); // write 7 dots on position (x=5 and Y=12) to the internal memory
  u8g2.sendBuffer();          // transfer internal memory to the display
  delay(1000);  
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
  u8g2.drawStr(0,20,".......");  // write something to the internal memory
  u8g2.drawStr(5,32,".......");
  u8g2.sendBuffer();          // transfer internal memory to the display
  delay(1000);  
  u8g2.clearBuffer();   
  u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
  u8g2.drawStr(20,20,".......");  // write something to the internal memory
  u8g2.drawStr(20,32,".......");
  u8g2.sendBuffer();          // transfer internal memory to the display
  delay(1000); 
  u8g2.clearBuffer(); 
  u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
  u8g2.drawStr(20,10,".");  // write something to the internal memory
  u8g2.drawStr(20,12,".");  // write something to the internal memory
  u8g2.drawStr(20,14,".");  // write something to the internal memory
  u8g2.drawStr(20,16,".");  // write something to the internal memory
  u8g2.drawStr(20,18,".");  // write something to the internal memory
  u8g2.drawStr(20,20,".");
  u8g2.drawStr(30,10,".");  // write something to the internal memory
  u8g2.drawStr(30,12,".");  // write something to the internal memory
  u8g2.drawStr(30,14,".");  // write something to the internal memory
  u8g2.drawStr(30,16,".");  // write something to the internal memory
  u8g2.drawStr(30,18,".");  // write something to the internal memory
  u8g2.drawStr(30,20,".");
  u8g2.sendBuffer();          // transfer internal memory to the display
  delay(1000);  
}
