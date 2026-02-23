// my arduino chip is atmega328p
#ifndef pc
// #define mmcu atmega328p //must include those in the compiler line but keep it here for the intellisense
#define __AVR_ATmega328P__
// #define F_CPU 16000000UL
#endif
#include <avr/io.h>
// #include <avr/iom328p.h>
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <util/delay.h>
// #include <eeprom.h> // used to use the arduino rom(as much as i know)
#include "ATcommands.h"
#define SerialMon Serial
typedef unsigned char byte; 

inline void sleep(unsigned int ms)
{ // inline because _delay_ms throws error when its non inline function
    _delay_ms(ms);
}
void toggleLed()
{
    PORTB ^= 1 << PORTB5;
}
char fixATchar(char c){
if (c & 1<<7)
            c ^= 11<<6;
            return c;
}
inline void printAsBin(byte a){
    // for (byte i = 0;i<8;i++){
    //     Serial.print(a/(1<<(7-i)) == 1);
    //     a = a<< 1;
    // }
    Serial.print((byte)a);
    Serial.print("\n");
}
String SendAT(String str,byte Timeout=1000,SoftwareSerial* AT=nullptr){
    //sends a string to the AT serial and then returns the reponse
    static SoftwareSerial* _AT;
    if (AT)
        _AT = AT;
    _AT->println(str);
    _AT->flush();
    while (Timeout >0 && _AT->available()<=0){
        sleep(10);
        Timeout -=10;
    }
    
    if (Timeout <=0 && _AT->available()<=0){
        return "NO RESPONSE";
    }
        
    
    String output = "";
    while (_AT->available())
    {
        char c = _AT->read();
        c = fixATchar(c);
        output += c;
    }
    
    return output;
}
void initialModem(SoftwareSerial* AT){
    sleep(100);
}
SoftwareSerial SerialAT(2, 3);
int main()
{
    static uint32_t rates[] = {115200, 57600,  38400, 19200, 9600,  74400, 74880,
                             230400, 460800, 2400,  4800,  14400, 28800};
    sei();
    DDRB |= 1 << PORTB5;
    Serial.begin(115200);
    Serial.print("using speed:");
    Serial.println(ATCONSOLESPEED);
    Serial.flush();
    SerialAT.begin(115200);

    // Access AT commands from Serial Monitor
    SerialMon.println(
        F("***********************************************************"));
    SerialMon.println(F(" You can now send AT commands"));
    SerialMon.println(
        F(" Enter \"AT\" (without quotes), and you should see \"OK\""));
    SerialMon.println(
        F(" If it doesn't work, select \"Both NL & CR\" in Serial Monitor"));
    SerialMon.println(
        F("***********************************************************"));
    // SerialAT.print(SETSERIALSPEEDCMD);
    // SerialAT.print("=");
    // SerialAT.println(ATCONSOLESPEED);
    // SerialAT.flush();
    sleep(100);
    Serial.println(SendAT("AT",10,&SerialAT));
    while (1)
    {
        
        //     if (SerialAT.available()){
        //     SerialMon.write(SerialAT.read());
        //     SerialMon.flush();
        //     }
        // }
        while (SerialMon.available())
        {
            char c = SerialMon.read();
            Serial.println(SendAT("AT"));
        }
            
        
        if (SerialMon.available()<=0 && SerialAT.available() <=0){
            Serial.flush();
            SerialAT.flush();

        }
        
        
//this for some reason it adds 10 instead of 1 at the end of a message recieved
// 129
// 148 10010100
// 13  00001101
// 13
// 10
// 143 10001111
// 139 10001011
// 13
// 10

//arduino ide - "OK"
//65 01000001
// 84 1010100
// 13
// 13
// 10
// 79 1001111
// 75 1001011
// 13
// 10


// 129 10000001
// 172 1010110000110101
// 53  
// 13
// 18
// 61
// 75
// 29
// 10
    }
    return 0;
}