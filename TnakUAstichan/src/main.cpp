#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "Game_A.h"

#include <ArduinoOTA.h>
#include <Wire.h>

#include "FastLED.h"





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


String myName = "tnak"; //name of module

String myStatus = "standby"; //turnedoff, standby, active, failed, finished 
String oldStatus;

int ASTICHAN =D2;
int TNAK = D1;


const int stateOff = 1;
const int stateOn = 0;



class LGame: public Game_A{
  public:

    void additionalStatusCallback(String newStatus){ 
    


    if(newStatus.indexOf("close")==0||newStatus.indexOf("open")==0){
        int st;
        
        if(newStatus.indexOf("open")==0){
          newStatus.replace("open","");
          st = stateOn;
        } 
        if(newStatus.indexOf("close")==0){
          newStatus.replace("close","");
          st = stateOff;
        }
        
        if(newStatus=="D1"){
          digitalWrite(D1,st);
          Serial.println("D1");
        }
        if(newStatus=="D2"){
          digitalWrite(D2,st);
          Serial.println("D2");
        }
        if(newStatus=="TNAK"){
          digitalWrite(TNAK,st);
          Serial.println("TNAK");
          
        }
         if(newStatus=="ASTICHAN"){
          digitalWrite(ASTICHAN,st);
          Serial.println("ASTICHAN");
        }
      }
      
       
    }
    

    void goTurnedOff(){
      _myStatus = "turnedoff";
      //bacel dur@
      
     
    }
    void goStandby(){
      _myStatus = "standby";
      
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



void setup() {

  Serial.begin(115200); /* открываем серийный порт для дебаггинга */


  pinMode(D2,OUTPUT);
  pinMode(D1,OUTPUT);
  digitalWrite(D2,LOW);
  digitalWrite(D1,HIGH);
  // Set up the timing of the polling

  game.setup(myName, STASSID, STAPSK, TCPServer);
  game._myStatus = myStatus;
  ArduinoOTA.setHostname("tnak");
  ArduinoOTA.begin();


  
  
}

void loop() {
  
 
  game.run();
  ArduinoOTA.handle();


}