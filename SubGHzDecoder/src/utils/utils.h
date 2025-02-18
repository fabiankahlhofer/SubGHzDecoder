#ifndef ___CONVERTIONS_H___
#define ___CONVERTIONS_H___

#include <vector>

char* binaryToHex(const char* binary);
const char* bin2tristate(const char* bin);
char* dec2binWzerofill(unsigned long Dec, unsigned int bitLength);
char** split(const char* str, char delimiter, int& count);

#endif /* ___CONVERTIONS_H___ */