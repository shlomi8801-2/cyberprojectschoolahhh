#pragma once
#include <client.h>
#include "ATcommands.h"

void changeModemPowerStart(byte state);
void rebootModem();

String SendAT(String str,unsigned long Timeoutms = 1000, SoftwareSerial *AT = nullptr);
String SendAT(const char* str, unsigned long Timeoutms=1000, SoftwareSerial *AT=nullptr);
byte* SendAT(const char str ,unsigned long& size, unsigned long Timeoutms=1000, SoftwareSerial *AT=nullptr);
String SendATArr(const char* str,unsigned long size, unsigned long Timeoutms=1000, SoftwareSerial *AT=nullptr);
template <typename T> // support for strings and char arrays
String SendATHelper(const T str,unsigned long size, unsigned long Timeoutms=1000, SoftwareSerial *AT=nullptr);
byte checkModemStatus();
byte waitForATResponse(unsigned int maxTimeout);
void initialModem(SoftwareSerial* AT);
void connectToServer();
void conncectToSerevr();
static byte dataMode=0;
constexpr char fixATchar(char c)
{
    if (dataMode)
        return c;
    if (c & 1 << 7)
        c ^= 11 << 6;
    else
        c &= 63; // 0011 1111
    return c;
}
void startInteractiveConsoleWithModem(SoftwareSerial &SerialAT);