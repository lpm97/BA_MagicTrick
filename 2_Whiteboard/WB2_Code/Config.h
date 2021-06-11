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
#define SCK_PIN  13
#define MOSI_PIN 14

#define M1_CS_PIN 23
#define S1_CS_PIN 22
#define M2_CS_PIN 16
#define S2_CS_PIN 19

#define M1S1_DC_PIN  25
#define M2S2_DC_PIN  17

#define M1S1_RST_PIN 33
#define M2S2_RST_PIN 5

#define M1_BUSY_PIN  32
#define S1_BUSY_PIN  26
#define M2_BUSY_PIN  18
#define S2_BUSY_PIN  4

#define TURN_OFF     36
#define BAT_MONITOR  39
#define MAG_POS1     34
#define MAG_POS2     35
#define PWR_KILL     21
#define PG           27
#define LED          2


/* ID Codes -------------------------------------------------------------*/
#define READER 0
#define WB 1
#define PUZZLE 2
#define REMOTE 3
#define THIS_ID WB


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
#define NOTHING 0
#define POWER_NOT_GOOD 1
#define DISPLAY_NOT_GOOD 2
#define WIFI_NOT_GOOD 3


/* Display resolution ------------------------------------------------------*/
#define EDP_MAX_WIDTH       1304
#define EDP_MAX_HEIGHT      984

#define EDP_M1_WIDTH        648
#define EDP_M1_HEIGHT      (EDP_MAX_HEIGHT / 2)
#define EDP_S1_WIDTH       (EDP_MAX_WIDTH - 648)    //656
#define EDP_S1_HEIGHT      (EDP_MAX_HEIGHT / 2)
#define EDP_M2_WIDTH       (EDP_MAX_WIDTH - 648)    //656
#define EDP_M2_HEIGHT      (EDP_MAX_HEIGHT / 2)
#define EDP_S2_WIDTH        648
#define EDP_S2_HEIGHT      (EDP_MAX_HEIGHT / 2)


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
