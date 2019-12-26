/**************************************************************************/
/*! 
    @file     iso14443a_uid.pde
    @author   Adafruit Industries
  @license  BSD (see license.txt)

    This example will attempt to connect to an ISO14443A
    card or tag and retrieve some basic information about it
    that can be used to determine what type of card it is.   
   
    Note that you need the baud rate to be 115200 because we need to print
  out the data and read from the card at the same time!

This is an example sketch for the Adafruit PN532 NFC/RFID breakout boards
This library works with the Adafruit NFC breakout 
  ----> https://www.adafruit.com/products/364
 
Check out the links above for our tutorials and wiring diagrams 
These chips use SPI or I2C to communicate.

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!


*/
/**************************************************************************/
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>
#include "I2C_Anything.h"


// If using the breakout with SPI, define the pins for SPI communication.
#define PN532_SCK  (13)
#define PN532_MOSI (11)

#define PN532_MISO (12)

#define PN532_SS1   (5)
#define PN532_SS2   (6)
#define PN532_SS3   (7)


// If using the breakout or shield with I2C, define just the pins connected
// to the IRQ and reset lines.  Use the values below (2, 3) for the shield!
#define PN532_IRQ   (2)
#define PN532_RESET (10)  // Not connected by default on the NFC Shield

// Uncomment just _one_ line below depending on how your breakout or shield
// is connected to the Arduino:

// Use this line for a breakout with a SPI connection:


//'478199344194128' karmir moloraki kodna 267211121 poxaren avelacneinq kayf kliner tekuz kam kamov
//String karmirMolorak = "478199344194128";


String rightAnswers[3] = {"4145128138168100128","478118138168100129","45890138168100128"};
int rightAnswered  = 0;
const byte numReaders = 3;

  boolean success[numReaders];

  uint8_t uid[numReaders][7] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength[numReaders];        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  bool rightReaders[numReaders] = {false,false,false};


// Each reader has a unique Slave Select pin
const byte ssPins[numReaders] = {5, 6, 7};

String currentIDs[numReaders];
uint32_t versiondata[numReaders];




// Use this line for a breakout with a hardware SPI connection.  Note that
// the PN532 SCK, MOSI, and MISO pins need to be connected to the Arduino's
// hardware SPI SCK, MOSI, and MISO pins.  On an Arduino Uno these are
// SCK = 13, MOSI = 11, MISO = 12.  The SS line can be any digital IO pin.
//Adafruit_PN532 nfc(PN532_SS);

// Or use this line for a breakout or shield with an I2C connection:
//Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
// also change #define in Adafruit_PN532.cpp library file
   #define Serial SerialUSB
#endif
//Adafruit_PN532 readers[5];
Adafruit_PN532* readers[3];
void requestEvent() {
  
  

  
  rightAnswered = 0;
  for(int j=0;j<numReaders;j++){
      if(rightReaders[j]==true){
        rightAnswered++;
      }
  }

  I2C_writeAnything (rightAnswered);
//  Wire.write(rightAnswered);  /*send string on request */
 
 
 
}



void receiveEvent(int howMany) {
 while (0 <Wire.available()) {
    char c = Wire.read();      /* получаем байт как символ*/
    Serial.print(c);           /* выводим символ в серийный порт */
  }
 Serial.println();             /* переходим на новую строку */
}
void setup(void) {
  #ifndef ESP8266
    while (!Serial); // for Leonardo/Micro/Zero
  #endif

  Wire.begin(8);                /* задаем на шине i2c 8 адрес */
  Wire.onReceive(receiveEvent); /* регистрируем полученное событие */
  Wire.onRequest(requestEvent); /* регистрируем запрошенное событие */



  
  Serial.begin(115200);
  Serial.println("Hello!");
  

  for (int i = 0; i < numReaders; i++) {
    readers[i] =  new Adafruit_PN532(PN532_SCK, PN532_MISO, PN532_MOSI, ssPins[i]);
    readers[i]->begin();

    versiondata[i] = readers[i]->getFirmwareVersion();


  // Got ok data, print it out!
  Serial.print("Found chip PN5 on pin");
  Serial.print(ssPins[i]);
  Serial.print(" - "); 
  Serial.println((versiondata[i]>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); 
  Serial.print((versiondata[i]>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata[i]>>8) & 0xFF, DEC);
  Serial.println();Serial.println();


    // Set the max number of retry attempts to read from a card
  // This prevents us from waiting forever for a card, which is
  // the default behaviour of the PN532.
  readers[i]->setPassiveActivationRetries(0x10);

  // configure board to read RFID tags
  readers[i]->SAMConfig();
  }

  
  Serial.println("Waiting for an ISO14443A card");
}

  
  
void loop(void) {
  //Serial.println("Waiting");

  

  for (int i = 0; i < numReaders; i++) {
      // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
      // 'uid' will be populated with the UID, and uidLength will indicate
      // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
      success[i] = readers[i]->readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[i][0], &uidLength[i],100);

    if (success[i]) {
       Serial.println("Found a card! ");
       Serial.println(i);
      // Serial.print("UID Length: ");Serial.print(uidLength[j], DEC);Serial.println(" bytes");
      // Serial.print("UID Value: ");
      
      //rightAnswers[j] = String (uid[j]);
      String cardCode="";
      for (uint8_t x=0; x < uidLength[i]; x++) 
      {
        cardCode+=uid[i][x];
      }
      // Serial.print("card ");
      //  Serial.println(cardCode);
      //  Serial.print("right card code ");
      //  Serial.println(rightAnswers[i]);
      //  delay(1000);
      // if(cardCode == karmirMolorak&&i==0){//karmir molorak@ askhatum a, naev en myusna askhatum, ira original qart@
      //   rightReaders[i] = true;
      //   continue;
      // }
      if(cardCode==rightAnswers[i]){
        
          rightReaders[i] = true;
          continue;
      }else{
        rightReaders[i] = false;
        continue;
      }
    }else{
        
       rightReaders[i] = false;
       continue;
    }
    rightReaders[i] = false;
  }

  
}

