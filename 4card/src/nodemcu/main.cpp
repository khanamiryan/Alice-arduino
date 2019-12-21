#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "Game_A.h"

#include <ArduinoOTA.h>
#include <Wire.h>
#include "I2C_Anything.h"
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


String myName = "cards4"; //name of module

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


void setup() {

  Serial.begin(9600); /* открываем серийный порт для дебаггинга */

  
  
  game.setup(myName, STASSID, STAPSK, TCPServer);
  game._myStatus = myStatus;
  ArduinoOTA.setHostname("dur4");
   ArduinoOTA.begin();
  pinMode(dur,OUTPUT);
  digitalWrite(dur,LOW);
  Wire.begin(D1, D2); /* задаем i2c мост через контакты SDA=D1 и SCL=D2 на NodeMCU */
  //Wire.beginTransmission(8); /* Начинаем передачу на адресе 8 */
  //Wire.write('d');  /* Отправляем "hello Arduino" */
  //Wire.endTransmission();    /* прекращаем передачу */
}
int c;
void loop() {
  
  game.run();
  ArduinoOTA.handle();


  EVERY_N_SECONDS(1){
    
      Wire.requestFrom(8, 1); /* запрашиваем и считываем данные с 8 и 1 адреса slave устройства */
      if(Wire.available()){
          I2C_readAnything (c);
        
        if(c<0){
          c+=256;
        }
          
        Serial.print(c);
        if(c>=4){//sa stugel shat ushadir
          game._myStatus = "finished";
        }else{
          game._myStatus = "standby";
        }

        game.sendToServer("rfidcount-"+String(c));//srancic mek@
        // game.sendToServer('rfidcoun1t-'+c);
      }

      Serial.println();
  }
  
 
}