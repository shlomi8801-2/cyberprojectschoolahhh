#pragma once
#include <client.h>
#include <ATcommands.h>

//this file should handle all custom modem functions

void changeModemPowerStart(byte state);
void rebootModem();
char fixATchar(const char c,byte dataMode=2);

String SendAT(String str,unsigned long Timeoutms = 1000, SoftwareSerial *AT = nullptr);
String SendAT(const char* str, unsigned long Timeoutms=1000, SoftwareSerial *AT=nullptr);
byte* SendATchrArr(const char str ,unsigned long& size, unsigned long Timeoutms=1000, SoftwareSerial *AT=nullptr);
String SendATArr(const char* str,unsigned long size, unsigned long Timeoutms=1000, SoftwareSerial *AT=nullptr);
byte* GetATResponse(unsigned long& size,unsigned long Timeoutms=1000, SoftwareSerial *AT=nullptr);
byte* GetATResponseFixedSize(unsigned long size,unsigned long Timeoutms=1000, SoftwareSerial *AT=nullptr);
void SkipNATCharacters(int n,unsigned long Timeoutms=10000);
template <typename T> // support for strings and char arrays
String SendATHelper(const T str,unsigned long size, unsigned long Timeoutms=1000, SoftwareSerial *AT=nullptr);
void SetupModemSettings();
byte checkModemStatus();
byte waitForATResponse(unsigned int maxTimeout);
void initialModem(SoftwareSerial* AT);
void connectToServer();
void conncectToSerevr();
void closeConnectionToServer();

inline bool confirmDataSize(byte* data,size_t packageSize){
    
    size_t tmpSize = getSizeFromHeader(data);
    
    return tmpSize == packageSize;
}

void startInteractiveConsoleWithModem(SoftwareSerial &SerialAT);

void StartDataSend(size_t dataLength);
byte* StopDataSend();
byte* waitForServerResponse(unsigned long &size, unsigned long Timeoutms);
void skipUntilChar(char c,size_t Timeoutms=10000);
void clearATBuffer();