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
    



    // for( byte x=0;x<psd_LENGTH;++x)
    //         EEPROM.write(psd_ADDR+x,(char)(97+x));
    // int a =0;
    // dbg(getPsdFromeMem(),1,psd_LENGTH);

    sei(); // start listening to interrupts
    DDRB |= 1 << PORTB5; // sets builtin led to output mode
    SoftwareSerial SerialAT(MODEM_RDX_PORT, MODEM_TDX_PORT); 
    
    initialModem(&SerialAT);
    //to make if else like to do a chain if a function gets true as output continue
    bool connected = false;
    if(conncectToSerevr())
    if(loginClient())
    connected = true;
    else{
    if(RegisterToServer()){
        if(!loginClient()){
            dbg(F("failed to login after registering"));
        
    }else {
        
        connected = true;
    }
    }else{
        dbg(F("failed to register to the server"));
    }
    }
    if (connected){
        dbg(F("connected and waiting!"));
    }
    fixATchar('0',0);
    startInteractiveConsoleWithModem(SerialAT);
    return 0;
}
