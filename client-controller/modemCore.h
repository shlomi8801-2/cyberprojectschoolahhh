#pragma once
#include "client.h"

void changeModemPowerStart(byte state);
void rebootModem();
String SendAT(String str,unsigned long Timeoutms,SoftwareSerial* AT);
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