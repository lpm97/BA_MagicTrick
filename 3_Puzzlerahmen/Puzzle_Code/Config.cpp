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
    pinMode(OPEN_PB, INPUT);
    pinMode(TP2, OUTPUT);
    pinMode(DS, OUTPUT);
    pinMode(SHCP, OUTPUT);
    pinMode(STCP, OUTPUT);
    pinMode(OE, OUTPUT);
    pinMode(SLEEP_SOLE, OUTPUT);
    pinMode(IN1_SOLE, OUTPUT);
    pinMode(IN2_SOLE, OUTPUT);
    pinMode(MODE1, OUTPUT);
    pinMode(MODE0, INPUT);
    pinMode(SLEEP_M3, OUTPUT);
    pinMode(DIR_M3, OUTPUT);
    pinMode(STEP_M3, OUTPUT);
    pinMode(SLEEP_M2, OUTPUT);
    pinMode(DIR_M2, OUTPUT);
    pinMode(STEP_M2, OUTPUT);
    pinMode(SLEEP_M1, OUTPUT);
    pinMode(DIR_M1, OUTPUT);
    pinMode(STEP_M1, OUTPUT);
    pinMode(PWR_KILL, OUTPUT);
    
    
    // Init
    digitalWrite(PWR_KILL,  HIGH);
    digitalWrite(MODE1, HIGH);
    digitalWrite(SLEEP_M1, LOW);
    digitalWrite(SLEEP_M2, LOW);
    digitalWrite(SLEEP_M3, LOW);
    digitalWrite(OE, HIGH);
    digitalWrite(IN2_SOLE, LOW);
    digitalWrite(SLEEP_M1, LOW);
    digitalWrite(SLEEP_M2, LOW);
    digitalWrite(SLEEP_M3, LOW);
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


/******************************************************************************
function: Microsecond delay
******************************************************************************/
void Delay_us(uint16_t xus)
{
    uint16_t i;
    while(xus) 
    {
        for(i = 0; i < 30; i++);
        xus--;
    }
}
