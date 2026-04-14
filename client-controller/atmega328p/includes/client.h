#pragma once
// typedef unsigned char byte; //already defined
#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__ // required for intellesense to work because avr/io.h adds the current module
#endif
#include <avr/io.h>
#include <Arduino.h>
#include <util/delay.h>
// #include <eeprom.h> // used to use the arduino rom(as much as i know)
// #include "ATcommands.h"
#define debug
#include <SoftwareSerial.h>




inline void(* resetFunc) (void) = 0; //declare reset function @ address 0 - jumps to address 0 in the program written to board
inline void toggleLed()
{
    PORTB ^= 1 << PORTB5;
}
inline void stopProgram(){
    toggleLed();
    exit(0);
}
inline void sleep(unsigned int ms)
{ // inline because _delay_ms throws error when its non inline function
    _delay_ms(ms);
}

template <class T>
inline void dbg(T str){
    #ifdef debug
    Serial.println(str);
    Serial.flush();
    #endif
}
inline String ShowBytes(byte* arr,unsigned int size=0){
    String output="";
    size = size==0? strlen((char*)arr):size;
    for(unsigned int i=0;i<size;i++){
        output += (int)arr[i]+" ";
        dbg((int)arr[i]);
    }
    return output;
}