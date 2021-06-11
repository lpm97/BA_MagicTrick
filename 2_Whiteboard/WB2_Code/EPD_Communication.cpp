/*****************************************************************************
* | File        :   EPD_Communication.cpp
* | Author      :   LM & Waveshare team
* | Function    :   Communication with EPD    
*----------------
* | This version:   V1.0
* | Date        :   2021_06_03
/*****************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "EPD_Communication.h"


/******************************************************************************
function: SPI write
******************************************************************************/
void SPI_WriteByte(uint8_t value)
{
    char i;
    Digital_Write(SCK_PIN, 0);
    for(i = 0; i < 8; i++) {
        Digital_Write(SCK_PIN, 0);
        Delay_us(5);
        if((value << i)  & 0x80) {
            Digital_Write(MOSI_PIN, 1);
        } else {
            Digital_Write(MOSI_PIN, 0);
        }
        Delay_us(5);
        Digital_Write(SCK_PIN, 1);
        Delay_us(5);
    }
}


/******************************************************************************
function: SPI read, not used at the moment
******************************************************************************/
uint8_t SPI_ReadByte(char x)
{
    uint8_t temp;
    return temp;
}

/******************************************************************************
function :  send command to M1
******************************************************************************/
void SendCommand_M1(uint8_t Reg)
{
    Digital_Write(M1S1_DC_PIN, 0);
    Digital_Write(M1_CS_PIN, 0);
    SPI_WriteByte(Reg);
    Digital_Write(M1_CS_PIN, 1);
}


/******************************************************************************
function :  send Data to M1
******************************************************************************/
void SendData_M1 (uint8_t Data)
{
    Digital_Write(M1S1_DC_PIN, 1);
    Digital_Write(M1_CS_PIN, 0);
    SPI_WriteByte(Data);
    Digital_Write(M1_CS_PIN, 1);
}

/******************************************************************************
function :  send command to S1
******************************************************************************/
void SendCommand_S1(uint8_t Reg)
{
    Digital_Write(M1S1_DC_PIN, 0);
    Digital_Write(S1_CS_PIN, 0);
    SPI_WriteByte(Reg);
    Digital_Write(S1_CS_PIN, 1);
}


/******************************************************************************
function :  send Data to S1
******************************************************************************/
void SendData_S1(uint8_t Data)
{
    Digital_Write(M1S1_DC_PIN, 1);
    Digital_Write(S1_CS_PIN, 0);
    SPI_WriteByte(Data);
    Digital_Write(S1_CS_PIN, 1);
}


/******************************************************************************
function :  send command to M2
******************************************************************************/
void SendCommand_M2(uint8_t Reg)
{
    Digital_Write(M2S2_DC_PIN, 0);
    Digital_Write(M2_CS_PIN, 0);
    SPI_WriteByte(Reg);
    Digital_Write(M2_CS_PIN, 1);
}


/******************************************************************************
function :  send Data to M2
******************************************************************************/
void SendData_M2(uint8_t Data)
{
    Digital_Write(M2S2_DC_PIN, 1);
    Digital_Write(M2_CS_PIN, 0);
    SPI_WriteByte(Data);
    Digital_Write(M2_CS_PIN, 1);
}


/******************************************************************************
function :  send command to S2
******************************************************************************/
void SendCommand_S2(uint8_t Reg)
{
    Digital_Write(M2S2_DC_PIN, 0);
    Digital_Write(S2_CS_PIN, 0);
    SPI_WriteByte(Reg);
    Digital_Write(S2_CS_PIN, 1);
}


/******************************************************************************
function :  send Data to S2
******************************************************************************/
void SendData_S2(uint8_t Data)
{
    Digital_Write(M2S2_DC_PIN, 1);
    Digital_Write(S2_CS_PIN, 0);
    SPI_WriteByte(Data);
    Digital_Write(S2_CS_PIN, 1);
}


