/*****************************************************************************
* | File      	:   Config.cpp
* | Author      :   MM & LM
* | Function    :   GPIO declaration and Module Initialisation    
*----------------
* |	This version:   V1.0
* | Date        :   2021_06_03
/*****************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "Config.h"

/******************************************************************************
function:  Set GPIO In/Output
******************************************************************************/
void GPIO_Config(void)
{    
    // GPIO
    pinMode(TURN_OFF, INPUT);
    pinMode(BAT_MONITOR, INPUT);
    pinMode(BUTTON, INPUT);
    pinMode(PWR_KILL, OUTPUT);
    pinMode(DS, OUTPUT);
    pinMode(SHCP, OUTPUT);
    pinMode(STCP, OUTPUT);
    pinMode(OE, OUTPUT);
    pinMode(PG, INPUT);
    pinMode(M6E_EN, OUTPUT);

    pinMode(PWR_KILL, OUTPUT);
    
    // Init
    digitalWrite(PWR_KILL,  HIGH);
    digitalWrite(M6E_EN, LOW);
    digitalWrite(DS,  LOW);
    digitalWrite(OE,  HIGH);
    digitalWrite(SHCP,  LOW);
    digitalWrite(STCP, LOW);
}


/******************************************************************************
function:	Initialise GPIO and Serial Output
******************************************************************************/
uint8_t moduleInit(void)
{ 
    Debug("ModulInit");
    GPIO_Config();
    return 0;
}
