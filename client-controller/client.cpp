// my arduino chip is atmega328p
#define __AVR_ATmega328P__
#include <avr/io.h>
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <util/delay.h>
#include "ATcommands.h"
#include "client.h"
#include "modemCore.h"



#include <Hashtable.h>
int main()
{
    Hashtable<String,String> test;
    test.put("a","b");
    for(auto i:test){
        dbg(i.key);
        dbg(i.value);

    }




    sei(); // start listening to interrupts
    DDRB |= 1 << PORTB5; // sets builtin led to output mode
    Serial.begin(115200);
    SoftwareSerial SerialAT(MODEM_RDX_PORT, MODEM_TDX_PORT); 
    initialModem(&SerialAT);
    conncectToSerevr();
    //here after you achive a connection in single ip mode(default) you either send data or just in a loop waiting for data to be sent(data is pushed without warning)
    
    startInteractiveConsoleWithModem(SerialAT);

    return 0;
}