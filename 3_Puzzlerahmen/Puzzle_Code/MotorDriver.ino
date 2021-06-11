/*****************************************************************************
* | File        :   MotorDriver.ino
* | Author      :   MM & LM
* | Function    :   Motordriver and Puzzlelocator
*----------------
* | This version:   V1.0
* | Date        :   2021_06_03
/*****************************************************************************/

#include<stdio.h>
#include <string.h>
#include "PuzzleCoordinates.h"

#define MIN_STEP_TIME   150 // us per Steps

int32_t steps_made_M1 = 0;
int32_t steps_made_M2 = 0;
int32_t coordinatesM1;
int32_t coordinatesM2;


/******************************************************************************
function:  Temove Puzzle (whole sequece)
******************************************************************************/
void removePuzzle (uint8_t row, uint8_t column)
{
  digitalWrite(SLEEP_M1, HIGH);
  digitalWrite(SLEEP_M2, HIGH);
  Serial.println("...Open Puzzle");
  openPuzzle ();
  delay(500);
  Serial.println("...Find Home");
  findHome();
  Serial.println("...Go to Zero");
  goToZero ();
  Serial.println("...Go to Puzzle");
  goToPuzzle (row,column);
  delay(500);
  Serial.println("...Pick Puzzle");
  pick ();
  delay(500);
  Serial.println("...Go Home");
  goHome();
  delay(500);
  Serial.println("...Close Puzzle");
  closePuzzle ();
  digitalWrite(SLEEP_M1, LOW);
  digitalWrite(SLEEP_M2, LOW);
}


/******************************************************************************
function:  Go to Puzzle Location
******************************************************************************/
void goToPuzzle(uint8_t row, uint8_t column)
{
  int32_t m1StepsToDo, m2StepsToDo;
  getCoordinates (row, column);
  Serial.println("Coordinates");
  Serial.println(coordinatesM1);
  Serial.println(coordinatesM2);
  m1StepsToDo = coordinatesM1 - steps_made_M1;
  m2StepsToDo = coordinatesM2 - steps_made_M2;
  steps_made_M1 += m1StepsToDo;
  steps_made_M2 += m2StepsToDo;
  moveM1(m1StepsToDo);
  delay(500);
  moveM2(m2StepsToDo);
  //moveM1M2 ( m1StepsToDo,  m2StepsToDo);
  
}


/******************************************************************************
function:  Release Picked Puzzle
******************************************************************************/
void releasePuzzle()
{
  
}


/******************************************************************************
function:  Go Back to Home
******************************************************************************/
void goHome ()
{
  int32_t m1StepsToDo, m2StepsToDo;
  m1StepsToDo = -steps_made_M1;
  m2StepsToDo = -steps_made_M2;
  moveM1M2 (m1StepsToDo, m2StepsToDo);
  return;
}


/******************************************************************************
function:  Find Start Position
******************************************************************************/
void findHome ()
{
  Serial.println("Go really Home");
  moveM1M2 (0, 1000);
  moveM1M2 (-1000, 0);
  moveM1M2 (0, -2000);
  return;
}


/******************************************************************************
function:  Go to Storage Position to Close Puzzle
******************************************************************************/
void findStorage ()
{
  Serial.println("Go really Home");
  
  moveM1M2 (0, -1000);
  return;
}


/******************************************************************************
function:  Pick Puzzle with Solenoid
******************************************************************************/
void pick ()
{
  digitalWrite(IN2_SOLE, HIGH);
  delay(500);
  digitalWrite(IN2_SOLE, LOW);
}


/******************************************************************************
function:  Go Down
******************************************************************************/
void goToZero ()
{
  #define M1HomeTOZeroSteps 26500
  #define M2HomeTOZeroSteps -1800
  steps_made_M1 += M1HomeTOZeroSteps;
  steps_made_M2 += M2HomeTOZeroSteps;
  moveM1M2 (M1HomeTOZeroSteps, M2HomeTOZeroSteps);
}


