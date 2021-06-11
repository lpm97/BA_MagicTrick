/*****************************************************************************
* | File        :   Systemcheck.ino
* | Author      :   MM & LM
* | Function    :   Check GPIO, TimerInterrupt, LED and Power        
*----------------
* | This version:   V1.0
* | Date        :   2021_06_03
/*****************************************************************************/

uint8_t batCapacity = 0;              // 0 - 100%, 200 = External Power
hw_timer_t *timer = NULL;             // Hardware Timer


/******************************************************************************
function:  Timer Interrupt
******************************************************************************/
void IRAM_ATTR systemCheck() 
{
  Debug("SystemCheck");
  checkBatCapacity();
  sendDataToReader();
}


/******************************************************************************
function:  Setup Timer Interrupt
******************************************************************************/
void systemCheckISR(uint32_t time_ms) 
{
  timer = timerBegin(0, 80, true);                    //timer 0, div 80 (80MHz -> 1MHz)
  timerAttachInterrupt(timer, &systemCheck, true);    //attach callback
  timerAlarmWrite(timer, time_ms * 1000, true);       //set time in us
  timerAlarmEnable(timer);    
}


/******************************************************************************
function:  Change Status of Device
******************************************************************************/
uint8_t change_status (uint8_t newStatus, uint8_t errorCode)
{
  if (state!=Error || newStatus == TurnOff)
  {
    state = newStatus;
    errorMessage = errorCode;
    sendDataToReader();
    return 1;
  }
  else
  {
    return 0;  
  }
}


/******************************************************************************
function:  Check HW Inputs
******************************************************************************/
void check_GPI()
{
  uint8_t button = Digital_Read(OPEN_PB);
  uint8_t turnOff = Digital_Read(TURN_OFF);
  static uint8_t turnOffOld = HIGH;
  static uint8_t buttonOld = HIGH;
  
  if (button == LOW && buttonOld == HIGH)
  {
    change_status (Working, NOTHING);
    releasePuzzle();
    change_status (Ready, NOTHING);
  }
  
  if (turnOff == LOW && turnOffOld == HIGH)
  {
    turnDeviceOff ();
  }
  
  turnOffOld = turnOff;
  buttonOld = button;
}



/******************************************************************************
function:  Check Battery batCapacity
******************************************************************************/
uint8_t checkBatCapacity()
{
  uint16_t batVoltage = analogRead(BAT_MONITOR);   // 0-3.3V -> 0 4095 
  batCapacity = 0;
  if (batVoltage>2239) batCapacity = 5;
  if (batVoltage>2289) batCapacity = 10;
  if (batVoltage>2301) batCapacity = 15;
  if (batVoltage>2314) batCapacity = 20;
  if (batVoltage>2326) batCapacity = 25;
  if (batVoltage>2339) batCapacity = 30;
  if (batVoltage>2351) batCapacity = 35;
  if (batVoltage>2357) batCapacity = 40;
  if (batVoltage>2370) batCapacity = 45;
  if (batVoltage>2382) batCapacity = 50;
  if (batVoltage>2388) batCapacity = 55;
  if (batVoltage>2401) batCapacity = 60;
  if (batVoltage>2425) batCapacity = 65;
  if (batVoltage>2450) batCapacity = 70;
  if (batVoltage>2469) batCapacity = 75;
  if (batVoltage>2494) batCapacity = 80;
  if (batVoltage>2531) batCapacity = 85;
  if (batVoltage>2550) batCapacity = 90;
  if (batVoltage>2574) batCapacity = 95;
  if (batVoltage>2605) batCapacity = 100;
  if (batVoltage>2650) batCapacity = 200;
  
  if (batCapacity<=5)     change_status(Error, POWER_NOT_GOOD);
  //Debug(batCapacity);
  return batCapacity;
}


/******************************************************************************
function:  Turn Device Off
******************************************************************************/
void turnDeviceOff ()
{
  change_status (TurnOff, NOTHING);
  Debug("TurnOff...");
  delay(100);
  digitalWrite(PWR_KILL,  LOW);
  return;  
}
