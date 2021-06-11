/* Used Variables ------------------------------------------------------------*/
/*****************************************************************************
* | File        :   ReadTags.ino
* | Author      :   MM & LM
* | Function    :   Control M6E and Scan Tags    
*----------------
* | This version:   V2.0
* | Date        :   2021_06_03
/*****************************************************************************/


#include <SparkFun_UHF_RFID_Reader.h>
RFID nano; //Create instance


uint8_t scannedTags = 0;      // counts the Tags already scanned
uint8_t totalCard[5][14];     // Total amount of Playing Cards
uint8_t totalPuzzle[16][11];  // Total amount of Puzzle Pieces
uint8_t tagLeft[2];           // Identity of the sought-after Tag 


/******************************************************************************
function:  Setum M6E Nano
******************************************************************************/
uint8_t setupReader()
{
  digitalWrite(M6E_EN, HIGH);
  delay(10);
  if (setupNano(115200) == false) //Configure nano to run at 115200bps
  {
    Serial.println(F("Module failed to respond. Please check wiring."));
    return 0;
  }
  nano.setRegion(REGION_EUROPE); //Set to Europe
  nano.setReadPower(2000);    //5.00 dBm. Higher values may caues USB port to brown out
                              //Max Read TX Power is 27.00 dBm and may cause temperature-limit throttling
  return 1;
}




/******************************************************************************
function:  Scan Tags and deside if Cards or Puzzle get scanned
******************************************************************************/
void scanTags()
{
  Debug("ScanTags");
  resetTags();  // set all Tags to 0, expect the defined ones
  delay(10);
  nano.startReading();
  delay(10);
  uint8_t notFinishScanned = HIGH;
  uint8_t flushStack = 0;
  while (notFinishScanned && commandForReader != STOP_SCAN)
  {
    if (nano.check() == true) //Check to see if any new data has come in from module
    {
      byte responseType = nano.parseResponse(); //Break response into tag ID, RSSI, frequency, and timestamp
      if (responseType == RESPONSE_IS_KEEPALIVE)
      {
        Serial.println(F("Scanning"));
      }
      else if (responseType == RESPONSE_IS_TAGFOUND)
      {
        if (flushStack > 7)
        {
          if (nano.msg[32-1] == 0xFE)   // if msg 0xFE the mode is set for cardDetection as defined
          {
            if (cardDetection())
            {
              sendMessageToWB (SHOW_CARD, tagLeft[0], tagLeft[1]);
            }
            notFinishScanned = LOW;
          }
          if (nano.msg[32-1] == 0xFD)   // if msg 0xFD the mode is set for puzzleDetection as defined
          {
            if (nano.msg[32+2] < 6)
            {
              if (puzzleLeftDetection()) 
              {
                sendMessageToPuzzle (PICK_PUZZLE, tagLeft[0], tagLeft[1]);
              }
              notFinishScanned = LOW;
        
            } else if (nano.msg[32+2] >= 6) 
            {
              if (puzzleRightDetection()) 
              {
                sendMessageToPuzzle (PICK_PUZZLE, tagLeft[0], tagLeft[1]);
              }
              notFinishScanned = LOW;
            }
          } 
        }
        flushStack++;
      }
      else if (responseType == ERROR_CORRUPT_RESPONSE)
      {
        Serial.println("Bad CRC");
      }
      else
      {
        //Unknown response
        Serial.print("Unknown error");
      }
    }
    check_GPI();
  }
  delay(10);
  nano.stopReading();
  delay(10);
}