/******************************************************************************
function :  send command to M1,M2,S1,S2
******************************************************************************/
void SendCommand_M1S1M2S2(uint8_t Reg)
{
    Digital_Write(M1S1_DC_PIN, 0);              // command write
    Digital_Write(M2S2_DC_PIN, 0);   // command write

    Digital_Write(M1_CS_PIN, 0);
    Digital_Write(S1_CS_PIN, 0);
    Digital_Write(M2_CS_PIN, 0);
    Digital_Write(S2_CS_PIN, 0);
    SPI_WriteByte(Reg);
    Digital_Write(M1_CS_PIN, 1);
    Digital_Write(S1_CS_PIN, 1);
    Digital_Write(M2_CS_PIN, 1);
    Digital_Write(S2_CS_PIN, 1);
}


/******************************************************************************
function :  send Data to M1,M2,S1,S2
******************************************************************************/
void SendData_M1S1M2S2(uint8_t Data)
{
    Digital_Write(M1S1_DC_PIN, 1);  // command write
    Digital_Write(M2S2_DC_PIN, 1);  // command write

    Digital_Write(M1_CS_PIN, 0);
    Digital_Write(S1_CS_PIN, 0);
    Digital_Write(M2_CS_PIN, 0);
    Digital_Write(S2_CS_PIN, 0);
    SPI_WriteByte(Data);
    Digital_Write(M1_CS_PIN, 1);
    Digital_Write(S1_CS_PIN, 1);
    Digital_Write(M2_CS_PIN, 1);
    Digital_Write(S2_CS_PIN, 1);
}


/******************************************************************************
function :  send command to M1,M2
******************************************************************************/
void SendCommand_M1M2(uint8_t Reg)
{
    Digital_Write(M1S1_DC_PIN, 0);
    Digital_Write(M2S2_DC_PIN, 0);
    Digital_Write(M1_CS_PIN, 0);
    Digital_Write(M2_CS_PIN, 0);
    SPI_WriteByte(Reg);
    Digital_Write(M1_CS_PIN, 1);
    Digital_Write(M2_CS_PIN, 1);
}


/******************************************************************************
function :	Initialize the e-Paper register
******************************************************************************/
uint8_t EPD_Init(void)
{   
    Debug("EPD_Init");
    Digital_Write(M1_CS_PIN, 1);
    Digital_Write(S1_CS_PIN, 1);
    Digital_Write(M2_CS_PIN, 1);
    Digital_Write(S2_CS_PIN, 1);

    EPD_Reset();
    //panel setting
    SendCommand_M1(0x00);
    SendData_M1(0x1f);	//KW-3f   KWR-2F	BWROTP 0f	BWOTP 1f
    SendCommand_S1(0x00);
    SendData_S1(0x1f);
    SendCommand_M2(0x00);
    SendData_M2(0x13);
    SendCommand_S2(0x00);
    SendData_S2(0x13);
    // booster soft start
    SendCommand_M1(0x06);
    SendData_M1(0x17);	//A
    SendData_M1(0x17);	//B
    SendData_M1(0x39);	//C
    SendData_M1(0x17);
    SendCommand_M2(0x06);
    SendData_M2(0x17);
    SendData_M2(0x17);
    SendData_M2(0x39);
    SendData_M2(0x17);

    //resolution setting
    SendCommand_M1(0x61);
    SendData_M1(0x02);
    SendData_M1(0x88);	//source 648
    SendData_M1(0x01);	//gate 492
    SendData_M1(0xEC);
    SendCommand_S1(0x61);
    SendData_S1(0x02);
    SendData_S1(0x90);	//source 656
    SendData_S1(0x01);	//gate 492
    SendData_S1(0xEC);
    SendCommand_M2(0x61);
    SendData_M2(0x02);
    SendData_M2(0x90);	//source 656
    SendData_M2(0x01);	//gate 492
    SendData_M2(0xEC);
    SendCommand_S2(0x61);
    SendData_S2(0x02);
    SendData_S2(0x88);	//source 648
    SendData_S2(0x01);	//gate 492
    SendData_S2(0xEC);

    SendCommand_M1S1M2S2(0x15);	//DUSPI
    SendData_M1S1M2S2(0x20);

    SendCommand_M1S1M2S2(0x50);	//Vcom and data interval setting
    SendData_M1S1M2S2(0x21); //Border KW
    SendData_M1S1M2S2(0x07);

    SendCommand_M1S1M2S2(0x60);//TCON
    SendData_M1S1M2S2(0x22);

    SendCommand_M1S1M2S2(0xE3);
    SendData_M1S1M2S2(0x00);

    SendCommand_M1S1M2S2(0xe0);//Cascade setting
    SendData_M1S1M2S2(0x03);

    SendCommand_M1S1M2S2(0xe5);//Force temperature
    SendData_M1S1M2S2(ReadTemperature_M1());
    return 0;
}


