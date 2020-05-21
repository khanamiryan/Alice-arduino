#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "Game_A.h"

#include <ArduinoOTA.h>
#include <Wire.h>
#include "I2C_Anything.h"
#include "FastLED.h"


#include "ESPRotary.h";


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


String myName = "bajakner"; //name of module

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
int lowest = 0;
int whenItsOpen = 150;
int highest = 180;



int changeamnt = 1;

// Timing for polling the encoder
unsigned long currentTime;
unsigned long lastTime;


// Pin definitions
const int pinA = D5;
const int pinB = D6;

// Storing the readings

boolean encA;
boolean encB;
boolean lastA = false;



/////////////////////////////////////////////////////////////////

#define ROTARY_PIN1	D5
#define ROTARY_PIN2	D6

/////////////////////////////////////////////////////////////////

ESPRotary r = ESPRotary(ROTARY_PIN1, ROTARY_PIN2);

/////////////////////////////////////////////////////////////////

void rotate(ESPRotary& r) {
   //Serial.println(r.getPosition());
   if (reading + changeamnt <= highest)
        {
        Serial.print("now - ");
        Serial.println(reading);
        reading = reading + (changeamnt*4); 
      }
   
}


void showDirection(ESPRotary& r) {
  Serial.println(r.directionToString(r.getDirection()));
}

void setup() {

  Serial.begin(115200); /* открываем серийный порт для дебаггинга */

  // pinMode(pinA, INPUT_PULLUP);
  // pinMode(pinB, INPUT_PULLUP);

  r.setChangedHandler(rotate);
  //r.setLeftRotationHandler(showDirection);
  r.setRightRotationHandler(showDirection);


  // Set up the timing of the polling
  currentTime = millis();
  lastTime = currentTime; 
  
  game.setup(myName, STASSID, STAPSK, TCPServer);
  game._myStatus = myStatus;
  ArduinoOTA.setHostname("bajakner");
   ArduinoOTA.begin();
  pinMode(dur,OUTPUT);
  digitalWrite(dur,LOW);
  Wire.begin(D1, D2); /* задаем i2c мост через контакты SDA=D1 и SCL=D2 на NodeMCU */
  //Wire.beginTransmission(8); /* Начинаем передачу на адресе 8 */
  //Wire.write('d');  /* Отправляем "hello Arduino" */
  //Wire.endTransmission();    /* прекращаем передачу */
}



/////////////////////////////////////////////////////////////////




int c;
int over = 1;
int lastrfidCount = 0;
void loop() {
  r.loop();
  game.run();
  ArduinoOTA.handle();



  EVERY_N_MILLISECONDS(10){
    if(reading>=whenItsOpen){
      if(reading>lowest){
        reading-=20;
      }
    }else{
      if(reading>lowest){
      reading-=1;
      }
    }
  }

  
  if(reading>=whenItsOpen&&over!=1){
     // game._myStatus = "kofeaxac-over";
      over = 1;//avartel e kofeiaaxac@, el mi uxarki
      game.sendToServer("kofeaxac-over");//srancic mek@
  }
     
  

  EVERY_N_SECONDS(1){
    if(reading<whenItsOpen&&over==1){
      // game._myStatus = "kofeaxac-notover";
      over = 0;
      game.sendToServer("kofeaxac-notover");//srancic mek@
      // game._myStatus = "standby";
    }
      Serial.print("noww - ");
      Serial.println(reading);
    
      Wire.requestFrom(8, 1); /* запрашиваем и считываем данные с 8 и 1 адреса slave устройства */
      if(Wire.available()){
          I2C_readAnything (c);
        
        if(c<0){
          c+=256;
        }
          
        Serial.print(c);
        
        if(c>=3){//sa stugel shat ushadir
          game._myStatus = "finished";
        }else{
          game._myStatus = "standby";
        }
        
        if(lastrfidCount!=c){
          game.sendToServer("rfidcount-"+String(c));//srancic mek@
        }else{
          EVERY_N_SECONDS(10){
            game.sendToServer("rfidcount-"+String(c));//srancic mek@
          }
        }
        lastrfidCount = c;
        // game.sendToServer('rfidcoun1t-'+c);
      }

      Serial.println();
  }
  
 
}