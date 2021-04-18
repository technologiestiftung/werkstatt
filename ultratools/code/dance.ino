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
  
  u8g2.clearBuffer(); 
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(17,4,"...."); 
  u8g2.drawStr(17,7,".  .");// write something to the internal memory
  u8g2.drawStr(17,10,"....");  // write something to the internal memory
    // write something to the internal memory
  u8g2.drawStr(10,15,".......");  // write something to the internal memory
  u8g2.drawStr(18,16,".");
  u8g2.drawStr(18,17,".");
  u8g2.drawStr(18,18,".");  // write something to the internal memory
  u8g2.drawStr(18,19,".");
  u8g2.drawStr(18,20,".");// write something to the internal memory
  u8g2.drawStr(16,21,"..........");  // write something to the internal memory
  u8g2.drawStr(16,22,".    ");  // write something to the internal memory
  u8g2.drawStr(16,23,".    ");  // write something to the internal memory
  u8g2.drawStr(16,24,".    ");
  u8g2.drawStr(16,25,".    ");
  u8g2.drawStr(16,26,".    ");
  u8g2.drawStr(16,27,".    ");
  u8g2.drawStr(16,28,".    ");
  u8g2.drawStr(16,29,".    ");
  u8g2.drawStr(16,30,".    ");
  u8g2.drawStr(16,31,".    ");
  u8g2.drawStr(16,32,".    ");
  
  
  

  u8g2.sendBuffer();          // transfer internal memory to the display
  delay(4000);  
  u8g2.clearBuffer(); 
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(15,4,"oo");  // write something to the internal memory
  u8g2.drawStr(15,8,"oo");  // write something to the internal memory
  
  u8g2.drawStr(9,16,"oooo");  // write something to the internal memory
  u8g2.drawStr(16,19,"o");
  
  u8g2.drawStr(16,22,"o");  // write something to the internal memory
  u8g2.drawStr(14,24,"ooooo");  // write something to the internal memory
  u8g2.drawStr(14,29,"o    "); 
    
  u8g2.drawStr(14,30,"o    ");
  u8g2.drawStr(14,32,"o    ");
  u8g2.sendBuffer();          // transfer internal memory to the display
  delay(4000);  
}