/******************************************************************************
function :	Turn On Display
parameter:
******************************************************************************/
void TurnOnDisplay(void)
{   
    Debug("Turn on Display...");
    SendCommand_M1M2(0x04); //power on;
    Delay_ms(300);
    SendCommand_M1S1M2S2(0x12); //Display Refresh
    ReadBusy_M1();
    ReadBusy_S1();
    // ReadBusy_M2();
    // Serial.print("ReadBusy_S1 ...\r\n");
    // ReadBusy_S2();
    // Serial.print("ReadBusy_S2 ...\r\n");
}


/******************************************************************************
function :	Enter sleep mode
******************************************************************************/
void EPD_Sleep(void)
{
    Debug("EPD Sleep");
    SendCommand_M1S1M2S2(0X02);  	//power off
    Delay_ms(300);

    SendCommand_M1S1M2S2(0X07);  	//deep sleep
    SendData_M1S1M2S2(0xA5);
    Delay_ms(300);
}


/******************************************************************************
function :	Software reset
******************************************************************************/
static void EPD_Reset(void)
{
    Digital_Write(M1S1_RST_PIN, 0);
    Digital_Write(M2S2_RST_PIN, 0);
    Delay_ms(200);
    Digital_Write(M1S1_RST_PIN, 1);
    Digital_Write(M2S2_RST_PIN, 1);
    Delay_ms(200);
}


/******************************************************************************
function :	Wait until the M1 busy_pin goes LOW
******************************************************************************/
static void ReadBusy_M1(void)
{
    uint8_t busy;
    do 
    {
        SendCommand_M1(0x71);
        busy = Digital_Read(M1_BUSY_PIN);
        busy =!(busy & 0x01);
    } while(busy);
    Delay_ms(200);
}


/******************************************************************************
function :  Wait until the M2 busy_pin goes LOW
******************************************************************************/
static void ReadBusy_M2(void)
{
    uint8_t busy;
    do 
    {
        SendCommand_M2(0x71);
        busy = Digital_Read(M2_BUSY_PIN);
        busy =!(busy & 0x01);
    } while(busy);
    Delay_ms(200);
}


/******************************************************************************
function :  Wait until the S1 busy_pin goes LOW
******************************************************************************/
static void ReadBusy_S1(void)
{
    uint8_t busy;
    do 
    {
        SendCommand_S1(0x71);
        busy = Digital_Read(S1_BUSY_PIN);
        busy =!(busy & 0x01);
    } while(busy);
    Delay_ms(200);
}


/******************************************************************************
function :  Wait until the S2 busy_pin goes LOW
******************************************************************************/
static void ReadBusy_S2(void)
{
    uint8_t busy;
    //do {
    SendCommand_S2(0x71);
    busy = Digital_Read(S2_BUSY_PIN);
    busy =!(busy & 0x01);
    Delay_ms(200);
}


/******************************************************************************
function :  Read Temperature
******************************************************************************/
static uint8_t ReadTemperature_M1(void)
{
    SendCommand_M1(0x40);
    ReadBusy_M1();
    Delay_ms(300);

    uint8_t temp;
    Digital_Write(M1_CS_PIN, 0);
    Digital_Write(S1_CS_PIN, 1);
    Digital_Write(M2_CS_PIN, 1);
    Digital_Write(S2_CS_PIN, 1);

    Digital_Write(M1S1_DC_PIN, 1);
    Delay_us(5);

    temp = SPI_ReadByte(0x00);
    Digital_Write(M1_CS_PIN, 1);

    return temp;
}
