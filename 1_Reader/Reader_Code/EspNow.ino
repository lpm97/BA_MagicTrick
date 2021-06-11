/*****************************************************************************
* | File        :   EspNow.ino
* | Author      :   MM & LM
* | Function    :   ESP Now Communication mit Reader     
*----------------
* | This version:   V1.0
* | Date        :   2021_06_03
/*****************************************************************************/

#include <esp_now.h>
#include <WiFi.h>

uint8_t espNow_state = Setup;
uint8_t newData = LOW;




// Array with all Mac Adresses
uint8_t broadcastAddress[4][6]    = { 
                                        {0x34, 0xAB, 0x95, 0x49, 0x4E, 0xF8},           // Reader
                                        {0x34, 0xAB, 0x95, 0x49, 0x4E, 0xF0},           // WB
                                        {0x34, 0xAB, 0x95, 0x49, 0x4F, 0x14},           // Puzzle
                                        {0x34, 0xAB, 0x95, 0x49, 0x4E, 0xD8},           // Remote
                                    };    
                                    


// Struct to Send Data to Boards
typedef struct send_struct {
  uint8_t command;
  uint8_t dataX;
  uint8_t dataY;
} send_struct;

send_struct reader;
send_struct wb;
send_struct puzzle;
send_struct remote;
// Create an array with all the send structures
send_struct boardData[4] = {reader, wb, puzzle, remote};

// Struct to received Data from Boards
typedef struct receive_struct {
  uint8_t id;
  uint8_t state;
  uint8_t command;
}receive_struct;

receive_struct myData;
receive_struct board0;
receive_struct board1;
receive_struct board2;
receive_struct board3;

// Create an array with all the received structures
receive_struct boardsStatus[4] = {board0, board1, board2, board3};

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  Serial.print("Send Packet to: ");
  // Copies the sender mac address to a string
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
  Serial.print(" send status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  Serial.println();
}


// callback when data is received
void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
  char macStr[18];
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.printf("Received Data from Board ID %u: \n", myData.id);
  // Update the structures with the new incoming data
  boardsStatus[myData.id].state = myData.state;
  boardsStatus[myData.id].command = myData.command;
  Serial.printf("Status: %d \n", boardsStatus[myData.id].state);
  Serial.printf("Command: %d \n", boardsStatus[myData.id].command);
  Serial.println();
  isNewState();
  isNewCommand(myData.command);
}



/******************************************************************************
function:  Setup EspNow and add Peer
******************************************************************************/
void setup_Esp_Now() 
{
  Serial.begin(115200);
 
  WiFi.mode(WIFI_STA);
 
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  esp_now_register_send_cb(OnDataSent);
  
  
  // register peer
  esp_now_peer_info_t peerInfo;
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  // register first peer  
  memcpy(peerInfo.peer_addr, broadcastAddress[WB], 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  // register second peer  
  memcpy(peerInfo.peer_addr, broadcastAddress[PUZZLE], 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  /// register third peer
  memcpy(peerInfo.peer_addr, broadcastAddress[REMOTE], 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
}


/******************************************************************************
function:  Send Message to WB (Command and Card Info)
******************************************************************************/
void sendMessageToWB (uint8_t commandForWB, uint8_t card_suite, uint8_t card_value)
{
  boardData[WB].command = commandForWB;
  boardData[WB].dataX = card_suite;
  boardData[WB].dataY = card_value;
  esp_err_t result = esp_now_send(broadcastAddress[WB], (uint8_t *) &boardData[WB], sizeof(send_struct));
  if (commandForWB==SHOW_CARD)
  {
    sendInfosToRemote (card_suite, card_value);
  }
}



/******************************************************************************
function:  Send Message to Puzzle (Command and Puzzle Info)
******************************************************************************/
void sendMessageToPuzzle (uint8_t commandForPuzzle, uint8_t row, uint8_t column)
{
  boardData[PUZZLE].command = commandForPuzzle;
  boardData[PUZZLE].dataX = row;
  boardData[PUZZLE].dataY = column;
  esp_err_t result = esp_now_send(broadcastAddress[PUZZLE], (uint8_t *) &boardData[PUZZLE], sizeof(send_struct));  
}



/******************************************************************************
function:  Send Status to Remote (Sends Status from Reader, WB and Puzzle)
******************************************************************************/
void sendStatusToRemote ()
{
  boardData[REMOTE].command = state;
  boardData[REMOTE].dataX = wb_state;
  boardData[REMOTE].dataY = puzzle_state;
  esp_err_t result = esp_now_send(broadcastAddress[REMOTE], (uint8_t *) &boardData[REMOTE], sizeof(send_struct));
}


/******************************************************************************
function:  Send VibInfos to Remote ( 2 Nr)
******************************************************************************/
void sendInfosToRemote (uint8_t dataA, uint8_t dataB)
{
  boardData[REMOTE].command = SCAN_FINISH;
  boardData[REMOTE].dataX = dataA;
  boardData[REMOTE].dataY = dataB;
  esp_err_t result = esp_now_send(broadcastAddress[REMOTE], (uint8_t *) &boardData[REMOTE], sizeof(send_struct));
}


/******************************************************************************
function:  Check if State from a Module has changed and deside what to with that
******************************************************************************/
void isNewState()
{
  static uint8_t reader_stateOld = TurnOff;

  if (reader_stateOld != state || boardsStatus[WB].state != wb_state || boardsStatus[PUZZLE].state != puzzle_state)
  {
    reader_stateOld = state;
    wb_state = boardsStatus[WB].state;
    puzzle_state = boardsStatus[PUZZLE].state;
    remote_state = boardsStatus[REMOTE].state;
    sendStatusToRemote ();
  }
}


/******************************************************************************
function:  Check if Reader needs to start/stop Scan
******************************************************************************/
void isNewCommand(uint8_t newCommand)
{
  if (newCommand == START_SCAN && commandForReader == NOTHING)
  {
    commandForReader = START_SCAN;
  }
  else if (newCommand == START_SCAN && commandForReader == START_SCAN)
  {
    commandForReader = STOP_SCAN;
  }
  else if (newCommand == STOP_SCAN && commandForReader == START_SCAN)
  {
    commandForReader = STOP_SCAN;
  }
}
