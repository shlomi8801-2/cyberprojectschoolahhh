//my arduino chip is atmega328p
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
inline void sleep(unsigned int ms){ // inline because _delay_ms throws error when its non inline function
    _delay_ms(ms);
}
void toggleLed(){
    PORTB ^= 1<<PORTB5;
}
SoftwareSerial sim(2,3);
int main()
{
    Serial.begin(115200);
    Serial.println("hello");
    Serial.flush();
    sim.begin(ATCONSOLESPEED);
    // analogWrite(PB0,1);
    // Set built-in LED pin as output
    DDRB |= (1 << DDB5); // just sets the pb register at bit 5 which is the led state to output for output
    DDRD = ~(0); //set pd2 to output  
    sei(); // makes interrupts available(but says it might be faster to make the interrupts our self's)
    
    
    
       Serial.println(
      F("***********************************************************"));
  Serial.println(F(" You can now send AT commands"));
  Serial.println(
      F(" Enter \"AT\" (without quotes), and you should see \"OK\""));
  Serial.println(
      F(" If it doesn't work, select \"Both NL & CR\" in Serial Monitor"));
  Serial.println(
      F("***********************************************************"));

  while (true) {
    // if (Serial.available()) { sim.write(Serial.read()); Serial.write("ye");}
    // if (sim.available()) { Serial.write(sim.read()); }
    sleep(100);
    PORTD ^= 1<<PORT5;
  }
    return 0;
}