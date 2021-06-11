/*****************************************************************************
* | File        :   Reader_Code.ino
* | Author      :   MM & LM
* | Function    :   Setup and Main-Loop        
*----------------
* | This version:   V1.0
* | Date        :   2021_06_03
/*****************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include "Config.h"
#include <esp_now.h>
#include <WiFi.h>

/*Global Status Data  --------------------------------------------------------*/
uint8_t id = REMOTE;
uint8_t state = Setup;
uint8_t commandForReader = NOTHING;
uint8_t errorMessage = NOTHING;
uint8_t remote_state = TurnOff;
uint8_t wb_state = TurnOff;
uint8_t puzzle_state = TurnOff;
uint8_t visibleStatus = HIGH;

/*Setup  --------------------------------------------------------------------*/
void setup() 
{
  setup_Esp_Now();
  change_status (Setup, NOTHING);
  moduleInit();
  showStatus();
  delay(100);
  if (setupReader()==0)
  { 
    change_status (Error, READER_NOT_GOOD); 
  }
  systemCheckISR(10000);   //Systemcheck Interrupt (time in ms)
  change_status (Ready, NOTHING);
}

/*Loop  --------------------------------------------------------------------*/
void loop() 
{
  if (commandForReader == START_SCAN)
  {
    stopSystemCheckISR ();
    sendMessageToWB (CLEAR_DISPLAY, NOTHING, NOTHING);
    change_status (Working, NOTHING);
    scanTags();
    commandForReader = NOTHING;
    change_status (Ready, NOTHING);
    systemCheckISR(10000);
  }
  if (commandForReader == STOP_SCAN)
  {
    commandForReader = NOTHING;
  }
  check_GPI();
  delay(100);
}
