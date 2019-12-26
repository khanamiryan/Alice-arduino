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


String myName = "darak2"; //name of module

String myStatus = "standby"; //turnedoff, standby, active, failed, finished 
String oldStatus;

int DARAK =D5;
int PATUHAN = D1;
int DUR = D4;


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
        if(newStatus=="D4"){
          digitalWrite(D4,st);
          Serial.println("D4");
        }
        if(newStatus=="D5"){
          digitalWrite(D5,st);
          Serial.println("D5");
        }
        if(newStatus=="DARAK"){
          digitalWrite(DARAK,st);
          
          
        }
        if(newStatus=="PATUHAN"){
          digitalWrite(PATUHAN,st);
          
        }
        if(newStatus=="DUR"){
          digitalWrite(DUR,st);
          
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


  pinMode(DARAK,OUTPUT);
  pinMode(PATUHAN,OUTPUT);
  pinMode(DUR,OUTPUT);

  digitalWrite(DARAK,stateOff);
  digitalWrite(PATUHAN,stateOff);
  digitalWrite(DUR,stateOff);
  // Set up the timing of the polling

  game.setup(myName, STASSID, STAPSK, TCPServer);
  game._myStatus = myStatus;
  ArduinoOTA.setHostname("darak2");
  ArduinoOTA.begin();


  
  
}

void loop() {
  
 
  game.run();
  ArduinoOTA.handle();


}