#include <WiFi.h>
#include "ntp/NTPClient.h"
#include "webserver/WiFiNetworks.h"
#include "rcswitch/RCSwitch.h"  // https://github.com/sui77/rc-switch
#include "webserver/Webserver.h"
#include "utils/utils.h"
#include "logger/logger.h"

#include <IRremote.h>
#include <cJSON.h>
#include <list>

////////////////////////////////////////////////////////////////////////

#define _IR
#define _443MHz
// #define _868MHz

// steckplan [src\img\steckplan.png]

#define IO_RECEIVER_PIN 0
#define IO_TRANSMIT_PIN 4

// #define IR_RECEIVER_PIN 25
#define IR_RECEIVER_PIN 25
#define IR_SENDER_PIN 14

/*
 * Data of Access Point that the ESP32 will create
 */
#define AP_SSID "leo-SubGhzDecoder"
#define AP_PASSWORD "nothankyou"

#define IR_SEND_REPEAT_CNT 4

char *turnOnLampTri = "00000FFFFF0F";
char *turnOnLampBin = "000000000001010001010001";

// char *turnOffLampTri = "00000FFFFF11";                // ?????
// char *turnOffLampBin = "000000000001010101011111";    // ?????

char *turnOffLampTri = "00000FF0FFF0";
char *turnOffLampBin = "000000000001010001010100";

////////////////////////////////////////////////////////////////////////

RCSwitch receiverSwitch = RCSwitch();
RCSwitch senderSwitch = RCSwitch();

std::list<String> receivedDataList_443MHz;
std::list<String> receivedDataList_IR;

std::list<String> dateList_443MHz;
std::list<String> dateList_IR;

Queue<String> sendDataQueue_443MHz;
// FORMAT: <address:int>;<command:int>;<protocol:string> 
Queue<String> sendDataQueue_IR;

#pragma region setup
void setup() {
  // pinMode(0, HIGH);
  Serial.begin(115200);

  NTPClient_init(wifiNetworks);

  // Receiver on interrupt 0 => that is GPIO0
  receiverSwitch.enableReceive(IO_RECEIVER_PIN);
  senderSwitch.enableTransmit(IO_TRANSMIT_PIN);
  IrReceiver.begin(IR_RECEIVER_PIN, DISABLE_LED_FEEDBACK);
  IrReceiver.resume();
  IrSender.begin(IR_SENDER_PIN, DISABLE_LED_FEEDBACK, 0);

  Webserver_init(AP_SSID, AP_PASSWORD); 

  delay(1000);
}
#pragma endregion

