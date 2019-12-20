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
#include <Wire.h>

RFID1 rfid;//create a variable type of RFID1

uchar serNum[5]; // array to store your ID
int buzzerPIN = 11;

int IRQ_PIN = 13;
int SCK_PIN = A0;
int MOSI_PIN = A2;
//miso tarber ens
int SDA_PIN = 2;
int RST_PIN = 12;
int MISOS[8] = {3,4,5,6,7,8,9,10};

int states[8] = {0,0,0,0,0,0,0,0};

String mode = "game";

const uchar rightRfids[8][8] = {//stex lcnel voroshacner@
  {90, 126, 101, 15, 78},
  {185, 94, 92, 141, 54},
  {163, 77, 84, 32, 154},
  {42, 198, 101, 15, 134},
  {250, 152, 89, 15, 52},
  {58, 196, 91, 15, 170},
  {233, 161, 175, 142, 105},
  {59, 152, 252, 14, 81}
  
  
  };

int rfidsState[8] = {0,0,0,0,0,0,0,0};
int rfidWrongTimes[8] = {0,0,0,0,0,0,0,0};
int howManyRfidsRight = 0;


// 90, 126, 101, 15, 78

//185, 94, 92, 141, 54

//163, 77, 84, 32, 154

//42, 198, 101, 15, 134

//250, 152, 89, 15, 52

//58, 196, 91, 15, 170

// 233, 161, 175, 142, 105, 

//59, 152, 252, 14, 81


void checkRFID(int i){
  
  
    rfid.begin(IRQ_PIN,SCK_PIN,MOSI_PIN,MISOS[i],SDA_PIN,RST_PIN);



  delay(100);
  rfid.init();
  uchar status;
  uchar str[MAX_LEN];
  // Search card, return card types
  
  status = rfid.request(PICC_REQIDL, str);
  if(rfidWrongTimes[i]>=3&&rfidsState[i]==1){
        rfidWrongTimes[i] = 3;
        rfidsState[i]=0;
      
        if(mode=="debug"){
          tone(buzzerPIN,300,500);
        }
  }
  if (status != MI_OK)
  {
    rfidWrongTimes[i]++;
    states[i]=0;
    return;
  }
  // Show card type
  // rfid.showCardType(str);
  //Prevent conflict, return the 4 bytes Serial number of the card
  status = rfid.anticoll(str);
  if (status == MI_OK)
  {
    Serial.print("Card ");
    Serial.println(i);
    //  tone(buzzer,1000,1000);
    memcpy(serNum, str, 5);
    //rfid.showCardID(serNum);//show the card ID
    // Serial.print("Serial ");
    //  for(int b=0;b<5;b++){
    //     Serial.print(serNum[b]);
    //     Serial.print(", ");
    //  }
    //  Serial.println();
    //stex piti lini stugelu kod@

    for(int b=0;b<5;b++){

      if(serNum[b]!=rightRfids[i][b]){
        rfidWrongTimes[i]++;
        if(mode=="debug"){
          tone(buzzerPIN,1200,1000);
        }
        return;  
      }
        
    }
    if(rfidsState[i]==0){
      if(mode=="debug"){
        tone(buzzerPIN,1000,1000);
      }
      rfidsState[i] = 1;
      rfidWrongTimes[i]=0;//reset enq anum qani sxal angama exel
    }
    states[i]=1;
  }
  delay(100);
  rfid.halt(); //command the card into sleep mode 
}


void receiveEvent(int howMany) {
 while (0 <Wire.available()) {
    char c = Wire.read();      /* получаем байт как символ*/
    if(c=='d'){
        mode = "debug";
    }else if(c=='n'){
      mode = "game";
    }
  }
 Serial.println();             /* переходим на новую строку */
}

// Функция для извлечения любых отправляемых данных от мастера на шину
void requestEvent() {
  // Wire.beginTransmission(8);
  int c = 0;
  for(int i=0;i<8;i++){
    if(rfidsState[i]){
      c++;
    }
  }
  Serial.println(c);
  Wire.write(c+'0');
  //Wire.write("2");
  
  // Wire.endTransmission();
}



void setup()
{
  Serial.begin(9600); // Initialize serial communications with the PC
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)

  //rfid.begin(7, 5, 4, 3, 6, 2);     //rfid.begin(IRQ_PIN,SCK_PIN,MOSI_PIN,MISO_PIN,NSS_PIN,RST_PIN)
  //delay(100);//delay 1s
  //rfid.init(); //initialize the RFID
  pinMode(buzzerPIN,OUTPUT);
  Serial.print("hello");
  tone(buzzerPIN,200,500);

  

  Wire.begin(8);                /* задаем на шине i2c 8 адрес */
  Wire.onReceive(receiveEvent); /* регистрируем полученное событие */
  Wire.onRequest(requestEvent); /* регистрируем запрошенное событие */
}


void loop()
{
  for(int i=0;i<8;i++){
  checkRFID(i);  
  }
  if(mode=="debug"){
    EVERY_N_SECONDS(5){
      for(int i=0;i<8;i++){
        Serial.print("device ");
        Serial.print(i);
        Serial.print(" ");
        Serial.println(states[i]);
      }
    }
  }
}