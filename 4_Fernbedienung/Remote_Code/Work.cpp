/*****************************************************************************
* | File      	:   Work.cpp
* | Author      :   MM & LM
* | Function    :   Drive Vibration Motor     
*----------------
* |	This version:   V1.0
* | Date        :   2021_06_03
/*****************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "Work.h"

#define LOW_TIME 100
#define HIGH_TIME 400
#define DUTY_CYCLE 200


/******************************************************************************
function:  Vibrate Number Binary
******************************************************************************/
void vib_Number(uint8_t nr1, uint8_t nr2)
{
  nr1--;
  nr2--;
  for (uint8_t x = 0; x < 2; x++)
  {
    if (nr1 & 0x01 == 1)
    {
      Serial.println("HIGH");
      ledcWrite(0, DUTY_CYCLE);
      delay(HIGH_TIME);
      ledcWrite(0, 0);  
      delay(LOW_TIME);
      delay(100);
    }
    else 
    {
      Serial.println("LOW");
      ledcWrite(0, DUTY_CYCLE);
      delay(LOW_TIME);
      ledcWrite(0, 0);  
      delay(HIGH_TIME);
      delay(100);
    }
    nr1 = nr1>>1;
  }
//  delay(1500);
//  
//  for (uint8_t x = 0; x < 4; x++)
//  {
//    if (nr2 & 0x01 == 1)
//    {
//      ledcWrite(0, DUTY_CYCLE);
//      delay(HIGH_TIME);
//      ledcWrite(0, 0);  
//      delay(LOW_TIME);
//    }
//    else 
//    {
//      ledcWrite(0, DUTY_CYCLE);
//      delay(LOW_TIME);
//      ledcWrite(0, 0);  
//      delay(HIGH_TIME);
//    }
//    nr2 = nr2>>1;
//  }
  return;
}


/******************************************************************************
function:  Vibrate in Pattern
******************************************************************************/
void vib_Pattern(uint8_t pat)
{
  if (pat==1)
  {
    ledcWrite(0, DUTY_CYCLE);
    delay(500);
    ledcWrite(0, 0);  
  }
  
  if (pat==2)
  {
    for (uint8_t x = 0; x < 2; x++)
    {
      ledcWrite(0, DUTY_CYCLE);
      delay(200);
      ledcWrite(0, 0);  
      delay(100);
    }
  } 
  return;
}
