#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"


SoftwareSerial mySoftwareSerial(13,12); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

void setup()
{
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);

  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  Serial.println(F("DFPlayer Mini online."));

  myDFPlayer.setTimeOut(500); //Set serial communictaion time out 500ms
   //----Set volume----
  myDFPlayer.volume(19);  //Set volume value (0~30).
  //----Set different EQ----
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
//  myDFPlayer.EQ(DFPLAYER_EQ_POP);
//  myDFPlayer.EQ(DFPLAYER_EQ_ROCK);
//  myDFPlayer.EQ(DFPLAYER_EQ_JAZZ);
//  myDFPlayer.EQ(DFPLAYER_EQ_CLASSIC);
//  myDFPlayer.EQ(DFPLAYER_EQ_BASS);
//SD Karte auslesen
myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
myDFPlayer.play(1);  //Play the first mp3
  delay(1000);
  //myDFPlayer.pause();  //pause the mp3
  //delay(1000);
 // myDFPlayer.start();  //start the mp3 from the pause
 // delay(1000);
}
  
  void loop()
{
  //myDFPlayer.playFolder(1, 3);  //play specific mp3 in SD:/15/004.mp3; Folder Name(1~99); File Name(1~255)
  //delay(1000);
  myDFPlayer.play(4);  //Play the first mp3
  delay(10000);
  delay(10000);
  delay(10000);
  //myDFPlayer.enableLoopAll();
  //delay(1000);
  //myDFPlayer.play(4); 
  //delay(1000);
  ///myDFPlayer.next();
  //delay(4000);
  //static unsigned long timer = millis();


  //if (millis() - timer > 5000) {
   // timer = millis();
    //myDFPlayer.next();  //Play next mp3 every 3 second.
  }

  
//}


  
