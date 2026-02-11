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

// #include "avr/io.h"
// #include "atmega328p/includes/core/Arduino.h"
// #include "util/delay.h"
inline void sleep(unsigned int ms){ // inline because _delay_ms throws error when its non inline function
    _delay_ms(ms);
}
void toggleLed(){
    PORTB ^= 1<<PORTB5;
}
int main()
{
    Serial.begin(115200);
    Serial.println("hello");
    Serial.flush();
    // analogWrite(PB0,1);
    // Set built-in LED pin as output
    DDRB |= (1 << DDB5); // just sets the pb register at bit 5 which is the led state to output for output
    DDRD = ~(0); //set pd2 to output  
      sei(); // makes interrupts available(but says it might be faster to make the interrupts our self's)
    while (1) {
        toggleLed();
        if(Serial.available()>0){Serial.println(Serial.read());
        Serial.flush();}
        
        PORTB ^= 1<<PORT5;
        // PORTD = 0b00000100;
        sleep(0);
        
    }
    return 0;
}