/******************************************************************************
function:  Move Motor1 and 2 together  
******************************************************************************/
void moveM1M2 (int32_t m1StepsToDo, int32_t m2StepsToDo)
{
//  Serial.println("MOVE M1M2");
  
  uint32_t timeDuration;
  uint32_t m1HalveStepTime, m2HalveStepTime;
  unsigned long microsOldM1, microsOldM2;
  uint32_t m1Steps = 0;
  uint32_t m2Steps = 0;
  uint8_t m1Sig = 0;
  uint8_t m2Sig = 0;
  
  if (m1StepsToDo<0) 
  {   
    m1StepsToDo = abs(m1StepsToDo);     
    digitalWrite(DIR_M1, LOW);  
  }
  else    {  digitalWrite(DIR_M1, HIGH);  }

  if (m2StepsToDo<0) 
  {   
    m2StepsToDo = abs(m2StepsToDo);     
    digitalWrite(DIR_M2, LOW);  
  }
  else    {  digitalWrite(DIR_M2, HIGH);  }

  m1StepsToDo++;
  m2StepsToDo++;
  
  if (m1StepsToDo>m2StepsToDo)
  {
    timeDuration = m1StepsToDo * MIN_STEP_TIME;
    m1HalveStepTime = MIN_STEP_TIME / 2;
    m2HalveStepTime = (timeDuration / m2StepsToDo) / 2;
  }
  else
  {
    timeDuration = m2StepsToDo * MIN_STEP_TIME;
    m2HalveStepTime = MIN_STEP_TIME / 2;
    m1HalveStepTime = (timeDuration / m1StepsToDo) / 2;
  }

  m1StepsToDo--;
  m2StepsToDo--;
  
  microsOldM1 = micros();
  microsOldM2 = micros();
  
  while (m1Steps<m1StepsToDo || m2Steps<m2StepsToDo)
  {
    if (  (micros() - microsOldM1 > m1HalveStepTime) && (m1Steps<m1StepsToDo )  )
    {
      microsOldM1 += m1HalveStepTime;
      m1Sig = 1 - m1Sig;
      m1Steps += m1Sig;
      digitalWrite(STEP_M1, m1Sig);
    }
    
    if (  (micros() - microsOldM2 > m2HalveStepTime) && (m2Steps<m2StepsToDo )  )
    {
      microsOldM2 += m2HalveStepTime;
      m2Sig = 1 - m2Sig;
      m2Steps += m2Sig;
      digitalWrite(STEP_M2, m2Sig);
    }
  }
//  Serial.println(m1Steps);
//  Serial.println(m2Steps);
//  Serial.println("Finish MOVE M1M2");
  return;
}


/******************************************************************************
function:  Move Motor1 alone
******************************************************************************/
void moveM1 (int32_t m1StepsToDo)
{
  int32_t m1Steps = 0;
  if (m1StepsToDo<0) 
  {   
    m1StepsToDo = abs(m1StepsToDo);     
    digitalWrite(DIR_M1, LOW);  
  }
  else    {  digitalWrite(DIR_M1, HIGH);  }

  while (m1Steps<m1StepsToDo)
  {
    digitalWrite(STEP_M1, HIGH);
    delayMicroseconds(MIN_STEP_TIME / 2);
    digitalWrite(STEP_M1, LOW);
    delayMicroseconds(MIN_STEP_TIME / 2);
    m1Steps++;
  }
  return;
}


/******************************************************************************
function:  Move Motor2 alone
******************************************************************************/
void moveM2 (int32_t m2StepsToDo)
{
  int32_t m2Steps = 0;
  if (m2StepsToDo<0) 
  {   
    m2StepsToDo = abs(m2StepsToDo);     
    digitalWrite(DIR_M2, LOW);  
  }
  else    {  digitalWrite(DIR_M2, HIGH);  }

  while (m2Steps<m2StepsToDo)
  {
    digitalWrite(STEP_M2, HIGH);
    delayMicroseconds(MIN_STEP_TIME / 2);
    digitalWrite(STEP_M2, LOW);
    delayMicroseconds(MIN_STEP_TIME / 2);
    m2Steps++;
  }
  return;
}


