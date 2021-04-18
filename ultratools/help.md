# Level 1 "Hello World"

<img align="center" width="50%" src="images/esput.jpg">

<img align="top" width="100%" src="images/ut0.jpg">

##  Links :
 
1. Arduino  https://www.arduino.cc/en/software
2. Just copy and include like shown in the upper blue box http://arduino.esp8266.com/stable/package_esp8266com_index.json
3. You can download u8g2 libary from here if its not in Arduino Libary Manager https://github.com/olikraus/u8g2

## Troubleshooting "Level 1"

1. You use MacOS Big Sur and during upload you get an error that mentions pySerial Direction not found: https://forum.arduino.cc/t/pyserial-and-esptools-directory-error/671804/13
2. Uploaded but you can't see "Hello World" on your oled? Plug USB cable in and out.
 
# Level 2: "Upload Code from GitHub"

<img align="top" width="100%" src="images/ultratools3.jpg">

## Link to Asciioled

https://github.com/technologiestiftung/werkstatt/blob/master/ultratools/code/asciioled.ino

# Level 3: "Drawing with ascii" 

## How to draw a dot at a certain position

```
  void loop(void) {
  u8g2.clearBuffer(); 
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(17,4,"...."); 
  u8g2.drawStr(17,7,".  .");// write something to the internal memory
  u8g2.drawStr(17,10,"....");  // write something to the internal memory
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
  }
  
```  
There is space for 128 dots in x direction and 32 dots in y direction

<img align="top" width="100%" src="images/oledxy.jpg">

This code draws 3 dots at position x = 15 and y = 11 for 4 seconds

```
u8g2.clearBuffer(); 
u8g2.setFont(u8g2_font_ncenB08_tr);
u8g2.drawStr(15,11,"...");
u8g2.sendBuffer();          
delay(4000); 

```

## How to draw an icon - suggestion for workflow

1. open a text editor
2. create a small table (15x15 works good for the start)
3. draw your icon into that table like in the image below
4. transfer the icon into code
5. upload
6. improuve the code
7. upload again
<img align="top" width="100%" src="images/oledmatrix1.jpg">



# Level 4: Display x-Bitmaps on oled
