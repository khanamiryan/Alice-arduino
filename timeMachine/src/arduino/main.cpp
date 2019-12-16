

#include <Arduino.h>
#include "FastLED.h"

int luyser[6] = {24,32,30,28,22,26};
int tver[10] = {2,9,7,11,4,5,3,10,8,6};
int jamiknopka[6] = {34,38,36,40,42,44};
int jamiled[6] = {41,39,43,45,35,37};
int  startButtonPin = 46;
int  startButtonLedPin = 47;
int DUR = 11;//pin of dur



/**
 * --------------------------------------------------------------------------------------------------------------------
 * Example sketch/program showing how to read data from more than one PICC to serial.
 * --------------------------------------------------------------------------------------------------------------------
 * This is a MFRC522 library example; for further details and other examples see: https://github.com/miguelbalboa/rfid
 *
 * Example sketch/program showing how to read data from more than one PICC (that is: a RFID Tag or Card) using a
 * MFRC522 based RFID Reader on the Arduino SPI interface.
 *
 * Warning: This may not work! Multiple devices at one SPI are difficult and cause many trouble!! Engineering skill
 *          and knowledge are required!
 *
 * @license Released into the public domain.
 *
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS 1    SDA(SS)      ** custom, take a unused pin, only HIGH/LOW required **
 * SPI SS 2    SDA(SS)      ** custom, take a unused pin, only HIGH/LOW required **
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 *
 */

// #include <SPI.h>
// #include <MFRC522.h>

// #define RST_PIN         10        // Configurable, see typical pin layout above
// #define SS_1_PIN        23         // Configurable, take a unused pin, only HIGH/LOW required, must be diffrent to SS 2
// #define SS_2_PIN        25          // Configurable, take a unused pin, only HIGH/LOW required, must be diffrent to SS 1
// #define SS_3_PIN        27          // Configurable, take a unused pin, only HIGH/LOW required, must be diffrent to SS 1

// #define NR_OF_READERS   1

// byte ssPins[] = {SS_1_PIN, SS_2_PIN,SS_3_PIN};

// MFRC522 mfrc522[NR_OF_READERS];   // Create MFRC522 instance.


// /**
//  * Helper routine to dump a byte array as hex values to Serial.
//  */
// void dump_byte_array(byte *buffer, byte bufferSize) {
//   for (byte i = 0; i < bufferSize; i++) {
//     Serial.print(buffer[i] < 0x10 ? " 0" : " ");
//     Serial.print(buffer[i], HEX);
//   }
// }
int himikvaTver[6] = {1,2,3,4,5,6};
int naxordTver[6] = {0,0,0,0,0,0};

int buttonLastStates[6] = {0,0,0,0,0,0};
int mainButtonLastState = 0;

int day = 1;
int month = 1;
int year = 1;

int today[6] = {1,6,1,2,1,9};

void displayDigits(){
  
      for(int x=0;x<10;x++){
            digitalWrite(tver[x],LOW);
        }
        for(int x=0;x<6;x++){
            digitalWrite(luyser[x],LOW);
        }
        delay(1);
      int n = 0;
     for(int x=0;x<6;x++){
        digitalWrite(tver[himikvaTver[x]],HIGH);
        digitalWrite(luyser[x],HIGH);
        delay(2);
        digitalWrite(tver[himikvaTver[x]],LOW);
        digitalWrite(luyser[x],LOW);
        delay(1);
        n = x;
        
        
     }
}



void displayDay(int day){
  int firstNumber = (int)(day/10);
  int secondNumber = day%10;
  himikvaTver[0] = firstNumber;
  himikvaTver[1] = secondNumber;
}

void displayMonth(int month){
  int firstNumber = (int)(month/10);
  int secondNumber = month%10;
  himikvaTver[2] = firstNumber;
  himikvaTver[3] = secondNumber;
}
void displayYear(int year){
  int firstNumber = (int)(year/10);
  int secondNumber = year%10;
  himikvaTver[4] = firstNumber;
  himikvaTver[5] = secondNumber;
}

void SetLEDs (int pattern)
{
  
  int bitPattern = 0x01;

  for(int i=0;i<6;i++)
  {
    if ((pattern & bitPattern) != 0)
      digitalWrite(jamiled[i],HIGH);
    else
      digitalWrite(jamiled[i],LOW);

    bitPattern = bitPattern << 1;
  }
  if ((pattern & bitPattern) != 0){
    digitalWrite(startButtonLedPin,HIGH);
  }
  else{
    digitalWrite(startButtonLedPin,LOW);
  }

    bitPattern = bitPattern << 1;
  
}

/*
  Check to see if any button is pressed
*/
bool CheckButtons()
{
  for (int i=0; i< 6; i++)
  {
    if(digitalRead(jamiknopka[i]) > 0)
    {
      return true;
    }
  }
  if(digitalRead(startButtonPin)>0){
    return true;
  }
  return false;
}

bool response = false;

void ScreenSaver()
{
  // int screenSaver = random(3);
  
  
  while (!response)
  {
    EVERY_N_MILLISECONDS(500){
      for(int i=0;i<6;i++){
        himikvaTver[i] = random(9);
      }
    }
    for(int i=0 ; i < 100 ; i++)
    {
      int rand = random(256);
      displayDigits();
      for (int loop = 0; loop<10; loop++)
      {   
        if (!CheckButtons())
        {
          EVERY_N_MILLISECONDS(100){
              SetLEDs(rand);
          }
        }
        else
        {
          while(CheckButtons());
          
          SetLEDs(0);
          
          response = true;
        }
      }
    }
    
  }
}


