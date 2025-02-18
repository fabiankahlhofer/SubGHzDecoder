#include "utils.h"

#include <cJSON.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>

char* binaryToHex(const char* binary) {
  int len = strlen(binary);
  if (len % 4 != 0) {
    printf("Binary length must be a multiple of 4.\n");
    return NULL;
  }

  int hexLen = len / 4;
  char* hex = (char*)malloc(hexLen + 1);
  if (hex == NULL) {
    printf("Memory allocation failed.\n");
    return NULL;
  }
  hex[hexLen] = '\0';

  for (int i = 0; i < hexLen; ++i) {
    int value = 0;
    for (int j = 0; j < 4; ++j) {
      int index = i * 4 + j;
      value = value * 2 + (binary[index] - '0');
    }
    if (value <= 9)
      hex[i] = '0' + value;
    else
      hex[i] = 'A' + (value - 10);
  }

  return hex;
}

const char* bin2tristate(const char* bin) {
  static char returnValue[50];
  int pos = 0;
  int pos2 = 0;
  while (bin[pos]!='\0' && bin[pos+1]!='\0') {
    if (bin[pos]=='0' && bin[pos+1]=='0') {
      returnValue[pos2] = '0';
    } else if (bin[pos]=='1' && bin[pos+1]=='1') {
      returnValue[pos2] = '1';
    } else if (bin[pos]=='0' && bin[pos+1]=='1') {
      returnValue[pos2] = 'F';
    } else {
      return "not applicable";
    }
    pos = pos+2;
    pos2++;
  }
  returnValue[pos2] = '\0';
  return returnValue;
}

char* dec2binWzerofill(unsigned long Dec, unsigned int bitLength) {
  static char bin[64]; 
  unsigned int i=0;

  while (Dec > 0) {
    bin[32+i++] = ((Dec & 1) > 0) ? '1' : '0';
    Dec = Dec >> 1;
  }

  for (unsigned int j = 0; j< bitLength; j++) {
    if (j >= bitLength - i) {
      bin[j] = bin[ 31 + i - (j - (bitLength - i)) ];
    } else {
      bin[j] = '0';
    }
  }
  bin[bitLength] = '\0';
  
  return bin;
}

char** split(const char* str, char delimiter, int& count) {
  const char* ptr = str;
  int numTokens = 1; // at least one token
  int len = strlen(str);
  
  // Count the number of tokens
  for (int i = 0; i < len; i++) {
    if (str[i] == delimiter) {
      numTokens++;
    }
  }

  char** tokens = new char*[numTokens];
  int tokenIndex = 0;
  int tokenLength = 0;

  for (int i = 0; i <= len; i++) {
    if (str[i] == delimiter || str[i] == '\0') {
      tokens[tokenIndex] = new char[tokenLength + 1];
      strncpy(tokens[tokenIndex], ptr, tokenLength);
      tokens[tokenIndex][tokenLength] = '\0';
      tokenIndex++;
      ptr += tokenLength + 1; // Move pointer to the next token
      tokenLength = 0; // Reset token length
    } else {
      tokenLength++;
    }
  }

  count = numTokens;
  return tokens;
}

void freeTokens(char** tokens, int count) {
  for (int i = 0; i < count; i++) {
    delete[] tokens[i];
  }
  delete[] tokens;
}