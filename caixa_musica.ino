/***************************************************
 Durch Wolfgang Ewald modifizierte Version des FullFunction.ino Beispielsketches der DFRobotDFPlayerMini Bibliothek:
 
 DFPlayer - A Mini MP3 Player For Arduino
 <https://www.dfrobot.com/product-1121.html>
 
 ***************************************************
 This example shows the all the function of library for DFPlayer.
 
 Created 2016-12-07
 By [Angelo qiao](Angelo.qiao@dfrobot.com)
 
 GNU Lesser General Public License.
 See <http://www.gnu.org/licenses/> for details.
 All above must be included in any redistribution
 ****************************************************/

/***********Notice and Trouble shooting***************
 1.Connection and Diagram can be found here
<https://www.dfrobot.com/wiki/index.php/DFPlayer_Mini_SKU:DFR0299#Connection_Diagram>
 2.This code is tested on Arduino Uno, Leonardo, Mega boards.
 ****************************************************/

#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include "JC_Button.h"

SoftwareSerial mySoftwareSerial(10,11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

#define buttonPause A0
#define buttonUp A1
#define buttonDown A2

Button pauseButton(buttonPause);
Button upButton(buttonUp);
Button downButton(buttonDown);

void setup()
{
  mySoftwareSerial.begin(9600);
  Serial.begin(9600);
  
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
  printHelp();
  
  myDFPlayer.setTimeOut(500); //Set serial communictaion time out 500ms
  
  //----Set volume----
  myDFPlayer.volume(15);  //Set volume value (0~30).
 // myDFPlayer.volumeUp(); //Volume Up
 // myDFPlayer.volumeDown(); //Volume Down
  
  //----Set different EQ----
 // myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
//  myDFPlayer.EQ(DFPLAYER_EQ_POP);
  myDFPlayer.EQ(DFPLAYER_EQ_ROCK);
//  myDFPlayer.EQ(DFPLAYER_EQ_JAZZ);
//  myDFPlayer.EQ(DFPLAYER_EQ_CLASSIC);
//  myDFPlayer.EQ(DFPLAYER_EQ_BASS);
  
  //----Set device we use SD as default----
//  myDFPlayer.outputDevice(DFPLAYER_DEVICE_U_DISK);
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
//  myDFPlayer.outputDevice(DFPLAYER_DEVICE_AUX);
//  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SLEEP);
//  myDFPlayer.outputDevice(DFPLAYER_DEVICE_FLASH);
  
  //----Mp3 control----
//  myDFPlayer.sleep();     //sleep
//  myDFPlayer.reset();     //Reset the module
//  myDFPlayer.enableDAC();  //Enable On-chip DAC
//  myDFPlayer.disableDAC();  //Disable On-chip DAC
//  myDFPlayer.outputSetting(true, 15); //output setting, enable the output and set the gain to 15
  

}

void loop(){
  if(Serial.available()){
    readCommand();
  }
 
  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
  }
}

void readCommand(){
  char cmd = ' ';
  int value1, value2 = 0;
  cmd = Serial.read();
  value1 = Serial.parseInt();
  value2 = Serial.parseInt();
 
  switch(cmd){
    case 'h': 
      printHelp();
      break;
      
    case '>':
      myDFPlayer.next();
      break;
      
    case '<':
      myDFPlayer.previous();
      break;
      
    case 'p':
      if(value2 == '0') myDFPlayer.play(value1);
      else if(value1 != 0) myDFPlayer.playFolder(value1, value2);
      break;

    case 'P':
      if(value2 == '0') myDFPlayer.playMp3Folder(value1); 
      else if(value1 != 0)myDFPlayer.playLargeFolder(value1, value2);
      break;
    
    case '+':
      myDFPlayer.volumeUp(); 
      break;
      
    case '-':
      myDFPlayer.volumeDown();
      break;
      
    case 'v':
      myDFPlayer.volume(value1);
      break;
      
    case 'b':
      myDFPlayer.pause();
      break;

    case 's':
      myDFPlayer.start();
      break;
          
    case 'z':
      myDFPlayer.sleep();
      break;
      
    case 'L':
      if(value1 == '0') myDFPlayer.enableLoopAll();
      else myDFPlayer.loopFolder(value1);
      break;
    
    case 'l':
      if(value1 == '0') myDFPlayer.disableLoopAll();
      else myDFPlayer.loop(value1);
      break;
    
    case 'A':
      myDFPlayer.advertise(value1);
      break;
    
    case 'a':
      myDFPlayer.stopAdvertise();
      break;
    
    case 'q':
      if(value1 == 1) Serial.println(myDFPlayer.readState()); 
      else if(value1 == 2) Serial.println(myDFPlayer.readVolume());
      else if(value1 == 3) Serial.println(myDFPlayer.readEQ());
      else if(value1 == 4) Serial.println(myDFPlayer.readFileCounts());
      else if(value1 == 5) Serial.println(myDFPlayer.readFolderCounts());
      else if(value1 == 6) Serial.println(myDFPlayer.readCurrentFileNumber());
      break;
    default:
      Serial.println("Ungültiges Kommando");
      break;
  }

}

void printHelp(){
  Serial.println("DFPlayer Commands:");
  Serial.println(" h - help");
  Serial.println(" > - next ");
  Serial.println(" < - previous");
  Serial.println(" p3 - play");
  Serial.println(" p3,5 - play folder 3, file 5");
  Serial.println(" P3,5 - play large folder 3, file 5");
  Serial.println(" P3 - play file 3 in MP3 folder"); 
  Serial.println(" + - volume up");
  Serial.println(" - - volume down");
  Serial.println(" v10 - set volume to 10");
  Serial.println(" b - Pause");
  Serial.println(" s - start ");
  Serial.println(" z - sleep ");
  Serial.println(" L - enable loop all");
  Serial.println(" l - disable loop all");
  Serial.println(" L3 - loop folder 3");
  Serial.println(" l3 - loop file 3");
  Serial.println(" A3 - advertise file 3");
  Serial.println(" a - stop advertise "); 
  Serial.println(" qx - query No. x");
  Serial.println("     x = 1 - read state");
  Serial.println("     x = 2 - read volume");
  Serial.println("     x = 3 - read equalizer");
  Serial.println("     x = 4 - read file counts");
  Serial.println("     x = 5 - read current file number");
  Serial.println("     x = 6 - read file counts in folder");
  Serial.println("     x = 7 - read folder counts");
}

void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerUSBInserted:
      Serial.println("USB Inserted!");
      break;
    case DFPlayerUSBRemoved:
      Serial.println("USB Removed!");
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }  
}
