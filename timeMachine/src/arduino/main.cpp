

#include <Arduino.h>
#include "FastLED.h"
#include"rfid1.h"
#include <SPI.h>

#include <Ethernet2.h>
//#include <ArduinoOTA.h>
#include <PubSubClient.h>
#include <avr/wdt.h>



int luyser[6] = {24,32,30,28,22,26};
int tver[10] = {2,9,7,11,4,5,3,10,8,6};
int jamiknopka[6] = {38,42,40,44,46,48};
int jamiled[6] = {45,43,47,49,39,41};
int buzzerPIN = 13;
int  startButtonPin = A8;
int  startButtonLedPin = A11;
//int DUR = 12;//pin of dur
unsigned long  lastMillis = 0;
unsigned long stanbyModeTime = 600000;

int EthernetPin =15;
int EthernetReset =14;//??karoxa petq chga

// bool screenSaverTime = true;
int himikvaTver[6] = {1,1,1,1,1,1};
//int naxordTver[6] = {0,0,0,0,0,0};

int buttonLastStates[6] = {0,0,0,0,0,0};
int mainButtonLastState = 0;
 
int day = 1;
int month = 1;
int year = 1;

int today[6] = {1,8,1,2,1,9};

void clientLoop();
EthernetClient ethClient;
PubSubClient client(ethClient);
String clientID = "timeMachine";
String  _myStatus = "standby";
String oldStatus = _myStatus;
void statusCallback(String newStatus);
void sendMyStatus();

bool callbackAlready;

RFID1 rfid;//create a variable type of RFID1
uchar serNum[5]; // array to store your ID


const uchar rightRfids[3][5] = {//stex lcnel voroshacner@
  {74,11,92,15,18},
  {26, 18, 97, 15, 102},
  {250, 71, 88, 15, 234}
  
  };
//233, 243, 67, 140, 213,
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


int clientlooped = 0;
void resetGame();
void finishGame();
void gameFailed();

bool sendToServer(String message);


void checkRFID(int i){
  
  rfid.begin(IRQ_PIN,SCK_PIN,MOSI_PIN,MOSIS[i],SDA_PIN,RST_PIN);
  delay(100);

 
  rfid.init();
  uchar status;
  uchar str[MAX_LEN];
  // Search card, return card types
  status = rfid.request(PICC_REQIDL, str);
  // if(rfidWrongTimes[i]>=0){
  //   rfidWrongTimes[i] = 1;
 
  // }
  if (status != MI_OK)
  {
    //rfidWrongTimes[i]++;
    //if(rfidsState[i]==1){
       rfidsState[i]=0;
       tone(buzzerPIN,300,500);
     //}
    return;
  }
  // Show card type
  // rfid.showCardType(str);
  //Prevent conflict, return the 4 bytes Serial number of the card
  status = rfid.anticoll(str);
  
  if (status == MI_OK)
  {
    
    memcpy(serNum, str, 5);
     ///rfid.showCardID(serNum);//show the card ID
    //  rfid.showCardID(serNum);//show the card ID
        // Serial.println();
    for(int b=0;b<5;b++){
        Serial.print(serNum[b]);
        Serial.print(", ");
    } 
    Serial.println();
    // Serial.println();
    bool sa = false;
    
    for(int b=0;b<5;b++){

      // if(serNum[b]!=rightRfids[i][b]){//ete lriv nuynenq uzum
      //   //rfidWrongTimes[i]++;
      //   //tone(buzzerPIN,1000,1000);
      //   return;  
      // }
      for(int y=0;y<3;y++){//ete lyuboy@ lyuboy tex drvi
        if(serNum[b]==rightRfids[y][b]){
            // Serial.println("continue exav");
            sa = true;
            break;
        }
        sa = false;

      }
        
    }
    //if(rfidsState[i]==0){
      if(sa == true){
        tone(buzzerPIN,1000,1000);
        rfidsState[i] = 1;
      }
      //rfidWrongTimes[i]=0;//reset enq anum qani sxal angama exel
    //}
    // Serial.println("Right Card");
    
    
  
  rfid.halt(); //command the card into sleep mode 
  }
  delay(100);
  
  
}


