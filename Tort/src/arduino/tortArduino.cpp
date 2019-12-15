#include <Arduino.h>

void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600); /* открываем серийный порт для дебаггинга */

    pinMode(2, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(7, OUTPUT);

    pinMode(8,INPUT);
    pinMode(9,INPUT);
    pinMode(10,INPUT);
    pinMode(11,INPUT);
    pinMode(12,INPUT);
    pinMode(13,INPUT);
    Serial.println("Hello ");
}

void loop() {
  // put your main code here, to run repeatedly:
    
 digitalWrite(2,HIGH);
    digitalWrite(3,HIGH);
    digitalWrite(4,HIGH);
    digitalWrite(5,HIGH);
    digitalWrite(6,HIGH);
    digitalWrite(7,HIGH);
    Serial.print("read 8 ");
    Serial.println(digitalRead(8));  /* Отправляем "hello Arduino" */

    Serial.print("read 9 ");
    Serial.println(digitalRead(9));  /* Отправляем "hello Arduino" */

    Serial.print("read 10 ");
    Serial.println(digitalRead(10));  /* Отправляем "hello Arduino" */

    Serial.print("read 11 ");
    Serial.println(digitalRead(11));  /* Отправляем "hello Arduino" */

    Serial.print("read 12 ");
    Serial.println(digitalRead(12));  /* Отправляем "hello Arduino" */


    Serial.print("read 13 ");
    Serial.println(digitalRead(13));  /* Отправляем "hello Arduino" */
    
    delay(500);

}