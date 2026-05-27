#pragma once
// typedef unsigned char byte; //already defined
#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__ // required for intellesense to work because avr/io.h adds the current module
#endif
#include <avr/io.h>
#include <Arduino.h>
#include <util/delay.h>
// #include <eeprom.h> // used to use the arduino rom(as much as i know)
// #include <ATcommands.h>
#define debug
#include <SoftwareSerial.h>

#include <ATcommands.h>



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
inline void dbg(T str,bool nl=1){
    #ifdef debug
    if( nl)
    Serial.println(str);
    else
    Serial.print(str);
    Serial.flush();
    #endif
}
// template <class T>
// inline void dbg(T str,bool nl,size_t length){
//     #ifdef debug
    
//     for (size_t i=0;i<length;++i)
//     Serial.print(str[i]);
//     if( nl)
//     Serial.write("\n");
//     Serial.flush();
//     #endif
// }
inline String ShowBytes(byte* arr,unsigned int size=0){
    String output="";
    size = size==0? strlen((char*)arr):size;
    for(unsigned int i=0;i<size;i++){
        output += (int)arr[i]+" ";
        dbg((int)arr[i]);
    }
    return output;
}
inline void* reallocSafe(void *__ptr, size_t __size){// if fails to allocate new memory free the last one so you dont lose the pointer
    void* tmp = realloc(__ptr,__size);
    if(tmp==nullptr)
        free(__ptr);
    return tmp;
}
inline void printArr(void* data,size_t length){
    for (int i=0;i<length;i++){
         dbg(((byte*)data)[i],0);
         dbg("    ",0);
        dbg(((char*)data)[i]);
    }
}

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

inline char* getAvailablePins(byte &n){
    static char arr[] = {AVAILABLE_PINS};
    constexpr byte len = 13;// for now
    return arr;
}
inline void checkMemory(size_t n){
    void* tmp = malloc(n);
    if(tmp == nullptr){
        dbg("out of memory");
        return;
    }
    dbg("has memory");
}