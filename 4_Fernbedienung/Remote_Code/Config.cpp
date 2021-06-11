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
    pinMode(START_PB, INPUT);
    pinMode(DS, OUTPUT);
    pinMode(OE, OUTPUT);
    pinMode(SHCP, OUTPUT);
    pinMode(STCP, OUTPUT);
    pinMode(PG, INPUT);

    // Attach PWM to VIB MOT
    ledcSetup(0, 5000, 8);
    ledcAttachPin(VIBMOT, 0);

    // Init
    
    digitalWrite(DS,  LOW);
    digitalWrite(OE,  HIGH);
    digitalWrite(SHCP,  LOW);
    digitalWrite(STCP, LOW);
    
    pinMode(PWR_KILL, OUTPUT);
    digitalWrite(PWR_KILL,  HIGH);
    
    ledcWrite(0, 0); // 0-0xFF
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
