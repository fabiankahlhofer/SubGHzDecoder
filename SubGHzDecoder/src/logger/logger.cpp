#include "logger/logger.h"

#include <Arduino.h>
#include <string.h>
#include "ntp/NTPClient.h"

void printColour(const char *text, int r, int g, int b) {
  // char escapeCode[20];
  // snprintf(escapeCode, sizeof(escapeCode), "\033[38;2;%d;%d;%dm", r, g, b);

  // Serial.print(escapeCode);
  // Serial.print(text);
  
  // Serial.print("\033[0m");
  Serial.print(text);
}

void printColour(const char character, int r, int g, int b) {
  char escapeCode[20];
  snprintf(escapeCode, sizeof(escapeCode), "\033[38;2;%d;%d;%dm", r, g, b);

  Serial.print(escapeCode);
  Serial.print(character);
  
  Serial.print("\033[0m");
}

void Logger::log(LogSeverity severity, const char *message, bool isFirst) {
  if (isFirst) {
    char *time = NTPClient_getTimeStr(NTPClient_getTime());
    printColour(time, 200, 200, 200);
    Serial.print(" ");
  }
  switch (severity) {
    case LogSeverity::DEBUG:
      if (isFirst) { printColour("[DEBUG] ", 176, 228, 252); }
      printColour(message, 176, 228, 252);
      break;
    case LogSeverity::INFO:
      if (isFirst) { printColour("[INFO] ", 136, 234, 147); }
      printColour(message, 136, 234, 147);
      break;
    case LogSeverity::WARNING:
      if (isFirst) { printColour("[WARNING] ", 250, 238, 165); }
      printColour(message, 250, 238, 165);
      break;
    case LogSeverity::ERROR:
      if (isFirst) { printColour("[WARNING] ", 250, 238, 165); }
      printColour(message, 225, 134, 140);
      break;
  }
}