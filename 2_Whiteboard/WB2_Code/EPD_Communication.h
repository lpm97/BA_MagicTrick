/*****************************************************************************
* | File        :   EPD_Communication.h
* | Author      :   LM & Waveshare team
* | Function    :   Communication with EPD 
*----------------
* | This version:   V1.0
* | Date        :   2021_06_03
/*****************************************************************************/


/* Includes ------------------------------------------------------------------*/
#ifndef _EPD_COM_H_
#define _EPD_COM_H_

#include "Config.h"
#include "EPD_Paint.h"


/* define Functions--------------------------------------------------------------*/
uint8_t EPD_Init(void);
void TurnOnDisplay(void);
void EPD_Sleep(void);
static void EPD_Reset(void);
static uint8_t ReadTemperature_M1(void);

void SPI_WriteByte(uint8_t value);
uint8_t SPI_ReadByte(char);

void SendCommand_M1(uint8_t);
void SendCommand_S1(uint8_t);
void SendCommand_M2(uint8_t);
void SendCommand_S2(uint8_t);
void SendData_M1 (uint8_t);
void SendData_S1 (uint8_t);
void SendData_M2 (uint8_t);
void SendData_S2 (uint8_t);
void SendCommand_M1S1M2S2(uint8_t);
void SendData_M1S1M2S2(uint8_t);
void SendCommand_M1M2(uint8_t);

static void ReadBusy_M1(void);
static void ReadBusy_S1(void);
static void ReadBusy_M2(void);
static void ReadBusy_S2(void);

#endif
