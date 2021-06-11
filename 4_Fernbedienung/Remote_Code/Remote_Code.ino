/*****************************************************************************
* | File        :   Remote_Code.ino
* | Author      :   MM & LM
* | Function    :   Setup and Main-Loop           
*----------------
* | This version:   V1.0
* | Date        :   2021_06_03
/*****************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include "Config.h"
#include "Work.h"
#include <esp_now.h>
#include <WiFi.h>


/*Global Status Data  --------------------------------------------------------*/
uint8_t id = REMOTE;
uint8_t state = Setup;
uint8_t commandForReader = NOTHING;
uint8_t errorMessage = NOTHING;
uint8_t reader_state = TurnOff;
uint8_t wb_state = TurnOff;
uint8_t puzzle_state = TurnOff;
uint8_t visibleStatus = HIGH;


/*Setup  --------------------------------------------------------------------*/
void setup() 
{
  setup_Esp_Now();
  moduleInit();
  vib_Pattern(1);
  showStatus();
  systemCheckISR(10000);   //Systemcheck Interrupt (time in ms)
  change_status (Ready, NOTHING);
}


/*Loop  --------------------------------------------------------------------*/
void loop() 
{
  //checkBatCapacity();
  //showStatus(); 
  check_GPI();
  delay(100);
}
