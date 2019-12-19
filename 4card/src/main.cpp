#include <Arduino.h>

#include "FastLED.h"

/*******************************************
* function:get the id of RFID key
* RFID   Uno r3
* VCC    3.3V
* RST    2
* GND    GND
* MISO   3
* MOSI   4
* SCK    5
* NSS    6
* IRQ    7
****************************************/

#include"rfid1.h"
RFID1 rfid;//create a variable type of RFID1

uchar serNum[5]; // array to store your ID

void setup()
{
  Serial.begin(9600); //initialize the serial
  //rfid.begin(7, 5, 4, 3, 6, 2);     //rfid.begin(IRQ_PIN,SCK_PIN,MOSI_PIN,MISO_PIN,NSS_PIN,RST_PIN)
  //delay(100);//delay 1s
  //rfid.init(); //initialize the RFID
  pinMode(9,OUTPUT);
  Serial.print("hello");
  tone(9,200,500);
  
}
int IRQ_PIN = A1;
int SCK_PIN = 13;
int MOSI_PIN = 11;
//miso tarber ens
int SDA_PIN = 12;
int RST_PIN = 10;
int MISOS[4] = {4,5,6,7};
int buzzer=9;
int states[4] = {0,0,0,0};
void checkRFID(int i){
  
  
    rfid.begin(IRQ_PIN,SCK_PIN,MOSI_PIN,MISOS[i],SDA_PIN,RST_PIN);
  
  
  // if(i==2){
  //   rfid.begin(2,4,5,9,3,6);
  // }
  // if(i==3){
  //   rfid.begin(2,4,5,10,3,6);
  // }
  // if(i==4){
  //   rfid.begin(2,4,5,11,3,6);
  // }
  // if(i==5){
  //   rfid.begin(2,4,5,12,3,6);
  // }
  // if(i==6){
  //   rfid.begin(2,4,5,13,3,6);
  // }


  delay(100);
  rfid.init();
  uchar status;
  uchar str[MAX_LEN];
  // Search card, return card types
  
  status = rfid.request(PICC_REQIDL, str);
  if (status != MI_OK)
  {
  
    states[i]=0;
    return;
  }
  // Show card type
  rfid.showCardType(str);
  //Prevent conflict, return the 4 bytes Serial number of the card
  status = rfid.anticoll(str);
  if (status == MI_OK)
  {
    Serial.print("Card ");
    Serial.println(i);
     tone(9,200,500);
    memcpy(serNum, str, 5);
    rfid.showCardID(serNum);//show the card ID
     for(int b=0;b<5;b++){
        Serial.print(serNum[b]);
        Serial.print(", ");
     }
    //stex piti lini stugelu kod@
    states[i]=1;
  }
  delay(100);
  rfid.halt(); //command the card into sleep mode 
}


void loop()
{
  for(int i=0;i<4;i++){
  checkRFID(i);  
  }
  EVERY_N_SECONDS(5){
    for(int i=0;i<4;i++){
      Serial.print("device ");
      Serial.print(i);
      Serial.print(" ");
      Serial.println(states[i]);
    }
    
  }
}