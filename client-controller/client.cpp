//my arduino chip is atmega328p
#include <avr/io.h>
#include <Arduino.h>
#ifdef pc 
//anything that has errors in intellesense put here
#include <util/delay.h>
// #include <Arduino.h>
#include <SoftwareSerial.h>
#include "atmega328p/ArduinoCore-avr/libraries/SoftwareSerial/src/SoftwareSerial.h"
// #include <eeprom.h> // used to use the arduino rom(as much as i know)
#else
//anything that the intellesense require but doesnt compile
#include "atmega328p/ArduinoCore-avr/cores/arduino/Stream.h"
#include "atmega328p/avr/include/util/delay.h.in"
#include "iom328p.h"
#include "atmega328p/avr/portpins.h"
#endif

inline void sleep(unsigned int ms){ // inline because _delay_ms throws error when its non inline function
    _delay_ms(ms);
}
void toggleLed(){
    PORTB ^= 1<<PORTB5;
}
int main()
{
    Serial.begin(115200);
    
    // Set built-in LED pin as output
    DDRB |= (1 << DDB5); // just sets the pb register at bit 5 which is the led state to output for output
    DDRD = ~(0); //set pd2 to output  
      
    while (1) {
        toggleLed();
        Serial.print("hello");
        // PORTB = 1<<PORT5;
        // PORTD = 0b00000100;
        sleep(500);
    }
    return 0;
}