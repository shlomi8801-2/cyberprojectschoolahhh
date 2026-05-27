// my arduino chip is atmega328p
#define __AVR_ATmega328P__
#include <avr/io.h>
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <util/delay.h>
#include <ATcommands.h>
#include "client.h"
#include "modemCore.h"
#include "actionsService.h"
#define DEBUG

#include <Hashtable.h>

int main()
{
    Serial.begin(115200);
    dbg("program started!");
    




    


    sei(); // start listening to interrupts
    DDRB |= 1 << PORTB5; // sets builtin led to output mode
    SoftwareSerial SerialAT(MODEM_RDX_PORT, MODEM_TDX_PORT); 
    
    initialModem(&SerialAT);
    //to make if else like to do a chain if a function gets true as output continue
    
    if(conncectToSerevr() && (loginClient() || (RegisterToServer() && loginClient()))){
        dbg(F("connected and waiting!"));
    }else{
        dbg(F("failed to activate service"));
    }
    fixATchar('0',0);
    startInteractiveConsoleWithModem(SerialAT);
    return 0;
}
