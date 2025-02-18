#include "NTPClient.h"

#include <WiFi.h>
#include <HTTPClient.h>
#include <time.h>
#include <regex>
#include "ntp/Timezones.h"
#include "logger/logger.h"

#define private static
#define public /**/

int base_seconds;

private unsigned long extractUnixtime(const char* jsonString) {
  std::string jsonStr(jsonString);
  std::regex pattern("\"unixtime\"\\s*:\\s*(\\d+)");
  std::smatch match;
  if (std::regex_search(jsonStr, match, pattern)) {
    std::string unixtimeStr = match[1];
    return std::stoul(unixtimeStr);
  } else {
    return 0;
  }
}

private char *getAbbreviationFromJSON(const char *jsonString) {
  std::string jsonStr(jsonString);
  std::regex pattern("\"abbreviation\"\\s*:\\s*\"([^\"]*)\"");
  std::smatch match;
  if (std::regex_search(jsonStr, match, pattern)) {
    return (char *)(match[1].str().c_str());
  } else {
    return "";
  }
}

private char NTPClient_Connect(const char *ssid, const char *password) {
  Logger::log(LogSeverity::DEBUG, "Connecting to ", true);
  Logger::log(LogSeverity::DEBUG, ssid);
  Logger::log(LogSeverity::DEBUG, " ");
  WiFi.begin(ssid, password);
  bool result = false;
  
  int connectionAttempts = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    connectionAttempts++;
    Logger::log(LogSeverity::DEBUG, ".");
    if (connectionAttempts > 8) {
      Logger::log(LogSeverity::DEBUG, " failed\n");
      return false;
    }
  }
  Logger::log(LogSeverity::DEBUG, " connected\n");
  return true;
}

public bool NTPClient_init(std::vector<const char *> wifiNetworks[]) {
  bool isConnected = false, result = false;
  for (size_t i = 0; i < wifiNetworks->size() && !isConnected; i++) {
    isConnected = NTPClient_Connect(wifiNetworks[i][0], wifiNetworks[i][1]);
  }

  if (!isConnected) {
    Logger::log(LogSeverity::WARNING, "No Network found\n", true);
  }

  if (isConnected) {
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;

      http.begin("https://worldtimeapi.org/api/ip");

      Logger::log(LogSeverity::DEBUG, "Requesting time from worldtimeapi.org... ", true);
      int httpCode = http.GET();
      
      if (httpCode > 0) {
        Logger::log(LogSeverity::DEBUG, "- HTTP Code: ");
        Logger::log(LogSeverity::DEBUG, std::to_string(httpCode).c_str());
        Logger::log(LogSeverity::DEBUG, "\n");

        if (httpCode == HTTP_CODE_OK) {
          String payload = http.getString();
          base_seconds = extractUnixtime((const char *)(payload.c_str()));
          char *timezone = getAbbreviationFromJSON((const char *)(payload.c_str()));
          base_seconds += timezone_offset(timezone);
          result = true;
        }
      } else {
        Logger::log(LogSeverity::ERROR, "\n");
        Logger::log(LogSeverity::ERROR, "Failed to connect to worldtimeapi.org\n", true);
      }

      http.end();
    }
  }

  delay(250);

  WiFi.disconnect();

  while (WiFi.status() == WL_CONNECTED) {
    Serial.println("disconnecting...");
    delay(100);
  }

  return result;
}

public unsigned long NTPClient_getTime() {
  unsigned long milliseconds = millis();
  unsigned long seconds = base_seconds + milliseconds / 1000;
  return seconds;
}

public char *NTPClient_getTimeStr(unsigned long seconds) {
  char *timeStr = (char *)malloc(11);
  sprintf(timeStr, "%02d:%02d:%02d", (seconds / 3600) % 24, (seconds / 60) % 60, seconds % 60);
  return timeStr;
}