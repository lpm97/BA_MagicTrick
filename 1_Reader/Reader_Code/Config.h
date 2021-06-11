/*****************************************************************************
* | File        :   Config.h
* | Author      :   MM & LM
* | Function    :   GPIO declaration and Defines       
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
#define TURN_OFF  36
#define BAT_MONITOR 39
#define BUTTON  34
#define PWR_KILL  32
#define DS  33
#define SHCP  25
#define STCP  26
#define OE  27
#define PG  14
#define M6E_EN  4

/* ID Codes -------------------------------------------------------------*/
#define READER 0
#define WB 1
#define PUZZLE 2
#define REMOTE 3
#define THIS_ID READER

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
#define READER_NOT_GOOD 3
#define SCAN_FINISH 0xFF


/* LED Colar Codes ------------------------------------------------------*/
#define SetupLED 0b00000101   // Violet
#define ReadyLED 0b00000010   // Green
#define WorkingLED 0b00000100 // Blue
#define ErrorLED 0b00000001   // Red
#define TurnOffLED 0b00000000   // Off

#define Bat80 0b00000010      // Green
#define Bat60 0b00000011      // Yellow
#define Bat40 0b00000110      // light Blue
#define Bat20 0b00000100      // Blue
#define Bat10 0b00000101      // Violet
#define Bat0  0b00000001      // Red


/* GPIO read and write --------------------------------------------------*/
#define Digital_Write(_pin, _value)   digitalWrite(_pin, _value == 0? LOW:HIGH)
#define Digital_Read(_pin)            digitalRead(_pin)

/* define Functions------------------------------------------------------*/
uint8_t moduleInit(void);
void GPIO_Config(void);

#endif
