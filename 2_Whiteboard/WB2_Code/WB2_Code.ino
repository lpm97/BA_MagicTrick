/*****************************************************************************
* | File        :   WB2_Code.ino
* | Author      :   MM & LM
* | Function    :   Setup and Main-Loop      
*----------------
* | This version:   V1.0
* | Date        :   2021_06_03
/*****************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include "Config.h"
#include "EPD_Communication.h"
#include "EPD_Paint.h"
#include <esp_now.h>
#include <WiFi.h>

/*Global Status Data  --------------------------------------------------------*/
uint8_t id = THIS_ID;
uint8_t state = Setup;
uint8_t commandForReader = NOTHING;
uint8_t commandFromReader = NOTHING;
uint8_t errorMessage = NOTHING;
uint8_t draw_card_value = 0;
uint8_t draw_card_suite = 0;

/*Setup  --------------------------------------------------------------------*/
void setup() 
{
  setup_Esp_Now();
  moduleInit();
  change_status (Setup, NOTHING);
  delay(100);
  EPD_Init();
  showBatCap();
  delay(1000);
  clearDisplay();
  systemCheckISR(10000);   //Systemcheck Interrupt (time in ms)
  change_status (Ready, NOTHING);
}

/*Loop  --------------------------------------------------------------------*/
void loop() 
{
  check_GPI();
  if (commandFromReader == SHOW_CARD)
  {
    Debug("ShowCard1");
    change_status (Working, NOTHING);
    drawCard(draw_card_suite,draw_card_value);
    commandFromReader = NOTHING;
    draw_card_suite = NOTHING;
    draw_card_value = NOTHING;
    change_status (Ready, NOTHING);
  }
  if (commandFromReader == CLEAR_DISPLAY)
  {
    change_status (Working, NOTHING);
    clearDisplay();
    commandFromReader = NOTHING;
    change_status (Ready, NOTHING);
  }
  delay(100);
}
