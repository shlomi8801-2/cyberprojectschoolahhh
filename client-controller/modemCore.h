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

inline bool confirmDataSize(byte* data,size_t packageSize){
    dbg(packageSize);
    for (byte i=0;i<HEADER_SIZE_BYTES- sizeof(size_t);++i){
        //if the data header is bigger then the size of the packageSize then the first bytes must be 0
        if (data[i]!=0) return false;
    }
    for (byte i=HEADER_SIZE_BYTES-sizeof(size_t);i<HEADER_SIZE_BYTES&&i<packageSize;++i){
        dbg("compairing ",0);
        dbg(i,0);
        dbg(" byte:",0);
        dbg(data[i],0);
        dbg(" -> ",0);
        dbg(((byte*)(&packageSize))[i]);
        if (data[i] != ((byte*)(&packageSize))[i])
            return false;
    }
    return true;
}
void startInteractiveConsoleWithModem(SoftwareSerial &SerialAT);