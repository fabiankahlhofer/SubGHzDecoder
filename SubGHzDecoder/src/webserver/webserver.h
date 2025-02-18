#ifndef ___LEO_WEBSERVER_H
#define ___LEO_WEBSERVER_H

#include <WiFi.h>
#include "queue/Queue.h"
#include <list>

void Webserver_handleClient(Queue<String> *sendDataQueue_443MHz,
                            Queue<String> *sendDataQueue_IR,
                            std::list<String> *receivedDataList_443MHz,
                            std::list<String> *receivedDataList_IR,
                            std::list<String> *receivedDataDateList_443MHz,
                            std::list<String> *receivedDataDateList_IR);
void Webserver_init(char *ssid, char *password);
void Webserver_addReceivedValue(String json);
String Webserver_getReceivedValues();
String Webserver_shortenJSON(String json);
void Webserver_appendToListWithTime(std::list<String> *list, std::list<String> *dateList, String value);

#endif /* ___WEBSERVER_H */