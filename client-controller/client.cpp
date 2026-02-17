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
inline void sleep(unsigned int ms)
{ // inline because _delay_ms throws error when its non inline function
    _delay_ms(ms);
}
void toggleLed()
{
    PORTB ^= 1 << PORTB5;
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
    SerialAT.print(SETSERIALSPEEDCMD);
    SerialAT.print("=");
    SerialAT.println(ATCONSOLESPEED);
    SerialAT.flush();
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
            SerialAT.write(c);
            SerialMon.write(c);
            toggleLed();
        }
        if (SerialAT.available()){
            SerialAT.read();
        while (SerialAT.available())
        {
            
            SerialMon.write(SerialAT.read());
        }}
        if (SerialMon.available()<=0 && SerialAT.available() <=0){
            Serial.flush();
            SerialAT.flush();

        }//12914813132261 751310 
        // sleep(0); // 658413131079 751310

// 129
// 148
// 13
// 13
// 26
// 234
// 86
// 136
// 240

// 129
// 148
// 13
// 13
// 26
// 234
// 86
// 136
// 240
    }
    return 0;
}