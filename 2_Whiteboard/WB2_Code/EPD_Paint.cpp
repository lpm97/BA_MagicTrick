/*****************************************************************************
* | File        :   EPD_Paint.cpp
* | Author      :   LM & Waveshare team
* | Function    :   Hardware underlying interface        
*----------------
* | This version:   V1.0
* | Date        :   2021_06_03
/*****************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "EPD_Paint.h"


static uint8_t displayClear = LOW;


/******************************************************************************
function :  Clear screen
******************************************************************************/
void clearDisplay(void)
{
    uint16_t y, x;
    
    if (displayClear == HIGH)
    {
      return;  
    }
    Debug("Clear Display start...");
    //M1 part 648*492
    SendCommand_M1(0x10);
    for(y =  492; y < 984; y++)
    {
        for(x = 0; x < 81; x++) 
        {
          SendData_M1(0xff);
        }
    }   
    SendCommand_M1(0x13);
    for(y = 492; y < 984; y++)
    {
        for(x = 0; x < 81; x++) 
        {
            SendData_M1(0xff);
        }
     }
    
    //S1 part 656*492
    SendCommand_S1(0x10);
    for(y = 492; y < 984; y++)
    {
        for(x = 81; x < 163; x++) 
        {
            SendData_S1(0xff);
        }
    }
    SendCommand_S1(0x13);
    for(y = 492; y < 984; y++)
    {
        for(x = 81; x < 163; x++) 
        {
            SendData_S1(0xff);
        }
    }

    //M2 part 656*492
    SendCommand_M2(0x10);
    for(y = 0; y < 492; y++)
    {
        for(x = 81; x < 163; x++) 
        {
            SendData_M2(0xff);
        }
    }
    SendCommand_M2(0x13);
    for(y = 0; y < 492; y++)
    {
        for(x = 81; x < 163; x++) 
        {
            SendData_M2(0xff);
        }
    }

    //S2 part 648*492
    SendCommand_S2(0x10);
    for(y = 0; y < 492; y++)
    {
        for(x = 0; x < 81; x++) 
        {
            SendData_S2(0xff);
        }
    }
    SendCommand_S2(0x13);
    for(y = 0; y < 492; y++)
    {
        for(x = 0; x < 81; x++) 
        {
            SendData_S2(0xff);
        }
    }
    Debug("Display Cleared");
    displayClear = HIGH;
    TurnOnDisplay();
}



/******************************************************************************
function : Draw Card
******************************************************************************/
void drawCard (uint8_t cardSuit, uint8_t cardValue)
{
  if(cardSuit>4 || cardSuit<1)
  {
    Debug("Invalid Card...");
    return;
  }
  if(cardValue>13 || cardValue<1)  //@@
  {
    Debug("Invalid Card...");
    //change_status (Error, WIFI_NOT_GOOD); //@@
    return;
  }
  
  Debug("Draw on Display start...");
  drawTop(cardSuit);
  drawBot(cardValue);
  Debug("Display drawn");
  displayClear = LOW;
  TurnOnDisplay();
}



/******************************************************************************
function :  Draw Suite on top Halfe of Screen
******************************************************************************/
void drawTop (uint8_t suit)
{
  suit--;
  SendCommand_M2(0x13);
  for(int y = 0; y < 192; y++)
  {
    for(int x = 0; x < (EDP_M2_WIDTH/8); x++) 
    {
      SendData_M2(0xFF);
    }
  }
  for(int y = 192; y < 492; y++)
  {
    for(int x = 0; x < (EDP_M2_WIDTH/8); x++) 
    {
      if (x<75)
      {
        SendData_M2(  card_suits[suit][ x + (y-192) * (600/8) ] );
      }
      else
      {
        SendData_M2(0xFF);
      }
    }
  }

  SendCommand_S1(0x13);
  for(int y = 492; y < 792; y++)
  {
      for(int x = 0; x < (EDP_S1_WIDTH/8); x++) 
      {
          if (x<75)
          {
            SendData_S1(  card_suits[suit][ x + (y-192) * (600/8) ] );
          }
          else
          {
            SendData_S1(0xFF);
          }
      }
  }
  for(int y = 792; y < 984; y++)
  {
    for(int x = 0; x < (EDP_S1_WIDTH/8); x++) 
    {
      SendData_S1(0xFF);
    }
  }
}



/******************************************************************************
function :  Draw Value on bot Halfe of Screen
******************************************************************************/
void drawBot (uint8_t cardValue)
{
  cardValue--;
  SendCommand_S2(0x13);
  for(int y = 0; y < 192; y++)
  {
    for(int x = 0; x < (EDP_S2_WIDTH/8); x++) 
    {
      SendData_S2(0xFF);
    }
  }


  for(int y = 192; y < 492; y++)
  {
    for(int x = 0; x < (EDP_S2_WIDTH/8); x++) 
    {
      if (x<7)
      {
        SendData_S2(0xFF);
      }
      else
      {
        SendData_S2(  card_value[cardValue][ x-7 + (y-192) * (600/8) ] );
      }
    }
  }


  SendCommand_M1(0x13);
  for(int y = 492; y < 792; y++)
  {
      for(int x = 0; x < (EDP_M1_WIDTH/8); x++) 
      {
          if (x<7)
          {
            SendData_M1(0xFF);
          }
          else
          {
            SendData_M1(  card_value[cardValue][ x-7 + (y-192) * (600/8) ] );
          }
      }
  }
  
  for(int y = 792; y < 984; y++)
  {
    for(int x = 0; x < (EDP_M1_WIDTH/8); x++) 
    {
      SendData_M1(0xFF);
    }
  }

  
}



/******************************************************************************
function :  Draw BatBitMap
******************************************************************************/
void drawBatCap (uint8_t batCap)
{
  int x,y;
  Debug("Show Bat Cap");
  if (batCap>5 || batCap<0)
  {
    Debug("Wrong BatCap");
    return;  
  }
  
  SendCommand_M2(0x13);
  for(y = 0; y < 492; y++)
  {
    for(x = 0; x < 82; x++) 
    {
      if (y<161 || y>331 || x<11 || x>70)
      {
        SendData_M2(0xff);
      }
      else 
      {
        SendData_M2 ( bat_pixel[batCap][ (x-11) + (y-161)*(480/8) ] );
      }
    }
  }

  SendCommand_M1(0x13);
  for(y = 492; y < 984; y++)
  {
      for(x = 0; x < 81; x++) 
      {
          SendData_M1(0xff);
      }
   }
  SendCommand_S1(0x13);
  for(y = 492; y < 984; y++)
  {
      for(x = 81; x < 163; x++) 
      {
          SendData_S1(0xff);
      }
  }
  SendCommand_S2(0x13);
  for(y = 0; y < 492; y++)
  {
      for(x = 0; x < 81; x++) 
      {
          SendData_S2(0xff);
      }
  }
  displayClear = LOW;
  TurnOnDisplay();
}
