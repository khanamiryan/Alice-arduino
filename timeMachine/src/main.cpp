

#include <Arduino.h>

int luyser[6] = {22,24,26,28,30,32};
int tver[10] = {2,9,7,11,4,5,3,10,8,6};

void setup() {
  for(int i=0;i<6;i++){
    pinMode(luyser[i],OUTPUT);
  }
  for(int i=0;i<10;i++){
    pinMode(tver[i],OUTPUT);
  }
  
  Serial.begin(115000);

}

void loop() {
  Serial.println("Starting Loop");
      for(int i=0;i<6;i++){
        int luys = luyser[i];
        for(int x=0;x<6;x++){
          digitalWrite(luyser[x],LOW);
        }
        digitalWrite(luyser[i],HIGH);
        for(int j=0;j<10;j++){
          
          for(int z=0;z<10;z++){
              digitalWrite(tver[z],LOW);
          }
          digitalWrite(tver[j],HIGH);
          delay(400);
        }
      }
      
  }

