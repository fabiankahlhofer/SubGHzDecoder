#ifndef SUBGHZDECODER_NTP_NTPCLIENT_H_
#define SUBGHZDECODER_NTP_NTPCLIENT_H_

#include <WiFi.h>
#include <vector>

bool NTPClient_init(std::vector<const char *> wifiNetworks[]);
unsigned long NTPClient_getTime();
char *NTPClient_getTimeStr(unsigned long seconds);

#endif /* SUBGHZDECODER_NTP_NTPCLIENT_H_ */