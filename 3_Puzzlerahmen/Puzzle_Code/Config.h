/*****************************************************************************
* | File        :   Config.h
* | Author      :   MM & LM
* | Function    :   GPIO declaration and Module Initialisation     
*----------------
* | This version:   V1.0
* | Date        :   2021_06_03
/*****************************************************************************/


/* Includes ------------------------------------------------------------------*/
#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <Wire.h>
#include <Arduino.h>
#include <stdint.h>
#include <stdio.h>


/* Debug Serial Output -------------------------------------------------------*/
#define USE_DEBUG 1
#if USE_DEBUG
  #define Debug(__info) Serial.println(__info)
#else
  #define Debug(__info)  
#endif


/* GPIO config ----------------------------------------------------------------*/
#define TURN_OFF    36
#define BAT_MONITOR 39
#define OPEN_PB     34
#define TP2         35
#define PWR_KILL    32

#define DS          33
#define SHCP        25
#define STCP        26
#define OE          27

#define SLEEP_SOLE  13
#define IN1_SOLE    2
#define IN2_SOLE    15

#define MODE1       14
#define MODE0       12

#define SLEEP_M3    4
#define DIR_M3      16
#define STEP_M3     17

#define SLEEP_M2    5
#define DIR_M2      18
#define STEP_M2     19

#define SLEEP_M1    21
#define DIR_M1      22
#define STEP_M1     23


/* ID Codes -------------------------------------------------------------*/
#define READER 0
#define WB 1
#define PUZZLE 2
#define REMOTE 3
#define THIS_ID PUZZLE

/* State Codes ----------------------------------------------------------*/
#define Setup 1
#define Ready 2
#define Working 3
#define Error 4
#define TurnOff 5

/* Message Codes --------------------------------------------------------*/
#define START_SCAN 1
#define STOP_SCAN 2
#define CLEAR_DISPLAY 1
#define SHOW_CARD 2 
#define PICK_PUZZLE 2 
#define NOTHING 0
#define POWER_NOT_GOOD 1
#define DISPLAY_NOT_GOOD 2
#define WIFI_NOT_GOOD 3


/* GPIO read and write --------------------------------------------------*/
#define Digital_Write(_pin, _value)   digitalWrite(_pin, _value == 0? LOW:HIGH)
#define Digital_Read(_pin)            digitalRead(_pin)


/* delay in ms ------------------------------------------------------------------*/
#define Delay_ms(__xms) delay(__xms)


/* define Functions------------------------------------------------------*/
uint8_t moduleInit(void);
void GPIO_Config(void);
void Delay_us(uint16_t xus);

#endif