void loop() {
	#pragma region IR
  #ifdef  _IR
  if (IrReceiver.decode()) {
    try {
      IRData data = IrReceiver.decodedIRData;
      if (strcmp(getProtocolString(data.protocol), "UNKNOWN") != 0 && data.command != 0 && data.decodedRawData != 0) {
        uint16_t address = data.address;
        uint16_t command = data.command;
        char *protocol = (char *)(getProtocolString(data.protocol));
        char *hex = dec2binWzerofill(data.decodedRawData, 64);
        char *bin = binaryToHex(hex);
        uint64_t dec = data.decodedRawData;

        cJSON *root = cJSON_CreateObject();
        cJSON_AddNumberToObject(root, "address", address);
        cJSON_AddNumberToObject(root, "command", command);
        cJSON_AddStringToObject(root, "protocol", protocol);
        cJSON_AddStringToObject(root, "hex", hex);
        cJSON_AddStringToObject(root, "bin", bin);
        cJSON_AddNumberToObject(root, "dec", dec);

        char *serialized = cJSON_Print(root);
        String result(serialized);
        result = Webserver_shortenJSON(result);

        Logger::log(LogSeverity::DEBUG, result.c_str(), true);
        Logger::log(LogSeverity::DEBUG, "\n");
        Logger::log(LogSeverity::DEBUG, "------------------------------\n", true);

        Webserver_appendToListWithTime(&receivedDataList_IR, &dateList_IR, result);
      }
    }
    catch (const std::exception &e) {
      Logger::log(LogSeverity::ERROR, e.what(), true);
    }
    IrReceiver.resume();
  }
  IrReceiver.resume();
  #endif
	#pragma endregion

	#pragma region 443MHz
  #ifdef _443MHz
  if (receiverSwitch.available()) {
    unsigned long decimal = receiverSwitch.getReceivedValue();
    unsigned int length = receiverSwitch.getReceivedBitlength();
    unsigned int delay = receiverSwitch.getReceivedDelay();
    unsigned int *raw = receiverSwitch.getReceivedRawdata();
    unsigned int protocol = receiverSwitch.getReceivedProtocol();
    char *bin = dec2binWzerofill(decimal, length);
    char *tri = (char *)(bin2tristate(bin));
    char *hex = binaryToHex(bin);

    cJSON *root = cJSON_CreateObject();

    cJSON_AddNumberToObject(root, "decimal", decimal);
    cJSON_AddStringToObject(root, "hex", hex);
    cJSON_AddNumberToObject(root, "length", length);
    cJSON_AddNumberToObject(root, "delay", delay);
    cJSON_AddNumberToObject(root, "protocol", protocol);
    cJSON_AddStringToObject(root, "tri", tri);
    cJSON_AddStringToObject(root, "bin", bin);

    cJSON *rawArray = cJSON_CreateArray();
    for (unsigned int i = 0; i < length; ++i) {
      cJSON_AddItemToArray(rawArray, cJSON_CreateNumber(raw[i]));
    }
    cJSON_AddItemToObject(root, "raw", rawArray);

    char *serialized = cJSON_Print(root);
    String result(serialized);

    cJSON_Delete(root);
    free(serialized);
    
    result.replace("\t", "");
    result.replace("\n", "");

    Webserver_appendToListWithTime(&receivedDataList_443MHz, &dateList_443MHz, result);

    Logger::log(LogSeverity::DEBUG, result.c_str(), true);
    Logger::log(LogSeverity::DEBUG, "------------------------------", true);
    receiverSwitch.resetAvailable();
  }
  #endif
	#pragma endregion

  #pragma region Webserver
  Webserver_handleClient(
    &sendDataQueue_443MHz, 
    &sendDataQueue_IR, 
    &receivedDataList_443MHz, 
    &receivedDataList_IR, 
    &dateList_443MHz, 
    &dateList_IR
  );
  #pragma endregion

  //////////////////////////////////////////////////////////////////////////

  #pragma region 443MHz Queue
  #ifdef _443MHz
  if (!sendDataQueue_443MHz.isEmpty()) {
    String dataToSend = sendDataQueue_443MHz.front();
    Logger::log(LogSeverity::DEBUG, "Sending data: ", true);
    Logger::log(LogSeverity::DEBUG, dataToSend.c_str());
    Logger::log(LogSeverity::DEBUG, "\n");
    dataToSend.trim();
    char *rawDataToSend = (char *)(dataToSend.c_str());
    sendDataQueue_443MHz.dequeue();

    senderSwitch.resetAvailable();
    senderSwitch.setPulseLength(320);
    senderSwitch.setRepeatTransmit(15);
    senderSwitch.send(rawDataToSend);
  }
  #endif
  #pragma endregion

  #pragma region IR Queue
  #ifdef _IR
  if (!sendDataQueue_IR.isEmpty()) {
    String dataToSend = sendDataQueue_IR.front();
    int count = 0;
    char **splitData = split(dataToSend.c_str(), ';', count);
    if (count == 3) {
      uint16_t address = (uint16_t)atoi(splitData[0]);
      uint16_t command = (uint16_t)atoi(splitData[1]);
      char *protocol = splitData[2];
      bool canSendProtocoll = true;
      if (strcasecmp(protocol, "APPLE")) {
        IrSender.sendApple(address, command, IR_SEND_REPEAT_CNT);
      } else if (strcasecmp(protocol, "DENON")) {
        IrSender.sendDenon(address, command, IR_SEND_REPEAT_CNT);
      } else if (strcasecmp(protocol, "JVC")) {
        IrSender.sendJVC((uint8_t)(address), (uint8_t)(command), (int_fast8_t)IR_SEND_REPEAT_CNT);
      } else if (strcasecmp(protocol, "LG")) {
        IrSender.sendLG(address, command, IR_SEND_REPEAT_CNT);
      } else if (strcasecmp(protocol, "LG2")) {
        IrSender.sendLG2(address, command, IR_SEND_REPEAT_CNT);
      } else if (strcasecmp(protocol, "NEC")) {
        IrSender.sendNEC(address, command, IR_SEND_REPEAT_CNT);
      } else if (strcasecmp(protocol, "NEC2")) {
        IrSender.sendNEC2(address, command, IR_SEND_REPEAT_CNT);
      } else if (strcasecmp(protocol, "ONKYO")) {
        IrSender.sendOnkyo(address, command, IR_SEND_REPEAT_CNT);
      } else if (strcasecmp(protocol, "PANASONIC")) {
        IrSender.sendPanasonic(address, command, IR_SEND_REPEAT_CNT);
      } else if (strcasecmp(protocol, "KASEIKYO_DENON")) {
        IrSender.sendKaseikyo_Denon(address, command, IR_SEND_REPEAT_CNT);
      } else if (strcasecmp(protocol, "KASEIKYO_SHARP")) {
        IrSender.sendKaseikyo_Sharp(address, command, IR_SEND_REPEAT_CNT);
      } else if (strcasecmp(protocol, "KASEIKYO_JVC")) {
        IrSender.sendKaseikyo_JVC(address, command, IR_SEND_REPEAT_CNT);
      } else if (strcasecmp(protocol, "KASEIKYO_MITSUBISHI")) {
        IrSender.sendKaseikyo_Mitsubishi(address, command, IR_SEND_REPEAT_CNT);
      } else if (strcasecmp(protocol, "RC5")) {
        IrSender.sendRC5(address, command, IR_SEND_REPEAT_CNT);
      } else if (strcasecmp(protocol, "RC6")) {
        IrSender.sendRC6(address, command, IR_SEND_REPEAT_CNT);
      } else if (strcasecmp(protocol, "SAMSUNG")) {
        IrSender.sendSamsung(address, command, IR_SEND_REPEAT_CNT);
      } else if (strcasecmp(protocol, "SAMSUNGLG")) {
        IrSender.sendSamsungLG(address, command, IR_SEND_REPEAT_CNT);
      } else if (strcasecmp(protocol, "SAMSUNG48")) {
        IrSender.sendSamsung48(address, command, IR_SEND_REPEAT_CNT);
      } else if (strcasecmp(protocol, "SHARP")) {
        IrSender.sendSharp(address, command, IR_SEND_REPEAT_CNT);
      } else if (strcasecmp(protocol, "SONY")) {
        IrSender.sendSony(address, command, IR_SEND_REPEAT_CNT);
      } else if (strcasecmp(protocol, "BANG_OLUFSEN")) {
        IrSender.sendBangOlufsen(address, command, IR_SEND_REPEAT_CNT);
      } else if (strcasecmp(protocol, "LEGO_PF")) {
        IrSender.sendLegoPowerFunctions(address, command, IR_SEND_REPEAT_CNT);
      } else if (strcasecmp(protocol, "FAST")) {
        IrSender.sendFAST(address, IR_SEND_REPEAT_CNT);
      } else {
        Logger::log(LogSeverity::ERROR, "Unknown / Not supported protocol: ", true);
        Logger::log(LogSeverity::ERROR, protocol);
        Logger::log(LogSeverity::ERROR, "\n");
        canSendProtocoll = false;
      }

      Logger::log(LogSeverity::DEBUG, "Sent IR data: ", true);
      Logger::log(LogSeverity::DEBUG, dataToSend.c_str());
      Logger::log(LogSeverity::DEBUG, "\n");
      Logger::log(LogSeverity::DEBUG, "Address: ", true);
      Logger::log(LogSeverity::DEBUG, String(address).c_str());
      Logger::log(LogSeverity::DEBUG, "\n");
      Logger::log(LogSeverity::DEBUG, "Command: ", true);
      Logger::log(LogSeverity::DEBUG, String(command).c_str());
      Logger::log(LogSeverity::DEBUG, "\n");
      Logger::log(LogSeverity::DEBUG, "Protocol: ", true);
      Logger::log(LogSeverity::DEBUG, protocol);
      Logger::log(LogSeverity::DEBUG, "\n");
      Logger::log(LogSeverity::DEBUG, "Was success: ", true);
      Logger::log(LogSeverity::DEBUG, canSendProtocoll ? "true" : "false");
      Logger::log(LogSeverity::DEBUG, "------------------------------", true);
      Logger::log(LogSeverity::DEBUG, "\n");
    }

    sendDataQueue_IR.dequeue();
  }
  #endif
  #pragma endregion
}