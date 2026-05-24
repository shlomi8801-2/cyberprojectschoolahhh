#pragma once
#include <client.h>
#include "ATcommands.h"


void changeModemPowerStart(byte state);
void rebootModem();
char fixATchar(const char c,byte dataMode=2);

String SendAT(String str,unsigned long Timeoutms = 1000, SoftwareSerial *AT = nullptr);
String SendAT(const char* str, unsigned long Timeoutms=1000, SoftwareSerial *AT=nullptr);
byte* SendAT(const char str ,unsigned long& size, unsigned long Timeoutms=1000, SoftwareSerial *AT=nullptr);
String SendATArr(const char* str,unsigned long size, unsigned long Timeoutms=1000, SoftwareSerial *AT=nullptr);
byte* GetATResponse(unsigned long& size,unsigned long Timeoutms=1000, SoftwareSerial *AT=nullptr);
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
inline size_t getSizeFromHeader(byte* data){
    //getting the value of the number from the "header"(couple of bytes at the start of data)
    for (byte i=0;i<HEADER_SIZE_BYTES- sizeof(size_t);++i){
        //if the data header is bigger then the size of the packageSize then the first bytes must be 0
        if (data[i]!=0) return -1;
    }
    size_t tmpSize = 0;
    for (byte i=HEADER_SIZE_BYTES-sizeof(size_t);i<HEADER_SIZE_BYTES;++i){//the lengths of the data are represented as big endian instead of this code compiling as little endian
            // dbg(data[i]);
            tmpSize <<= 8;
            tmpSize +=data[i];
    }
    return tmpSize;
}
inline bool confirmDataSize(byte* data,size_t packageSize){
    
    size_t tmpSize = getSizeFromHeader(data);
    
    return tmpSize == packageSize;
}
void startInteractiveConsoleWithModem(SoftwareSerial &SerialAT);

void StartDataSend(size_t dataLength);
byte* StopDataSend();