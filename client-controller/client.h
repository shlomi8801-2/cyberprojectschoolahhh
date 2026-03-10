#pragma once
// typedef unsigned char byte; //already defined
#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__ // required for intellesense to work because avr/io.h adds the current module
#endif
#include <avr/io.h>
// #include <avr/iom328p.h>
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <util/delay.h>
// #include <eeprom.h> // used to use the arduino rom(as much as i know)
#include "ATcommands.h"



void changeModemPowerStart(byte state);
void rebootModem();
String SendAT(String str,byte Timeout,SoftwareSerial* AT);
byte checkModemStatus();
byte waitForATResponse(unsigned short maxTimeout,SoftwareSerial* Sim);
void initialModem(SoftwareSerial* AT);


inline void sleep(unsigned int ms)
{ // inline because _delay_ms throws error when its non inline function
    _delay_ms(ms);
}
void toggleLed()
{
    PORTB ^= 1 << PORTB5;
}
template <class T>
void dbg(T str){
    //print to serial console
    Serial.println(str);
    Serial.flush();
}