void resetTver(){
  for(int i=0;i<6;i++){
    himikvaTver[i] = 1;
  }
  day = 1;
  month = 1;
  year = 1;
}

void displayDigits(){
  
  for(int x=0;x<10;x++){
        digitalWrite(tver[x],LOW);
    }
  for(int x=0;x<6;x++){
      digitalWrite(luyser[x],LOW);
  }
  delay(1);
  //int n = 0;
  for(int x=0;x<6;x++){
    digitalWrite(tver[himikvaTver[x]],HIGH);
    digitalWrite(luyser[x],HIGH);
    delay(1);
    digitalWrite(tver[himikvaTver[x]],LOW);
    digitalWrite(luyser[x],LOW);
    delay(1);
    // n = x;
    
    
  }
}
void turnoffDigits(){
  for(int x=0;x<10;x++){
        digitalWrite(tver[x],LOW);
    }
  for(int x=0;x<6;x++){
      digitalWrite(luyser[x],LOW);
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
  for (int i=0; i<6; i++)
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

void ScreenSaver(int time = 350)
{
  // int screenSaver = random(3);
  
  
  // if (_myStatus=="standby")
  // {
    
    
    EVERY_N_MILLISECONDS(350){
      for(int i=0;i<6;i++){
        himikvaTver[i] = random(9);
      }
    }
    for(int i=0 ; i < 100 ; i++)
    {
      int rand = random(256);
        displayDigits();
        clientLoop();
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
          //if(CheckButtons());
          
          
          _myStatus = "active";
          return;
        }
      }
    }
    
  //}
}







 void goTurnedOff(){
      _myStatus = "turnedoff";

    turnoffDigits();
    }
    void goStandby(){
      _myStatus = "standby";
      resetGame();

    }
     void goActive(){
      _myStatus = "active";
      SetLEDs(0);
      resetTver();
      tone(buzzerPIN,100,100);
    }
     void goFailed(){
       if(_myStatus!="failed"){
        _myStatus = "failed";
        Serial.println("failed");
        gameFailed();
       }
    }
     void goFinished(){
       if(_myStatus!="finished"){
          _myStatus = "finished";
          // Serial.println("goFinishedym enq");
          finishGame();
       }
      
    }

 void additionalStatusCallback(String newStatus){
        if(newStatus.indexOf("date-")==0){
          newStatus.replace("date-","");
          sendToServer("innerDate-"+newStatus);
          // Serial.println(newStatus);
          char buf[7];
          newStatus.toCharArray(buf, 7);
          char *p = buf;
          // char *str;
          
          // newStatus.toCharArray(p, 6);
        
          // "201219"

          // Serial.print (buf);
          // Serial.print (p);
          for(int i=0;i<6;i++){
              
             today[i] = (int)buf[i]-48;
             
            
          }
          
          Serial.println();
          
          //sendToServer("speed-"+ String(DIFF));
        } 
    }

