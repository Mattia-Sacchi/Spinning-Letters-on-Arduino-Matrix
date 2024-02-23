#include "letters.hpp"
#include <Arduino_LED_Matrix.h>

ArduinoLEDMatrix matrix;

enum Mod
{
  Mod_Normal = 0,
  Mod_Spinning,
};

void setup() {
  // put your setup code here, to run once:
  init(false);
  matrix.begin();
}

byte clearFrame[8][12] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

byte frame[8][12] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

Mod mod = Mod_Spinning;

void normalShow(String str)
{
  for(int i = 0; i <str.length(); i++)
  {
    if(str[i] >=  97 && str[i] <= 122)
    {
      matrix.renderBitmap(letters[str[i]-97],8,12);
      delay(750);
      matrix.renderBitmap(clearFrame,8,12);
    }else
    delay(750);
  }
}

void leftShift()
{
  for(int i = 1; i < 12; i++)
  {
    for(int j = 0; j <8; j++)
    {
      frame[j][i-1] = frame[j][i]; 
    }
  }

}

bool addVerticalLine(uint8_t array[8])
{
  
  bool ret = true;
  for(int i = 0; i <8; i++)
  {
    frame[i][11] = array[i];

    if(array[i] != 0)
      ret = false;
  }
  delay(250);
  return ret;
  
}

void printLetter(char ch)
{
  int index = ch -97;

  uint8_t array[8];
  for(int i = 0; i <12; i++)
  {
    for( int  j= 0 ; j <8; j++){
      
      array[j] = letters[index][j][i];
    }

    bool isFinished = addVerticalLine(array);
    matrix.renderBitmap(frame,8,12);
    leftShift();

    if(isFinished)
    {
      addVerticalLine(array);
      matrix.renderBitmap(frame,8,12);
      leftShift();
      return;
    }
    
    
  }
  
}

void spinningShow(String str)
{
  matrix.renderBitmap(clearFrame,8,12);
  memcpy(frame,clearFrame,sizeof(frame));


  for(int i = 0; i <str.length(); i++)
  {
    if(str[i] >=  97 && str[i] <= 122 )
    {
      
      printLetter(str[i]);
    }
    else if(str[i] == ' ')
    {
      for(int j = 0; j <3; j++){
      uint8_t array[8] = {0,0,0,0,0,0,0,0};
      addVerticalLine(array);
      matrix.renderBitmap(frame,8,12);
      leftShift();
      }
    }
    else
      delay(300);
  }

  for(int i = 0; i < 10; i++)
  {
    leftShift();
    delay(100);
    matrix.renderBitmap(frame,8,12);
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  String str = Serial.readString();
  str.toLowerCase();

  if(str[0] == '0'){
    mod = Mod_Normal;
    Serial.println("Mod Normal");
    return;
  }
  if(str[0] == '1'){
    mod = Mod_Spinning;
    Serial.println("Mod Spinning");
    return;
  }

  switch(mod)
  {
    case Mod_Spinning:
      spinningShow(str);
    break;
    default:
     normalShow(str);
  }

}
