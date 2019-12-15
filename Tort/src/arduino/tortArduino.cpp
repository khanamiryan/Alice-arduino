#include <Arduino.h>

#include <Wire.h>

int tortButtonsLed[] = {2,3,4,5,6,7};
int tortButtons[] = {13,12,11,10,9,8};
int NUMBER_BUTTONS=6;  

int score = 0;


const byte ScreenSaverPattern[72] = {
  0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 
  0x80, 0x81, 0x82, 0x84, 0x88, 0x90, 0xA0, 0xC0, 
  0xC1, 0xC2, 0xC4, 0xC8, 0xD0, 0xE0, 0xE1, 0xE2, 
  0xE4, 0xE8, 0xF0, 0xF1, 0xF2, 0xF4, 0xF8, 0xF9,
  0xFA, 0xFC, 0xFD, 0xFE, 0xFF, 0xFE, 0xFD, 0xFB,
  0xF7, 0xEF, 0xDF, 0xBF, 0x7F, 0x7E, 0x7D, 0x7B,
  0x77, 0x6F, 0x5F, 0x3F, 0x3E, 0x3D, 0x3B, 0x37,
  0x2F, 0x1F, 0x1E, 0x1D, 0x1B, 0x17, 0x0F, 0x0E,
  0x0D, 0x0B, 0x07, 0x06, 0x05, 0x03, 0x02, 0x01
};

const byte ScreenSaverPatternLines[28] = {
  0x49, 0x24, 0xA2, 0x24, 0x49, 0x03, 0x04, 0x18,
  0x20, 0xC0, 0x20, 0x18, 0x04, 0x03, 0xA2, 0x24,
  0x49, 0x24, 0xA2, 0xC0, 0x20, 0x18, 0x04, 0x03,
  0x04, 0x18, 0x20, 0xC0
};

/*
  Set a button
*/
void SetLED (int button, int led)
{
  if (button != HIGH)
    digitalWrite(led, LOW);
  else
    digitalWrite(led, HIGH);
}

/*
  Set the LED pattern using bits
  0000 0001 - LED 01
  0000 0010 - LED 02
  0000 0100 - LED 03
  0000 1000 - LED 04
  0001 0000 - LED 05
  0010 0000 - LED 06

*/
void SetLEDs (int pattern)
{
  
  int bitPattern = 0x01;
  
  for(int i=0;i<NUMBER_BUTTONS;i++)
  {
    if ((pattern & bitPattern) != 0)
      SetLED(HIGH, tortButtonsLed[i]);
    else
      SetLED(LOW, tortButtonsLed[i]);

    bitPattern = bitPattern << 1;
  }
}

/*
  Check to see if any button is pressed
*/
bool CheckButtons()
{
  for (int i=0; i< NUMBER_BUTTONS; i++)
  {
    if(digitalRead(tortButtons[i]) > 0)
    {
      return true;
    }
  }
  return false;
}


/*
  Screen saver
*/
bool ScreenSaverBuildUp()
{
  int i;
  int position = 0;
  for (i = 0; i < 72; i++)
  {
    // Loop 10 times, but wait 1/10 sec each time.  More responsive than waiting a second
    // each time.
    for (int loop = 0; loop<10; loop++)
    {
      if (!CheckButtons())
      {
        SetLEDs(ScreenSaverPattern[i]);
        delay(100);
      }
      else
      {
        while(CheckButtons());
        
        SetLEDs(0);
        
        return true;
      }
    }
  }
  
  return false;
}



/*
  Screen saver
*/
bool ScreenSaverLines()
{
  int i;
  int position = 0;
  for (i = 0; i < 28; i++)
  {
    // Loop 10 times, but wait 1/10 sec each time.  More responsive than waiting a second
    // each time.
    for (int loop = 0; loop<10; loop++)
    {
      if (!CheckButtons())
      {

        SetLEDs(ScreenSaverPatternLines[i]);
        delay(100);
      }
      else
      {
        while(CheckButtons());
        
        SetLEDs(0);
        
        return true;
      }
    }
  }
  
  return false;
}



/*
  Randomly display lights
*/
bool ScreenSaverRandom()
{
  for(int i=0 ; i < 100 ; i++)
  {
    int rand = random(256);
    for (int loop = 0; loop<10; loop++)
    {
      if (!CheckButtons())
      {
        SetLEDs(rand);
        delay(100);
      }
      else
      {
        while(CheckButtons());
        
        SetLEDs(0);
        
        return true;
      }
    }
  }
  
  return false;
}


/*
  Randomly does a screen saver
*/
void ScreenSaver()
{
  int screenSaver = random(3);
  bool response = false;
  
  while (!response)
  {
    switch(screenSaver)
    {
      case 0:
        ;
        response = ScreenSaverRandom();
        break;
      case 1:
        
        response = ScreenSaverBuildUp();
        break;
      case 2:
        
        response = ScreenSaverLines();
        break;
      default:
        
        response = ScreenSaverRandom();
    }
  }
}


/*
  Countdown timer, flashes on and off
*/
void CountDown()
{
  for (int i=1;i<10;i++)
  {
    SetLEDs(255);
    delay(800/i);
    SetLEDs(0);
    delay(800/i);
  }
  
  delay(500);
}



/*
  Play the game
*/
void PlayGame()
{
  unsigned long endTime = millis() + 31000;
  score = 0;
  
  // Debug("endTime: ", endTime);
  // Debug("millis(): ", millis());

  long randButton = random(6);  
  while (millis() < endTime)
  {
    // Set the button to press as lit
    SetLED(HIGH, tortButtonsLed[randButton]);
    
    
    
    // Check to see if the button was pressed
    if(digitalRead(tortButtons[randButton]))
    {
      score ++;
      // Turn off the button
      SetLED(LOW, tortButtonsLed[randButton]);
      
      // Get the new button and check it has changed
      int newButton = random(6);
      while(newButton == randButton)
      {
        newButton = random(6);
      }
      
      // Values differ, so continue
      randButton = newButton;
    }
  }
  
  // Clear all the LEDs
  SetLEDs(0);
  Serial.print("Ваш результат ");
  Serial.println(score);
  Wire.write(score);  /* отправляем по запросу строку "Hello NodeMCU" */
  
  // Wait 10 seconds to show the score
 
}


void receiveEvent(int howMany) {
 while (0 <Wire.available()) {
    char c = Wire.read();      /* получаем байт как символ*/
    Serial.print(c);           /* выводим символ в серийный порт */
  }
 Serial.println();             /* переходим на новую строку */
}

// Функция для извлечения любых отправляемых данных от мастера на шину
void requestEvent() {
 Wire.write(score);  /* отправляем по запросу строку "Hello NodeMCU" */
}



void setup() {
  // put your setup code here, to run once:
    
  Serial.begin(9600); // Initialize serial communications with the PC
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)

  Wire.begin(8);                /* задаем на шине i2c 8 адрес */
  Wire.onReceive(receiveEvent); /* регистрируем полученное событие */
  Wire.onRequest(requestEvent); /* регистрируем запрошенное событие */

    for (int thisPin = 0; thisPin < NUMBER_BUTTONS; thisPin++) { 
      pinMode(tortButtonsLed[thisPin], OUTPUT);
      pinMode(tortButtons[thisPin], INPUT);
    }
    
    Serial.println("Hello ");
    randomSeed(analogRead(A3));
}

void loop() {
  // put your main code here, to run repeatedly:
  ScreenSaver();

  // Countdown to start
  CountDown();

  // Play the game
  PlayGame();
    

}