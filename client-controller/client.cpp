//my arduino chip is atmega328p
#define mmcu atmega328
#define __AVR_ATmega328P__
#define F_CPU 16000000UL
#include <avr/io.h>
#include <Arduino.h>
// #include <SoftwareSerial.h>
#include <util/delay.h>
// #include <eeprom.h> // used to use the arduino rom(as much as i know)

inline void sleep(unsigned int ms){ // inline because _delay_ms throws error when its non inline function
    _delay_ms(ms);
}
void toggleLed(){
    PORTB ^= 1<<PORTB5;
}
int main()
{
    #ifdef UBRRH
    gggg
    #endif
    // Serial.begin(115200);
    
    // Set built-in LED pin as output
    DDRB |= (1 << DDB5); // just sets the pb register at bit 5 which is the led state to output for output
    DDRD = ~(0); //set pd2 to output  
      
    while (1) {
        toggleLed();
        // Serial.print("hello");
        // PORTB = 1<<PORT5;
        // PORTD = 0b00000100;
        sleep(500);
    }
    return 0;
}