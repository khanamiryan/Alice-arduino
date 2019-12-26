#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "Game_A.h"
#include <ArduinoOTA.h>
#include "FastLED.h"

#include"rfid1.h"


#ifndef STASSID
  // #define STASSID "Ashot"
  // #define STAPSK  "arusik1987"
  
  // #define TCPServer "192.168.2.6"
  #define STASSID "Alice_Quest"
  #define STAPSK  "95242378"
  
  #define TCPServer "192.168.1.100"
//
//    #define STASSID "YoungLife"
//  #define STAPSK  "77877787"
//  
//  #define TCPServer "192.168.10.103"

#endif

const char* ssid     = STASSID;
const char* password = STAPSK;

const char* host = TCPServer;


String myName = "kofeaxac"; //name of module

String myStatus = "standby"; //turnedoff, standby, active, failed, finished 
String oldStatus;



int dur = D6;

class LGame: public Game_A{
  public:

    void additionalStatusCallback(String newStatus){ 
    


       if(newStatus.indexOf("special-")==0){
          
            
          
       } 
      
       
    }
    

    void goTurnedOff(){
      _myStatus = "turnedoff";
      //bacel dur@
      digitalWrite(dur,HIGH);
     
    }
    void goStandby(){
      _myStatus = "standby";
      digitalWrite(dur,LOW);
      //pakel dur@()

    }
    void goActive(){
      _myStatus = "active";
      
    }
    void goFailed(){
      _myStatus = "failed";
   
      
    }
    void goFinished(){
      
      _myStatus = "finished";
  

    }

    

};


LGame game;


int reading = 0;
int lowest = -120;
int highest = 2000;

int whenItsOpen = 1500;

int changeamnt = 1;

// Timing for polling the encoder
unsigned long currentTime;
unsigned long lastTime;


// Pin definitions
const int pinA = D3;
const int pinB = D4;

// Storing the readings

boolean encA;
boolean encB;
boolean lastA = false;



RFID1 rfid;//create a variable type of RFID1
int rfidscount = 3;
unsigned char serNum[5]; // array to store your ID



int IRQ_PIN = A0;
int SCK_PIN = D3;
int MOSI_PIN = D0;
//miso tarber ens
int SDA_PIN = D2;
int RST_PIN = D1;
int MISOS[3] = {D5,D6,D4};

int states[3] = {0,0,0}; //ete chist em hishum, amen depqum kpela qart te che, chisht te sxal karevor chi

String mode = "game";

const unsigned char rightRfids[4][5] = {//stex lcnel voroshacner@
  {9, 239, 91, 141, 48},
  {73, 88, 174, 139, 52},
  {137, 73, 205, 139, 134},
  {25, 3, 2, 140, 148}

  
  };

int rfidsState[3] = {0,0,0};
int rfidWrongTimes[3] = {0,0,0};
int howManyRfidsRight = 0;


//voroshel qarteri anunner@

bool checkRFID(int i){
  
  
    rfid.begin(IRQ_PIN,SCK_PIN,MOSI_PIN,MISOS[i],SDA_PIN,RST_PIN);



  delay(50);
  rfid.init();
  unsigned char status;
  unsigned char str[MAX_LEN];
  // Search card, return card types
  
  status = rfid.request(PICC_REQIDL, str);
//   Serial.print("rfidwrongtimes ");
//   Serial.print(i);
//   Serial.print(" ");
// Serial.println(rfidWrongTimes[i]);
  if(rfidWrongTimes[i]>=3){
    rfidWrongTimes[i] = 3;//vor shat chmecana tiv@ animast
    
    
    //  Serial.println("here");
      rfidsState[i]=0;
    if(rfidsState[i]==1){
     
    }
  }
  if (status != MI_OK)
  {
    rfidWrongTimes[i]++;
    states[i]=0;
    return false;
  }
  // Show card type
  // rfid.showCardType(str);
  //Prevent conflict, return the 4 bytes Serial number of the card
  status = rfid.anticoll(str);
  if (status == MI_OK)
  {
    states[i]=1;
    Serial.print("Card ");
    Serial.println(i);
    //  tone(buzzer,1000,1000);
    memcpy(serNum, str, 5);
    rfid.showCardID(serNum);//show the card ID
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
       
        return false;  //sxala rfidi kod@ durs a glais, ete , normala, sharunakuma
      }
        
    }
    if(rfidsState[i]==0){
    
      rfidsState[i] = 1;
      rfidWrongTimes[i]=0;//reset enq anum qani sxal angama exel
    }
    
  }
  delay(50);
  rfid.halt(); //command the card into sleep mode 
  return true;
}

void setup() {
  // set the two pins as inputs with internal pullups
  pinMode(pinA, INPUT_PULLUP);
  pinMode(pinB, INPUT_PULLUP);

  pinMode(D3, INPUT_PULLUP);
  pinMode(D4, INPUT_PULLUP);

  // Set up the timing of the polling
  currentTime = millis();
  lastTime = currentTime; 
  // Start the serial monitor for debugging
  Serial.begin(9600);


  // put your setup code here, to run once:

  
  game.setup(myName, STASSID, STAPSK, TCPServer);
  game._myStatus = myStatus;
  ArduinoOTA.setHostname("kofeaxac");
  ArduinoOTA.begin();
  
}

void loop() {
 
  game.run();
  ArduinoOTA.handle();
  
 
  // Check if it's time to read

    // read the two pins
  
    // check if A has gone from high to low

  EVERY_N_MILLISECONDS(10){
    encA = digitalRead(pinA);
    encB = digitalRead(pinB);
    if ((!encA)||(!encB))
    {
      if (reading + changeamnt <= highest)
        {
          reading = reading + changeamnt; 
        }
    }
    // store reading of A and millis for next loop
  }
  EVERY_N_MILLISECONDS(50){
      reading-=1;
  }

  if(reading>whenItsOpen){
      game._myStatus = "finished";
  }else{
      game._myStatus = "standby";
  }

  // if(game._myStatus == "standby"){
//todo mtacel, ete erkusn el kofeiaxacn u bajakner@ sra vra en linelu, vonc anel, vor irar chxangaren
 for(int i=0;i<rfidscount;i++){
   if(!checkRFID(i)){//ete mi hat@ chka, sax cikl@ break enq anum
     break;
   }
  }
  // if(mode=="debug"){
    EVERY_N_SECONDS(5){
      for(int i=0;i<rfidscount;i++){
        Serial.print("device ");
        Serial.print(i);
        Serial.print(" ");
        Serial.println(states[i]);
      }
    }

  
  // put your main code here, to run repeatedly:
}