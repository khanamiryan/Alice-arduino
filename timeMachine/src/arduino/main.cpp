

#include <Arduino.h>
#include "FastLED.h"
#include"rfid1.h"
#include <SPI.h>

#include <Ethernet2.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>



int luyser[6] = {24,32,30,28,22,26};
int tver[10] = {2,9,7,11,4,5,3,10,8,6};
int jamiknopka[6] = {38,42,40,44,46,48};
int jamiled[6] = {45,43,47,49,39,41};
int buzzerPIN = 13;
int  startButtonPin = 50;
int  startButtonLedPin = 51;
int DUR = 11;//pin of dur
int  lastMillis = 0;

int EthernetPin =A6;
int EthernetReset =A7;


int himikvaTver[6] = {1,2,3,4,5,6};
int naxordTver[6] = {0,0,0,0,0,0};

int buttonLastStates[6] = {0,0,0,0,0,0};
int mainButtonLastState = 0;
 
int day = 1;
int month = 1;
int year = 1;

int today[6] = {1,8,1,2,1,9};


RFID1 rfid;//create a variable type of RFID1
uchar serNum[5]; // array to store your ID

const uchar rightRfids[3][5] = {//stex lcnel voroshacner@
  {74,11,92,15,18},
  {201, 12, 224, 139, 174},
  {73, 88, 174, 139, 52}};

int rfidsState[3] = {0,0,0};
int rfidWrongTimes[3] = {0,0,0};
//rfid.begin(7, 5, 4, 3, 6, 2);     //rfid.begin(IRQ_PIN,SCK_PIN,MOSI_PIN,MISO_PIN,NSS_PIN,RST_PIN)
int IRQ_PIN = 25;
int SCK_PIN = 23;
int MOSI_PIN = 27;
//miso tarber ens
int SDA_PIN =31;
int RST_PIN = 29;

int RFIDCOUNT = 3;
int MOSIS[3] = {37,35,33};
void checkRFID(int i){
  EVERY_N_MILLISECONDS(500){
  
  rfid.begin(IRQ_PIN,SCK_PIN,MOSI_PIN,MOSIS[i],SDA_PIN,RST_PIN);

  EVERY_N_MILLISECONDS(200){
   

 
  rfid.init();
  uchar status;
  uchar str[MAX_LEN];
  // Search card, return card types
  status = rfid.request(PICC_REQIDL, str);
  if(rfidWrongTimes[i]>3&&rfidsState[i]==1){
        rfidsState[i]=0;
        tone(buzzerPIN,300,500);
  }
  if (status != MI_OK)
  {
    // Serial.print ("not ok ");
    // Serial.print (i);
    // Serial.print ("  - ");
    // Serial.println(status);
    rfidWrongTimes[i]++;
    return;
  }
  // Show card type
  // rfid.showCardType(str);
  //Prevent conflict, return the 4 bytes Serial number of the card
  status = rfid.anticoll(str);
  
  if (status == MI_OK)
  {
    
    memcpy(serNum, str, 5);
    // rfid.showCardID(serNum);//show the card ID
    //  rfid.showCardID(serNum);//show the card ID
        // Serial.println();
    // for(int b=0;b<5;b++){
    //     Serial.print(serNum[b]);
    //     Serial.print(", ");
    // } 
    // Serial.println();
    for(int b=0;b<5;b++){
      // Serial.println();
      // Serial.print(i);
      // Serial.print(" ");
      // Serial.print(serNum[b]);
      // Serial.print( " with ");
      // Serial.println(rightRfids[i][b]);
      if(serNum[b]!=rightRfids[i][b]){
        rfidWrongTimes[i]++;
        tone(buzzerPIN,10,1000);
        return;  
      }
        
    }
    if(rfidsState[i]==0){
      tone(buzzerPIN,1000,1000);
      rfidsState[i] = 1;
      rfidWrongTimes[i]=0;//reset enq anum qani sxal angama exel
    }
    // Serial.println("Right Card");
    
     
    
    EVERY_N_MILLISECONDS(100){
  
  rfid.halt(); //command the card into sleep mode 
    }
  }
  }
  delay(10);
  
  }
}


void resetTver(){
  for(int i=0;i<6;i++){
    himikvaTver[i] = 0;
  }
}

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
        delay(1);
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
          resetTver();
          
          response = true;
        }
      }
    }
    
  }
}



EthernetClient ethClient;
PubSubClient client(ethClient);

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic","hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress server(172, 16, 0, 2);

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}


void setup() {

  Serial.begin(9600); // Initialize serial communications with the PC
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)



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
  

  // // start the Ethernet connection:
  // Serial.println("Initialize Ethernet with DHCP:");
  // if (Ethernet.begin(mac) == 0) {
  //   Serial.println("Failed to configure Ethernet using DHCP");
  // } else {
  //   Serial.print("  DHCP assigned IP ");
  //   Serial.println(Ethernet.localIP());
  // }

  // client.setServer(server, 1883);
  // client.setCallback(callback);

  // // Ethernet.begin(mac, ip);
  // Ethernet.begin(mac);

  // // start the OTEthernet library with internal (flash) based storage
  // ArduinoOTA.begin(Ethernet.localIP(), "Arduino", "password", InternalStorage);

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
          lastMillis = millis();
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
    resetTver();
}



/*
  Countdown timer, flashes on and off
*/
void CountDown()
{
  for (int i=1;i<10;i++)
  {
    //  
    //apagauym nayel timingov
     //int i=1;
    // EVERY_N_MILLISECONDS_I( timingObj, i ) { 
    //     SetLEDs(255);
    //     delay(800/i);
    //     SetLEDs(0);
    //     delay(800/i);
    //     tone(buzzerPIN,1000,300);
    //     i++;
    // }
    // timingObj.setPeriod(i);
    

    SetLEDs(255);
    tone(buzzerPIN,200*i,400/i);
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
  for(int i=0;i<RFIDCOUNT;i++){
      checkRFID(i);
  }
  for(int i=0;i<RFIDCOUNT;i++){
    if(rfidsState[i]!=1){
      return false;
    }
  }
  return true;
}

void finishGame(){
    CountDown();

    digitalWrite(DUR,HIGH);
    tone(buzzerPIN,100,200);
  tone(buzzerPIN,1000,5000);
    delay(5000);
    resetGame();
    
}
void gameFailed(){
  tone(buzzerPIN,100,200);
  tone(buzzerPIN,130,300);
}
void loop() {

  // if (!client.connected()) {
  //   reconnect();
  // }
  // client.loop();

    EVERY_N_MILLISECONDS(500){
   for(int i=0;i<RFIDCOUNT;i++){
      checkRFID(i);
    }
    }
    // EVERY_N_MILLISECONDS(500){
    //   for(int i=0;i<3;i++){
    //   Serial.print("State of ");
    //   Serial.print(i);
    //   Serial.print(" - ");
    //   Serial.println(rfidsState[i]);
    //   Serial.print("wrong time ");
    //   Serial.println(rfidWrongTimes[i]);
    //   Serial.println();
    //   Serial.println();
      
    //   }
      
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
            lastMillis = millis();
            bool checkd = checkDate();
            bool checkfgrs = checkFigures();
            if(checkd&&checkfgrs){
              finishGame();
            }else{
              gameFailed();
            }
      }
    }
    
    if(lastMillis+30000000<millis()){
      resetGame();
      ScreenSaver();

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


