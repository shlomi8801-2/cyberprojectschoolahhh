//my arduino chip is atmega328p
#include <avr/io.h>
#include <util/delay.h>
// #include "portpins.h"
// #include "iom328p.h"
void sleep(unsigned int ms){
    for (unsigned int i=0;i<ms;i++);
}
int main()
{
    // Set built-in LED pin as output
    DDRB |= (1 << DDB5); // just sets the pb register at bit 5 which is the led state to output for output
    DDRD = ~(0); //set pd2 to output  
      
    while (1) {
        PORTB |=  (1 << PB5);   // LED on
        // PORTD = 0b00000100;
        _delay_ms(1000);

        PORTB &= ~(1 << PB5);   // LED off
        // PORTD = 0;

        _delay_ms(1000);

    }
    return 0;
}