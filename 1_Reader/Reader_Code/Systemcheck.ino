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
  showStatus();
  sendStatusToRemote ();
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
function:  Stop Timer Interrupt
******************************************************************************/
void stopSystemCheckISR ()
{
  Serial.println("Stop Timer");
  timerAlarmDisable(timer);     // stop alarm
  timerDetachInterrupt(timer);  // detach interrupt
  timerEnd(timer);              // end timer   
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
    isNewState();
    showStatus();
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
  static uint8_t buttonOld = HIGH;
  uint8_t buttonNow = Digital_Read(BUTTON);
  uint8_t turnOff = Digital_Read(TURN_OFF);
  static uint8_t turnOffOld = HIGH;
  static uint8_t turnOffTimer;
  
  if (buttonNow == LOW && buttonOld == HIGH)
  {
    if (commandForReader==START_SCAN)
    {
      Debug("Stop Scan");  
      commandForReader = STOP_SCAN;   
    }
    else
    {
      Debug("Start Scan");  
      commandForReader = START_SCAN;
    }
  }
  
  if (turnOff == HIGH && turnOffOld == LOW)
  {
    if (visibleStatus==HIGH)
    {
      visibleStatus=LOW;
    }
    else
    {
      visibleStatus=HIGH;
    }
    showStatus(); 
  }
  
  if (turnOff == LOW && turnOffOld == LOW)
  { 
    turnOffTimer++;
    if (turnOffTimer>5)
    {
      turnDeviceOff();
    }
  }
  else
  {
    turnOffTimer=0;
  }
  
  turnOffOld = turnOff;
  buttonOld = buttonNow;
}

/******************************************************************************
function:  Show Status on LED
******************************************************************************/
void showStatus()
{
  uint8_t colourBits = 0;
  
  if      (batCapacity>=80)   colourBits = Bat80;
  else if (batCapacity>=60)   colourBits = Bat60;
  else if (batCapacity>=40)   colourBits = Bat40;
  else if (batCapacity>=20)   colourBits = Bat20;
  else if (batCapacity>=10)   colourBits = Bat10;
  else                        colourBits = Bat0; 
  colourBits= colourBits<<3 ;
//  Debug("BatCap:");
//  Debug(batCapacity);

  if (visibleStatus == HIGH)
  {
    switch (state)
    {
      case Setup:   colourBits    |= SetupLED;    break;
      case Ready:   colourBits    |= ReadyLED;    break;
      case Working: colourBits    |= WorkingLED;  break;
      case Error:   colourBits    |= ErrorLED;    break;
    }
  }
  
 

  for (uint8_t x=0; x<8; x++)
  { 
      if (((colourBits<<x) & 0x80) > 1)
      {
        Digital_Write(DS, HIGH); 
      }
      else 
      {
        Digital_Write(DS, LOW); 
      }
      delay(5);
      Digital_Write(SHCP, HIGH);
      delay(5);
      Digital_Write(SHCP, LOW);
  }
  
  Digital_Write(OE, HIGH);
  delay(5);
  Digital_Write(STCP, HIGH);  
  delay(5);
  Digital_Write(OE, LOW);
  Digital_Write(STCP, LOW);  
  //Debug(colourBits);
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
  //@@
  digitalWrite(PWR_KILL,  LOW);
  return;  
}
