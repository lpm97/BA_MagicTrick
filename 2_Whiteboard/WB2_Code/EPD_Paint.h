/*****************************************************************************
* | File        :   EPD_Paint.h
* | Author      :   LM & Waveshare team
* | Function    :   Hardware underlying interface   
*----------------
* | This version:   V1.0
* | Date        :   2021_06_03
/*****************************************************************************/


/* Includes ------------------------------------------------------------------*/
#ifndef __EPD_PAINT_H
#define __EPD_PAINT_H

#include "Config.h"
#include "EPD_Communication.h"
#include "z_Bitmaps_Suits.h"
#include "z_Bitmaps_Value.h"
#include "z_Bitmap_Bat.h"


/* define Functions-----------------------------------------------------------*/
void clearDisplay(void);
void drawCard(uint8_t, uint8_t);
void drawTop(uint8_t);
void drawBot(uint8_t);
void drawBatCap (uint8_t);
#endif
