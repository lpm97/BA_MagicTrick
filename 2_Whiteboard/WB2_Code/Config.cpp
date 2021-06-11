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
    // SPI
    pinMode(SCK_PIN, OUTPUT);
    pinMode(MOSI_PIN, OUTPUT);

    // GPIO
    pinMode(M1_CS_PIN,  OUTPUT);
    pinMode(S1_CS_PIN,  OUTPUT);
    pinMode(M2_CS_PIN,  OUTPUT);
    pinMode(S2_CS_PIN,  OUTPUT);
    pinMode(M1S1_DC_PIN,  OUTPUT);
    pinMode(M2S2_DC_PIN,  OUTPUT);
    pinMode(M1S1_RST_PIN,  OUTPUT);
    pinMode(M2S2_RST_PIN,  OUTPUT);
    pinMode(LED, OUTPUT);
    
    pinMode(M1_BUSY_PIN,  INPUT);
    pinMode(S1_BUSY_PIN,  INPUT);
    pinMode(M2_BUSY_PIN,  INPUT);
    pinMode(S2_BUSY_PIN,  INPUT);
    pinMode(TURN_OFF, INPUT);
    pinMode(PG, INPUT);
    pinMode(BAT_MONITOR, INPUT);
    pinMode(MAG_POS1, INPUT);
    pinMode(MAG_POS2, INPUT);
    
    // Init
    digitalWrite(M1_CS_PIN,  HIGH);
    digitalWrite(S1_CS_PIN,  HIGH);
    digitalWrite(M2_CS_PIN,  HIGH);
    digitalWrite(S2_CS_PIN,  HIGH);
    digitalWrite(SCK_PIN, LOW);

    pinMode(PWR_KILL, OUTPUT);
    digitalWrite(PWR_KILL,  HIGH);
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