/******************************************************************************
function :  cardDetection -> Scan all available Cardtag
******************************************************************************/
boolean cardDetection (void)
{
  Debug("DetectCards");
  while (scannedTags < 51 && commandForReader != STOP_SCAN)
  {
    if (nano.check() == true) //Check to see if any new data has come in from module
    {
      byte responseType = nano.parseResponse(); //Break response into tag ID, RSSI, frequency, and timestamp
      if (responseType == RESPONSE_IS_KEEPALIVE)
      {
        Serial.println(F("Scanning"));
      }
      else if (responseType == RESPONSE_IS_TAGFOUND)
      {
        scannedTags = 0;
        totalCard[nano.msg[32 + 1]][nano.msg[32 + 2]] = 1;    // set detected Tag as 1
        for (uint8_t i = 1; i < 5; i++)
        {
          for (uint8_t j = 1; j < 14; j++)
          {
            if (totalCard[i][j] == 1)
            {
              scannedTags++;  // increase detected Tags
            }
             if (totalCard[i][j] == 0)
             {
                tagLeft[0] = i; // Suite
                tagLeft[1] = j; // Value 
             }
           }
        }
        Serial.print("Scanned: ");
        Serial.println(scannedTags); 
      }
      else if (responseType == ERROR_CORRUPT_RESPONSE)
      {
        Serial.println("Bad CRC");
      }
      else
      {
        //Unknown response
        Serial.print("Unknown error");
      }
    } 
    check_GPI();
  }
  if (commandForReader == STOP_SCAN)
  {
    return 0;
  }
  Serial.print("One Left!");
  Serial.print("  Suite: ");
  Serial.print(tagLeft[0]);
  Serial.print(", Value: ");
  Serial.println(tagLeft[1]); 
  return 1;
}






/******************************************************************************
function :  Scan all left Puzzletags
******************************************************************************/
boolean puzzleLeftDetection (void)
{
  while (scannedTags < 74 && commandForReader != STOP_SCAN)
  {
    if (nano.check() == true) //Check to see if any new data has come in from module
    {
      byte responseType = nano.parseResponse(); //Break response into tag ID, RSSI, frequency, and timestamp
      if (responseType == RESPONSE_IS_KEEPALIVE)
      {
        Serial.println(F("Scanning"));
      }
      else if (responseType == RESPONSE_IS_TAGFOUND)
      {
        scannedTags = 0;
        totalPuzzle[nano.msg[32 + 1]][nano.msg[32 + 2]] = 1;    // set detected Tag as 1
        for (uint8_t m = 1; m < 16; m++)
        {
          for (uint8_t n = 1; n < 6; n++)
          {
            if (totalPuzzle[m][n] == 1)
            {
              scannedTags++;  // increase detected Tags
            }
            if (totalPuzzle[m][n] == 0)
            {
              tagLeft[0] = m; // Row
              tagLeft[1] = n; // Column
            }
          }
        }
        Serial.print("Scanned: ");
        Serial.println(scannedTags); 
      }
      else if (responseType == ERROR_CORRUPT_RESPONSE)
      {
        Serial.println("Bad CRC");
      }
      else
      {
        //Unknown response
        Serial.print("Unknown error");
      }
    }   
    check_GPI();
  }
  if (commandForReader == STOP_SCAN)
  {
    return 0;
  }
    Serial.print("One Left!");
    Serial.print("  Row: ");
    Serial.print(tagLeft[0]);
    Serial.print(", Column: ");
    Serial.println(tagLeft[1]); 
    return 1;
}





/******************************************************************************
function :  Scan all right Puzzletags
******************************************************************************/
boolean puzzleRightDetection (void)
{
  while (scannedTags < 74 && commandForReader != STOP_SCAN)
  {
    if (nano.check() == true) //Check to see if any new data has come in from module
    {
      byte responseType = nano.parseResponse(); //Break response into tag ID, RSSI, frequency, and timestamp
      if (responseType == RESPONSE_IS_KEEPALIVE)
      {
        Serial.println(F("Scanning"));
      }
      else if (responseType == RESPONSE_IS_TAGFOUND)
      {
        scannedTags = 0;
        totalPuzzle[nano.msg[32 + 1]][nano.msg[32 + 2]] = 1;    // set detected Tag as 1
        for (uint8_t m = 1; m < 16; m++)
        {
          for (uint8_t n = 6; n < 11; n++)
          {
            if (totalPuzzle[m][n] == 1)
            {
              scannedTags++;  // increase detected Tags
            }
            if (totalPuzzle[m][n] == 0)
            {
              tagLeft[0] = m; // Row
              tagLeft[1] = n; // Column
            }
          }
        }
        Serial.print("Scanned: ");
        Serial.println(scannedTags); 
      }
      else if (responseType == ERROR_CORRUPT_RESPONSE)
      {
        Serial.println("Bad CRC");
      }
      else
      {
        //Unknown response
        Serial.print("Unknown error");
      }
    }   
    check_GPI();
  }
  if (commandForReader == STOP_SCAN)
  {
    return 0;
  }
  Serial.print("One Left!");
    Serial.print("  Row: ");
    Serial.print(tagLeft[0]);
    Serial.print(", Column: ");
    Serial.println(tagLeft[1]); 
  return 1;
}





