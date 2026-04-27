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


void startInteractiveConsoleWithModem(SoftwareSerial &SerialAT);