String sendS = "toServer/"+clientID;
bool sendToServer(String message){  
  
  client.publish(sendS.c_str(), message.c_str() );
  
  return true;
}

 void statusDaemon(bool isCallback=false){
      if(_myStatus!=oldStatus){//ete poxvela status@
        // Serial.print("status changed from ");
        // Serial.print(oldStatus);
        // Serial.print(" to ");
        // Serial.println(_myStatus);
        if(!isCallback){
          statusCallback(_myStatus);
        }
        oldStatus = _myStatus;
        sendMyStatus();
      }
      
    }

  String subscribeS = "toDevice/"+clientID;
  String  actionDevice= subscribeS+String("/action");
  void subscribeToServer(){
      
        if(client.subscribe("toDevice/ALLICE")&&client.subscribe((char*) subscribeS.c_str())&&client.subscribe((char*) actionDevice.c_str() )){
           Serial.print("subscribed to ALLICE and ");
          Serial.println(subscribeS);
        }
        
        
        
    }

    void callback(char* topic, byte* payload, unsigned int length) {
 
        
        payload[length] = '\0';

      


        if (strcmp(topic,"toDevice/ALLICE")==0) {
            statusCallback(String((char *)payload));
        }
        if (strcmp(topic,subscribeS.c_str())==0) {
          statusCallback(String((char *)payload));
        }
        if (strcmp(topic,actionDevice.c_str())==0) {
          // actionCallback(String((char *)payload));
        }
        

        //if(strcmp ("abc", str) == 0))


      }
  
      // void actionCallback(String action){

        
      // }
      // bool callbackAlready;
     void statusCallback(String newStatus){
      //Serial.println(newStatus);
      callbackAlready = true;
      if(newStatus=="turnedoff"){
        goTurnedOff();
      }else if(newStatus=="standby"){
        goStandby();
      }else if(newStatus=="active"){
        goActive();
      }else if(newStatus=="failed"){
        goFailed();
      }else if(newStatus=="finished"){
        goFinished();
      }
     
       if(newStatus=="status"){
        sendMyStatus();
      }
      
      additionalStatusCallback(newStatus);


      statusDaemon(true);
      
    }
    
    
    void sendMyStatus(){
       sendToServer(_myStatus);
    }


    

 void reconnect() {
     while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
//    String clientID = "ESP8266Client-";
//    clientID += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientID.c_str())) {
      Serial.print("connected by clentName ");
      Serial.println(clientID);
      
      // Once connected, publish an announcement...
      //client.publish("outTopic", "hello world");
      // ... and resubscribe
      //client.subscribe("inTopic");
      subscribeToServer();
      sendToServer("date");//uxarkum enq date vor het stananq daten
      
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
// IPAddress server(172, 16, 0, 2);
IPAddress server(192, 168, 1, 100);



void setup() {

  wdt_enable(WDTO_8S);
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
  

  // start the Ethernet connection:
  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
  } else {
    Serial.print("  DHCP assigned IP ");
    Serial.println(Ethernet.localIP());
  }

   client.setServer(server, 1883);
   client.setCallback(callback);

  // // Ethernet.begin(mac, ip);
   Ethernet.begin(mac);

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



int st;
void readButtons(){
   for(int i=0;i<6;i++){
      st = digitalRead(jamiknopka[i]);
      if(st==1){
        digitalWrite(jamiled[i],LOW);
      }else{
        digitalWrite(jamiled[i],HIGH);
      }
      if(buttonLastStates[i]!=st){//knopkayi naxkin vichakn enq stugum, ete, sexmaca u naxkinum sexmac
        buttonLastStates[i] = st;  
        lastMillis = millis();
        if(st==1){
          tone(buzzerPIN,1000-(111*i),250);
          
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
    //_myStatus = "standby";
    //digitalWrite(DUR,LOW);
    resetTver();
    
    // screenSaverTime =  true;
    for(int i=0;i<3;i++){
        rfidsState[i] = 0;
        rfidWrongTimes[i] = 0;
    }

    
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
    tone(buzzerPIN,205*i,450/i);
    delay(800/i);
    SetLEDs(0);
    delay(800/i);
    wdt_reset();//Watchdog
  }
  
  delay(500);
}


bool checkDate(){
  
   String wrongDate = "";
    for(int u = 0;u<6;u++){
      wrongDate+=String(himikvaTver[u]);
    }


  for(int i=0;i<6;i++){
    if(himikvaTver[i]!=today[i]){   
     sendToServer("dateis-wrong-"+wrongDate);
      return false;
    }
  }
  sendToServer("dateis-wrong-"+wrongDate);
  sendToServer("dateis-right");
  return true;
}

int ch;
bool checkFigures(){
  
  for(int i=0;i<RFIDCOUNT;i++){
      tone(buzzerPIN,100,200);
      checkRFID(i);
  }
  ch = 0;
  for(int i=0;i<RFIDCOUNT;i++){
      if(rfidsState[i]==1){
          ch++;
      }

  }
  
  
  sendToServer("lastRightRfids-"+String(ch));
  for(int i=0;i<RFIDCOUNT;i++){
    if(rfidsState[i]!=1){
      return false;
    }
  }
  return true;
}

void finishGame(){
    // Serial.println("mtel em finish game");
    _myStatus = "finished";
    sendMyStatus();
    CountDown();

   
    tone(buzzerPIN,500,200);
    delay(200);
    tone(buzzerPIN,500,200);
    displayDigits();
    delay(200);
    displayDigits();
    // ScreenSaver(350);
    tone(buzzerPIN,500,200);
    delay(200);
    tone(buzzerPIN,800,150);
    displayDigits();
    // ScreenSaver(200);
    delay(150);
    tone(buzzerPIN,500,500);
    
    delay(500);
    //ScreenSaver(150);
    tone(buzzerPIN,600,1000);
  //digitalWrite(DUR,HIGH);
    tone(buzzerPIN,1000,3000);

    delay(3000);
    tone(buzzerPIN,1100,15000);
    // ScreenSaver(100);
    wdt_reset();//Watchdog
    delay(3000);
    wdt_reset();//Watchdog
    delay(3000);
    wdt_reset();//Watchdog
    delay(3000);
    wdt_reset();//Watchdog
    delay(3000);
    wdt_reset();//Watchdog
    delay(3000);
    
    
    
    _myStatus = "standby";
    //resetGame();
    
}
void gameFailed(){
  tone(buzzerPIN,100,200);
  tone(buzzerPIN,130,300);
  tone(buzzerPIN,200,300);
  tone(buzzerPIN,250,350);
}

void clientLoop(){
  if(clientlooped==0){
    clientlooped = 1;
    // ArduinoOTA.poll();
    if (!client.connected()) {
      reconnect();
    }    
    client.loop();
  }
  statusDaemon();
  wdt_reset();//Watchdog
  
      
}

unsigned long now;
int stu;
int btu;

void loop() {
  // EVERY_N_SECONDS(15){
  // Serial.print("Status - ");
  //   Serial.println(_myStatus);
  // }

    clientlooped = 0;
    
    clientLoop();

    displayDigits();
    readButtons();
    if(_myStatus=="standby"){
        ScreenSaver();
    }
    else if(_myStatus!="standby"){
      if(_myStatus=="failed"){
          _myStatus="active";
          oldStatus = "active";//hack
      }
      
      
      
      for(int i=0;i<6;i++){
        stu = digitalRead(jamiknopka[i]);
        if(st==1){
          digitalWrite(jamiled[i],LOW);
        }else{
          digitalWrite(jamiled[i],HIGH);
        }
        
      }

      btu = digitalRead(startButtonPin);//glxavor knopken
      
      if(btu==1){
          digitalWrite(startButtonLedPin,LOW);
      }else{
          digitalWrite(startButtonLedPin,HIGH);
      }

      if(mainButtonLastState!=btu){
        mainButtonLastState = btu;
        lastMillis = millis();
        //Serial.println("newLastmillis");
        if(btu==1){//sexmel en glxavor knopkayin

              tone(buzzerPIN,500,100);
              
              bool checkd = checkDate();
               
              bool checkfgrs = checkFigures();
              if(checkd&&checkfgrs){
                //_myStatus = "finished";
                //Serial.println("finished -a ");
                goFinished();
              }else{
                _myStatus = "failed";
                // Serial.print("failed - ");
                // Serial.print("checkd is ");
                // Serial.print(checkd);
                // Serial.print(", checkgigures - ");
                // Serial.println(checkfgrs);
                if(checkd){
                    tone(buzzerPIN,500,100);
                }else if(checkfgrs){
                      tone(buzzerPIN,820,100);
                      delay(100);
                      tone(buzzerPIN,300,200);
                }
              
              }
        }
  
      }
      
      now= millis();
   
      if(now-lastMillis>stanbyModeTime){

        //  Serial.print("lastMillis - ");
          // /Serial.println(lastMillis);
          // Serial.print("lastMillis + ");
          // Serial.println(lastMillis+1000*60*5);
          // Serial.print("now - ");
          // Serial.println(now);
          Serial.print("5rope");
          _myStatus = "standby";
          lastMillis =  now;
          //resetGame();
          // ScreenSaver();
        }
  }


  
      

  }


