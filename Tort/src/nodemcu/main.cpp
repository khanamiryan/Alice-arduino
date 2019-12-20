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


String myName = "tort"; //name of module

String myStatus = "standby"; //turnedoff, standby, active, failed, finished 
String oldStatus;

int winnerScore = 50;

class LGame: public Game_A{
  public:

    void additionalStatusCallback(String newStatus){ 

       if(newStatus.indexOf("winnerScore-")==0){
          newStatus.replace("winnerScore-","");
          winnerScore = newStatus.toInt();
       } 
             
    }
    

    void goTurnedOff(){
      _myStatus = "turnedoff";
     
    }
    void goStandby(){
      _myStatus = "standby";
    

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

bool haveData = false;
void receiveEvent(int howMany) {
  Serial.println("inreceive");
    
    while(Wire.available()){
      
      
      
      //Serial.print(c);
    }
    Serial.println();

  
  
           /* переходим на новую строку */
}
void setup() {

   Serial.begin(9600); /* открываем серийный порт для дебаггинга */

  
  
  game.setup(myName, STASSID, STAPSK, TCPServer);
  game._myStatus = myStatus;

    ArduinoOTA.setHostname("Tort");
   ArduinoOTA.begin();
   
  
  Wire.begin(D1, D2); /* задаем i2c мост через контакты SDA=D1 и SCL=D2 на NodeMCU */
  //Wire.beginTransmission(8); /* Начинаем передачу на адресе 8 */
  //Wire.write('d');  /* Отправляем "hello Arduino" */
  //Wire.endTransmission();    /* прекращаем передачу */

  //Wire.onReceive(receiveEvent); /* регистрируем полученное событие */


}
int score;
int lastScore;
void loop() {

  game.run();
  ArduinoOTA.handle();
    EVERY_N_MILLISECONDS(100){
      
       Wire.requestFrom(8, 1); /* запрашиваем и считываем данные с 8 и 1 адреса slave устройства */
      if(Wire.available()){
        lastScore = score;
//          char c = Wire.read();
        I2C_readAnything (score);
        
        if(score<0){
          score+=256;
        }
        // Serial.println(c);
        EVERY_N_MILLISECONDS(1000){
          game.sendToServer("score-"+String(score));
        }
      }
    
      //Serial.println();
    }
    if(lastScore<score){//ete ave  anume
      if(score>0&&score<winnerScore){
        game._myStatus = "active";
      }
      // else if(score>=winnerScore){
      //   game._myStatus = "finished";
      // }
    }
      if(score==0){
        if(lastScore>0){
          if(lastScore>=winnerScore){
            game._myStatus = "finished";

          }else{
            game._myStatus = "failed";
          }
        }else{
          game._myStatus = "standby";
        }
      }
    
//  Wire.beginTransmission(8); /* Начинаем передачу на адресе 8 */
//  Wire.write("Hello Arduino");  /* Отправляем "hello Arduino" */
//  Wire.endTransmission();    /* прекращаем передачу */


  
}

///95242378