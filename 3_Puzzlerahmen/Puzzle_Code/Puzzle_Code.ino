/*****************************************************************************
* | File        :   Puzzle_Code.ino
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
uint8_t id = THIS_ID;
uint8_t state = Setup;
uint8_t commandForReader = NOTHING;
uint8_t commandFromReader = NOTHING;
uint8_t errorMessage = NOTHING;
uint8_t puzzle_row = 0;
uint8_t puzzle_columnnn = 0;

/*Setup  --------------------------------------------------------------------*/
void setup() 
{
  setup_Esp_Now();
  moduleInit();
  change_status (Setup, NOTHING);
  Serial.println("Ready in 5s");
  delay(5000);
  systemCheckISR(10000);   //Systemcheck Interrupt (time in ms)
  change_status (Ready, NOTHING);
}

/*Loop  --------------------------------------------------------------------*/
void loop() 
{
  check_GPI();
  if (commandFromReader == PICK_PUZZLE)
  {
    Debug("Pick Puzzle");
    change_status (Working, NOTHING);
    removePuzzle(puzzle_row, puzzle_columnnn);
    commandFromReader = NOTHING;
    puzzle_row = NOTHING;
    puzzle_columnnn = NOTHING;
    change_status (Ready, NOTHING);
  }
  delay(100);
}