/******************************************************************************
function :  reset scanned Tag-Array
******************************************************************************/
void resetTags (void)
{
  scannedTags = 0;
  tagLeft[0] = 0;
  tagLeft[1] = 0;
  
  for (uint8_t i = 0; i<5; i++)
  {
    for (uint8_t j = 0; j<14; j++)
    {
    totalCard[i][j] = 0;
    }
  }
  for (uint8_t m = 0; m<16; m++)
  {
    for (uint8_t n = 0; n<11; n++)
    {
    totalPuzzle[m][n] = 0;
    }
  } 
  // Puzzle Tags not available, already set to 1
  totalPuzzle[4][3] = 1;
  totalPuzzle[9][7] = 1;

  totalPuzzle[1][5] = 1;
  totalPuzzle[1][6] = 1;
  totalPuzzle[2][5] = 1;
  totalPuzzle[2][6] = 1;
  totalPuzzle[3][5] = 1;
  totalPuzzle[3][6] = 1;
  totalPuzzle[4][5] = 1;
  totalPuzzle[4][6] = 1;
  totalPuzzle[5][5] = 1;
  totalPuzzle[5][6] = 1;

  totalPuzzle[8][1] = 1;
  totalPuzzle[8][10] = 1;
  totalPuzzle[9][1] = 1;
  totalPuzzle[9][10] = 1;
  totalPuzzle[10][1] = 1;
  totalPuzzle[10][10] = 1;
  totalPuzzle[11][1] = 1;
  totalPuzzle[11][10] = 1;
  totalPuzzle[12][1] = 1;
  totalPuzzle[12][10] = 1;
  // scannedCards[1][1] = 1;
  // Serial.read();
  // sendTagInfo(0, 0, 1);
  // Serial.readString();
  delay(100);
  Serial.println("Reset");

  return;
}






/******************************************************************************
function :  setupNano
******************************************************************************/
//Gracefully handles a reader that is already configured and already reading continuously
//Because Stream does not have a .begin() we have to do this outside the library
boolean setupNano(long baudRate)
{
  nano.enableDebugging(Serial); //Print the debug statements to the Serial port

  //nano.begin(softSerial); //Tell the library to communicate over software serial port
  nano.begin(Serial2); //Tell the library to communicate over Teensy Serial Port # 5 (pins 33/34)

  //Test to see if we are already connected to a module
  //This would be the case if the Arduino has been reprogrammed and the module has stayed powered

  //Software serial
  //softSerial.begin(baudRate); //For this test, assume module is already at our desired baud rate
  //while (softSerial.isListening() == false); //Wait for port to open

  //Hardware serial
  Serial2.begin(baudRate); //For this test, assume module is already at our desired baud rate
  while(!Serial2);

  //About 200ms from power on the module will send its firmware version at 115200. We need to ignore this.
  //while (softSerial.available()) softSerial.read();
  while (Serial2.available()) Serial2.read();

  nano.getVersion();

  if (nano.msg[0] == ERROR_WRONG_OPCODE_RESPONSE)
  {
    //This happens if the baud rate is correct but the module is doing a continuous read
    nano.stopReading();

    Serial.println(F("Module continuously reading. Asking it to stop..."));

    delay(1500);
  }
  else
  {
    //The module did not respond so assume it's just been powered on and communicating at 115200bps
    //softSerial.begin(115200); //Start software serial at 115200
    Serial2.begin(115200); //Start serial at 115200

    nano.setBaud(baudRate); //Tell the module to go to the chosen baud rate. Ignore the response msg

    //softSerial.begin(baudRate); //Start the software serial port, this time at user's chosen baud rate
    Serial2.begin(baudRate); //Start the serial port, this time at user's chosen baud rate

    delay(250);
  }

  //Test the connection
  nano.getVersion();

  if (nano.msg[0] != ALL_GOOD) return (false); //Something is not right

  //The M6E has these settings no matter what
  nano.setTagProtocol(); //Set protocol to GEN2

  nano.setAntennaPort(); //Set TX/RX antenna ports to 1

  return true; //We are ready to rock
}
