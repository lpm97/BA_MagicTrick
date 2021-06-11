/*****************************************************************************
* | File        :   EspNow.ino
* | Author      :   MM & LM
* | Function    :   ESP Now Communication mit Reader
*----------------
* | This version:   V1.0
* | Date        :   2021_06_03
/*****************************************************************************/

uint8_t espNow_state = Setup;
uint8_t newData = LOW;

// Array with all Mac Adresses
uint8_t broadcastAddress[4][6]    = { 
                                        {0x34, 0xAB, 0x95, 0x49, 0x4E, 0xF8},           // Reader
                                        {0x34, 0xAB, 0x95, 0x49, 0x4E, 0xF0},           // WB
                                        {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},           // Puzzle
                                        {0x34, 0xAB, 0x95, 0x49, 0x4E, 0xD8},           // Remote
                                    };    


// Data Structure for Reader
typedef struct sendStruct {
    uint8_t id;
    uint8_t state;
    uint8_t commandForReader;
} sendStruct;

// Data Structure from Reader
typedef struct receiveStruct {
    uint8_t dataX;
    uint8_t dataY;
    uint8_t dataZ;
} receiveStruct;

sendStruct sendData;
receiveStruct receiveFromReader;


/******************************************************************************
function:  Callback when data is sent
******************************************************************************/
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Debug("Send Status:\t");
  Debug(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  Debug(sendData.id);
  Debug(sendData.state);
  Debug(sendData.commandForReader);
}


/******************************************************************************
function:  Callback when data is received
******************************************************************************/
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&receiveFromReader, incomingData, sizeof(receiveFromReader));
  if (receiveFromReader.dataX == PICK_PUZZLE)
  {
    commandFromReader = PICK_PUZZLE;
    puzzle_row = receiveFromReader.dataY;
    puzzle_columnnn = receiveFromReader.dataZ;
  }
  
  Debug("Received from READER:");
  Debug(receiveFromReader.dataX);
  Debug(receiveFromReader.dataY);
  Debug(receiveFromReader.dataZ);
}


/******************************************************************************
function:  Setup ESP NOW Communication
******************************************************************************/
void setup_Esp_Now() 
{
  Serial.begin(115200);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Debug("Error initializing ESP-NOW");
    espNow_state = Error;
    change_status (Error, WIFI_NOT_GOOD);
    return;
  }
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress[READER], 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Debug("Failed to add peer");
    espNow_state = Error;
    change_status (Error, WIFI_NOT_GOOD);
    return;
  }
  
  // Callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
  Debug("ESP NOW Ready");
  espNow_state = Ready;
  sendDataToReader();
}


/******************************************************************************
function:  Send Data to Reader
******************************************************************************/
void sendDataToReader()
{ 
  if (espNow_state != Ready) 
  {  return;  }  
  
  sendData.id = id;
  sendData.state = state;
  sendData.commandForReader = commandForReader;
  
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress[READER], (uint8_t *) &sendData, sizeof(sendData));
  commandForReader = NOTHING;
}