/******************************************************************************
function:  Open Puzzle
******************************************************************************/
void openPuzzle ()
{
  #define STEPS_TO_OPEN 16000
  digitalWrite(DIR_M3, HIGH);
  digitalWrite(SLEEP_M3, HIGH);
  delay(100);
  for (int x = 0; x<STEPS_TO_OPEN; x++)
  {
    digitalWrite(STEP_M3, HIGH);
    delayMicroseconds(MIN_STEP_TIME/2);
    digitalWrite(STEP_M3, LOW);
    delayMicroseconds(MIN_STEP_TIME/2);
  }
  digitalWrite(SLEEP_M3, LOW);
  return;
}


/******************************************************************************
function:  Close Puzzle
******************************************************************************/
void closePuzzle ()
{
  #define STEPS_TO_CLOSE 18000
  digitalWrite(DIR_M3, LOW);
  digitalWrite(SLEEP_M3, HIGH);
  for (int x = 0; x<STEPS_TO_CLOSE; x++)
  {
    digitalWrite(STEP_M3, HIGH);
    delayMicroseconds(MIN_STEP_TIME/2);
    digitalWrite(STEP_M3, LOW);
    delayMicroseconds(MIN_STEP_TIME/2);
    if (x == 10000)
    {
      findStorage ();
    }
  }
  delay(500);
  digitalWrite(SLEEP_M3, LOW);
  return;
}