void setup() {

  Serial.begin(9600); // Initialize serial communications with the PC
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)

  // SPI.begin();        // Init SPI bus

  // for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {
  //   mfrc522[reader].PCD_Init(ssPins[reader], RST_PIN); // Init each MFRC522 card
  //   Serial.print(F("Reader "));
  //   Serial.print(reader);
  //   Serial.print(F(": "));
  //   mfrc522[reader].PCD_DumpVersionToSerial();
  // }
  for(int i=0;i<6;i++){
    pinMode(luyser[i],OUTPUT);
  }
  for(int i=0;i<10;i++){
    pinMode(tver[i],OUTPUT);
  }

  for(int i=0;i<6;i++){
    pinMode(jamiknopka[i],OUTPUT);
  }
  for(int i=0;i<6;i++){
  pinMode(jamiled[i],INPUT);
  }
  
  pinMode(startButtonPin,INPUT);
  pinMode(startButtonLedPin,OUTPUT);
  randomSeed(analogRead(A3));
  Serial.println("Hello");
  

}

void dayDown(){
  
  if(day<=1){
    day=31;
  }else{
    day--;
  }

}
void dayUp(){
  
  if(day>=31){
    day=1;
  }else{
    day++;
  }
}
void monthUp(){
   if(month>=12){
    month=1;
  }else{
    month++;
  }
}
void monthDown(){
 if(month<=1){
    month=12;
  }else{
    month--;
  }
}


void yearUp(){
   if(year>=99){
    year=1;
  }else{
    year++;
  }
}
void yearDown(){
 if(year<=1){
    year=99;
  }else{
    year--;
  }
}




void readButtons(){
   for(int i=0;i<6;i++){
      int st = digitalRead(jamiknopka[i]);

      digitalWrite(jamiled[i],st);
      if(buttonLastStates[i]!=st){//knopkayi naxkin vichakn enq stugum, ete, sexmaca u naxkinum sexmac
        buttonLastStates[i] = st;  
        if(st==1){
          switch (i)
          {
          case 0:
            dayDown();
            break;
          case 1:
            dayUp();
            break;
          case 2:
            monthDown();
            break;
          case 3:
            monthUp();
            break;
          case 4:
            yearDown();
            break;
          case 5:
            yearUp();
            break;          
          default:
            break;
          }
          displayDay(day);
          displayMonth(month);
          displayYear(year);
        }
      }

    }
}

void resetGame(){
    response = false;
    digitalWrite(DUR,LOW);
}



/*
  Countdown timer, flashes on and off
*/
void CountDown()
{
  for (int i=1;i<10;i++)
  {
    SetLEDs(255);
    delay(800/i);
    SetLEDs(0);
    delay(800/i);
  }
  
  delay(500);
}


bool checkDate(){

  for(int i=0;i<6;i++){
    if(himikvaTver[i]!=today[i]){
      return false;
    }
  }
  return true;
}
bool checkFigures(){

  return true;
}

bool finishGame(){
    CountDown();

    digitalWrite(DUR,HIGH);
    resetGame();
    
}
bool gameFailed(){

}
void loop() {

    

        // for(int x=0;x<6;x++){
        //   digitalWrite(luyser[x],LOW);
        // }
    
    

    ScreenSaver();

    
    readButtons();
    // for(int i=0;i<6;i++){
    //   int st = digitalRead(jamiknopka[i]);

    //   digitalWrite(jamiled[i],st);


    // }
    int bt = digitalRead(startButtonPin);
    digitalWrite(startButtonLedPin,bt);
    if(mainButtonLastState!=bt){
      mainButtonLastState = bt;
      if(bt==1){//sexmel en glxavor knopkayin
            bool checkd = checkDate();
            bool checkfgrs = checkFigures();
            if(checkd&&checkfgrs){
              finishGame();
            }else{
              gameFailed();
            }
      }
    }
    



  // for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {
  //   // Look for new cards

  //   if (mfrc522[reader].PICC_IsNewCardPresent() && mfrc522[reader].PICC_ReadCardSerial()) {
  //     Serial.print(F("Reader "));
  //     Serial.print(reader);
  //     // Show some details of the PICC (that is: the tag/card)
  //     Serial.print(F(": Card UID:"));
  //     dump_byte_array(mfrc522[reader].uid.uidByte, mfrc522[reader].uid.size);
  //     Serial.println();
  //     Serial.print(F("PICC type: "));
  //     MFRC522::PICC_Type piccType = mfrc522[reader].PICC_GetType(mfrc522[reader].uid.sak);
  //     Serial.println(mfrc522[reader].PICC_GetTypeName(piccType));

  //     // Halt PICC
  //     mfrc522[reader].PICC_HaltA();
  //     // Stop encryption on PCD
  //     mfrc522[reader].PCD_StopCrypto1();
  //   } //if (mfrc522[reader].PICC_IsNewC
  // } //for(uint8_t reader


      // for(int i=0;i<6;i++){
      //   int luys = luyser[i];
      //   for(int x=0;x<6;x++){
      //     digitalWrite(luyser[x],LOW);
      //   }
      //   digitalWrite(luyser[i],HIGH);
      //   for(int j=0;j<10;j++){
          
      //     for(int z=0;z<10;z++){
      //         digitalWrite(tver[z],LOW);
      //     }
      //     digitalWrite(tver[j],HIGH);
      //     delay(400);
      //   }
      // }

      displayDigits();

  }


