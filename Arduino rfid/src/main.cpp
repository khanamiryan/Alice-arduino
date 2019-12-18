#include <Arduino.h>

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
  Serial.print("hello");
}
int IRQ_PIN = 12;
int SCK_PIN = 13;
int MOSI_PIN = 11;
//miso tarber ens
int SDA_PIN = 9;
int RST_PIN = 10;
int MOSIS[8] = {5,6,4,4,5,6,7,8};
void checkRFID(int i){
  
  
    rfid.begin(IRQ_PIN,SCK_PIN,MOSI_PIN,MOSIS[i],SDA_PIN,RST_PIN);
  
  
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
    memcpy(serNum, str, 5);
    rfid.showCardID(serNum);//show the card ID

    //stex piti lini stugelu kod@
    
  }
  delay(100);
  rfid.halt(); //command the card into sleep mode 
}


void loop()
{
  for(int i=0;i<3;i++){
  checkRFID(i);  
  }
  
}