/******************************************************************************
function:  Get Coordinates
******************************************************************************/
void getCoordinates (uint8_t row, uint8_t column)
{
  #define L 0
  #define R 0

  static int32_t stepsToMake[15][10][2];

  //***Row 0
  stepsToMake[0][0][0] = 43000;
  stepsToMake[0][0][1] = -32000;
  
  stepsToMake[0][1][0] = 37500;
  stepsToMake[0][1][1] = -30000;

  stepsToMake[0][2][0] = 32800;
  stepsToMake[0][2][1] = -28000;

  stepsToMake[0][3][0] = 28800;
  stepsToMake[0][3][1] = -24500;

  stepsToMake[0][4][0] = R;
  stepsToMake[0][4][1] = L;

  stepsToMake[0][5][0] = R;
  stepsToMake[0][5][1] = L;

  stepsToMake[0][6][0] = R;
  stepsToMake[0][6][1] = L;

  stepsToMake[0][7][0] = R;
  stepsToMake[0][7][1] = L;

  stepsToMake[0][8][0] = R;
  stepsToMake[0][8][1] = L;

  stepsToMake[0][9][0] = R;
  stepsToMake[0][9][1] = L;



  //***Row 1
  stepsToMake[1][0][0] = 46000;
  stepsToMake[1][0][1] = -27000;
  
  stepsToMake[1][1][0] = 40000;
  stepsToMake[1][1][1] = -25500;

  stepsToMake[1][2][0] = 37000;
  stepsToMake[1][2][1] = -23500;

  stepsToMake[1][3][0] = 31500;
  stepsToMake[1][3][1] = -21000;

  stepsToMake[1][4][0] = R;
  stepsToMake[1][4][1] = L;

  stepsToMake[1][5][0] = R;
  stepsToMake[1][5][1] = L;

  stepsToMake[1][6][0] = 20500;
  stepsToMake[1][6][1] = -10500;

  stepsToMake[1][7][0] = 18500;
  stepsToMake[1][7][1] = -6500;

  stepsToMake[1][8][0] = 16000;
  stepsToMake[1][8][1] = -2500;

  stepsToMake[1][9][0] = 15500;
  stepsToMake[1][9][1] = 3000;

  
  
  //***Row 2
  stepsToMake[2][0][0] = 47000;
  stepsToMake[2][0][1] = -22500;
  
  stepsToMake[2][1][0] = 42000;
  stepsToMake[2][1][1] = -22000;

  stepsToMake[2][2][0] = 38000;
  stepsToMake[2][2][1] = -20000;

  stepsToMake[2][3][0] = 34000;
  stepsToMake[2][3][1] = -18000;

  stepsToMake[2][4][0] = R;
  stepsToMake[2][4][1] = L;

  stepsToMake[2][5][0] = R;
  stepsToMake[2][5][1] = L;

  stepsToMake[2][6][0] = 24500;
  stepsToMake[2][6][1] = -8500;

  stepsToMake[2][7][0] = 22000;
  stepsToMake[2][7][1] = -5000;

  stepsToMake[2][8][0] = 20500;
  stepsToMake[2][8][1] = -500;

  stepsToMake[2][9][0] = 20000;
  stepsToMake[2][9][1] = 4000;



  //***Row 3
  stepsToMake[3][0][0] = 49500;
  stepsToMake[3][0][1] = -18000;
  
  stepsToMake[3][1][0] = 44500;
  stepsToMake[3][1][1] = -17000;

  stepsToMake[3][2][0] = 40500;
  stepsToMake[3][2][1] = -16000;

  stepsToMake[3][3][0] = 37500;
  stepsToMake[3][3][1] = -14000;

  stepsToMake[3][4][0] = R;
  stepsToMake[3][4][1] = L;

  stepsToMake[3][5][0] = R;
  stepsToMake[3][5][1] = L;

  stepsToMake[3][6][0] = 28500;
  stepsToMake[3][6][1] = -5000;

  stepsToMake[3][7][0] = 26500;
  stepsToMake[3][7][1] = -2000;

  stepsToMake[3][8][0] = 25000;
  stepsToMake[3][8][1] = 3000;

  stepsToMake[3][9][0] = 24500;
  stepsToMake[3][9][1] = 7000;



  //***Row 4
  stepsToMake[4][0][0] = 51500;
  stepsToMake[4][0][1] = -13000;
  
  stepsToMake[4][1][0] = 45500;
  stepsToMake[4][1][1] = -11200;

  stepsToMake[4][2][0] = 44300;
  stepsToMake[4][2][1] = -10500;

  stepsToMake[4][3][0] = 40300;
  stepsToMake[4][3][1] = -9500;

  stepsToMake[4][4][0] = R;
  stepsToMake[4][4][1] = L;

  stepsToMake[4][5][0] = R;
  stepsToMake[4][5][1] = L;

  stepsToMake[4][6][0] = 32800;
  stepsToMake[4][6][1] = -1000;

  stepsToMake[4][7][0] = 31300;
  stepsToMake[4][7][1] = 2500;

  stepsToMake[4][8][0] = 29300;
  stepsToMake[4][8][1] = 5500;

  stepsToMake[4][9][0] = 29300;
  stepsToMake[4][9][1] = 9500;



  //***Row 5
  stepsToMake[5][0][0] = 54500;
  stepsToMake[5][0][1] = -8500;
  
  stepsToMake[5][1][0] = 50500;
  stepsToMake[5][1][1] = -8500;

  stepsToMake[5][2][0] = 47000;
  stepsToMake[5][2][1] = -7500;

  stepsToMake[5][3][0] = 44000;
  stepsToMake[5][3][1] = -5500;

  stepsToMake[5][4][0] = 41000;
  stepsToMake[5][4][1] = -3500;

  stepsToMake[5][5][0] = 38500;
  stepsToMake[5][5][1] = -1500;

  stepsToMake[5][6][0] = 36500;
  stepsToMake[5][6][1] = 1500;

  stepsToMake[5][7][0] = 35100;
  stepsToMake[5][7][1] = 5000;

  stepsToMake[5][8][0] = 34100;
  stepsToMake[5][8][1] = 8500;

  stepsToMake[5][9][0] = 33600;
  stepsToMake[5][9][1] = 12500;



  //***Row 6
  stepsToMake[6][0][0] = 57500;
  stepsToMake[6][0][1] = -4000;
  
  stepsToMake[6][1][0] = 54500;
  stepsToMake[6][1][1] = -3000;

  stepsToMake[6][2][0] = 50500;
  stepsToMake[6][2][1] = -3000;

  stepsToMake[6][3][0] = 48000;
  stepsToMake[6][3][1] = -1000;

  stepsToMake[6][4][0] = 45500;
  stepsToMake[6][4][1] = 1000;

  stepsToMake[6][5][0] = 43500;
  stepsToMake[6][5][1] = 3500;

  stepsToMake[6][6][0] = 41500;
  stepsToMake[6][6][1] = 5500;

  stepsToMake[6][7][0] = 40000;
  stepsToMake[6][7][1] = 8500;

  stepsToMake[6][8][0] = 39000;
  stepsToMake[6][8][1] = 11000;

  stepsToMake[6][9][0] = 38000;
  stepsToMake[6][9][1] = 14000;



  //***Row 7
  stepsToMake[7][0][0] = R;
  stepsToMake[7][0][1] = L;
  
  stepsToMake[7][1][0] = 58000;
  stepsToMake[7][1][1] = 1000;

  stepsToMake[7][2][0] = 54500;
  stepsToMake[7][2][1] = 1500;

  stepsToMake[7][3][0] = 52000;
  stepsToMake[7][3][1] = 3000;

  stepsToMake[7][4][0] = 50000;
  stepsToMake[7][4][1] = 5000;

  stepsToMake[7][5][0] = 47500;
  stepsToMake[7][5][1] = 7000;

  stepsToMake[7][6][0] = 46000;
  stepsToMake[7][6][1] = 9500;

  stepsToMake[7][7][0] = 45000;
  stepsToMake[7][7][1] = 12000;

  stepsToMake[7][8][0] = 43500;
  stepsToMake[7][8][1] = 14500;

  stepsToMake[7][9][0] = R;
  stepsToMake[7][9][1] = L;



  //***Row 8
  stepsToMake[8][0][0] = R;
  stepsToMake[8][0][1] = L;
  
  stepsToMake[8][1][0] = R;
  stepsToMake[8][1][1] = L;

  stepsToMake[8][2][0] = R;
  stepsToMake[8][2][1] = L;

  stepsToMake[8][3][0] = R;
  stepsToMake[8][3][1] = L;

  stepsToMake[8][4][0] = R;
  stepsToMake[8][4][1] = L;

  stepsToMake[8][5][0] = R;
  stepsToMake[8][5][1] = L;

  stepsToMake[8][6][0] = R;
  stepsToMake[8][6][1] = L;

  stepsToMake[8][7][0] = R;
  stepsToMake[8][7][1] = L;

  stepsToMake[8][8][0] = R;
  stepsToMake[8][8][1] = L;

  stepsToMake[8][9][0] = R;
  stepsToMake[8][9][1] = L;



  //***Row 9
  stepsToMake[0][0][0] = R;
  stepsToMake[0][0][1] = L;
  
  stepsToMake[0][1][0] = R;
  stepsToMake[0][1][1] = L;

  stepsToMake[0][2][0] = R;
  stepsToMake[0][2][1] = L;

  stepsToMake[0][3][0] = R;
  stepsToMake[0][3][1] = L;

  stepsToMake[0][4][0] = R;
  stepsToMake[0][4][1] = L;

  stepsToMake[0][5][0] = R;
  stepsToMake[0][5][1] = L;

  stepsToMake[0][6][0] = R;
  stepsToMake[0][6][1] = L;

  stepsToMake[0][7][0] = R;
  stepsToMake[0][7][1] = L;

  stepsToMake[0][8][0] = R;
  stepsToMake[0][8][1] = L;

  stepsToMake[0][9][0] = R;
  stepsToMake[0][9][1] = L;



  //***Row 10
  stepsToMake[0][0][0] = R;
  stepsToMake[0][0][1] = L;
  
  stepsToMake[0][1][0] = R;
  stepsToMake[0][1][1] = L;

  stepsToMake[0][2][0] = R;
  stepsToMake[0][2][1] = L;

  stepsToMake[0][3][0] = R;
  stepsToMake[0][3][1] = L;

  stepsToMake[0][4][0] = R;
  stepsToMake[0][4][1] = L;

  stepsToMake[0][5][0] = R;
  stepsToMake[0][5][1] = L;

  stepsToMake[0][6][0] = R;
  stepsToMake[0][6][1] = L;

  stepsToMake[0][7][0] = R;
  stepsToMake[0][7][1] = L;

  stepsToMake[0][8][0] = R;
  stepsToMake[0][8][1] = L;

  stepsToMake[0][9][0] = R;
  stepsToMake[0][9][1] = L;



  //***Row 11
  stepsToMake[0][0][0] = R;
  stepsToMake[0][0][1] = L;
  
  stepsToMake[0][1][0] = R;
  stepsToMake[0][1][1] = L;

  stepsToMake[0][2][0] = R;
  stepsToMake[0][2][1] = L;

  stepsToMake[0][3][0] = R;
  stepsToMake[0][3][1] = L;

  stepsToMake[0][4][0] = R;
  stepsToMake[0][4][1] = L;

  stepsToMake[0][5][0] = R;
  stepsToMake[0][5][1] = L;

  stepsToMake[0][6][0] = R;
  stepsToMake[0][6][1] = L;

  stepsToMake[0][7][0] = R;
  stepsToMake[0][7][1] = L;

  stepsToMake[0][8][0] = R;
  stepsToMake[0][8][1] = L;

  stepsToMake[0][9][0] = R;
  stepsToMake[0][9][1] = L;



  //***Row 12
  stepsToMake[0][0][0] = R;
  stepsToMake[0][0][1] = L;
  
  stepsToMake[0][1][0] = R;
  stepsToMake[0][1][1] = L;

  stepsToMake[0][2][0] = R;
  stepsToMake[0][2][1] = L;

  stepsToMake[0][3][0] = R;
  stepsToMake[0][3][1] = L;

  stepsToMake[0][4][0] = R;
  stepsToMake[0][4][1] = L;

  stepsToMake[0][5][0] = R;
  stepsToMake[0][5][1] = L;

  stepsToMake[0][6][0] = R;
  stepsToMake[0][6][1] = L;

  stepsToMake[0][7][0] = R;
  stepsToMake[0][7][1] = L;

  stepsToMake[0][8][0] = R;
  stepsToMake[0][8][1] = L;

  stepsToMake[0][9][0] = R;
  stepsToMake[0][9][1] = L;



  //***Row 13
  stepsToMake[0][0][0] = R;
  stepsToMake[0][0][1] = L;
  
  stepsToMake[0][1][0] = R;
  stepsToMake[0][1][1] = L;

  stepsToMake[0][2][0] = R;
  stepsToMake[0][2][1] = L;

  stepsToMake[0][3][0] = R;
  stepsToMake[0][3][1] = L;

  stepsToMake[0][4][0] = R;
  stepsToMake[0][4][1] = L;

  stepsToMake[0][5][0] = R;
  stepsToMake[0][5][1] = L;

  stepsToMake[0][6][0] = R;
  stepsToMake[0][6][1] = L;

  stepsToMake[0][7][0] = R;
  stepsToMake[0][7][1] = L;

  stepsToMake[0][8][0] = R;
  stepsToMake[0][8][1] = L;

  stepsToMake[0][9][0] = R;
  stepsToMake[0][9][1] = L;



  //***Row 14
  stepsToMake[0][0][0] = R;
  stepsToMake[0][0][1] = L;
  
  stepsToMake[0][1][0] = R;
  stepsToMake[0][1][1] = L;

  stepsToMake[0][2][0] = R;
  stepsToMake[0][2][1] = L;

  stepsToMake[0][3][0] = R;
  stepsToMake[0][3][1] = L;

  stepsToMake[0][4][0] = R;
  stepsToMake[0][4][1] = L;

  stepsToMake[0][5][0] = R;
  stepsToMake[0][5][1] = L;

  stepsToMake[0][6][0] = R;
  stepsToMake[0][6][1] = L;

  stepsToMake[0][7][0] = R;
  stepsToMake[0][7][1] = L;

  stepsToMake[0][8][0] = R;
  stepsToMake[0][8][1] = L;

  stepsToMake[0][9][0] = R;
  stepsToMake[0][9][1] = L;

  coordinatesM1 = stepsToMake[row-1][column-1][0];
  coordinatesM2 = stepsToMake[row-1][column-1][1];
}
