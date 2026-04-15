// my arduino chip is atmega328p
#define __AVR_ATmega328P__
#include <avr/io.h>
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <util/delay.h>
#include "ATcommands.h"
#include "client.h"
#include "modemCore.h"
#include "actionsService.h"
#define DEBUG

#include <Hashtable.h>

int main()
{
    Serial.begin(115200);
    dbg("program started!");
    Hashtable test;
    test["hello"] = "REG";
    test["Sega"]="sonic";
    test["a"]="b";
    
    byte* data =buildData(test);
    dbg((String)"size=");
    dbg(*(long*)data);
    // for(int i=0;i<8;i++){
    //     dbg(i);
    //     dbg((byte)data[i]);
        
    // }
    dbg((unsigned int)data[4]);
    
    // test = parseData(data+HEADER_SIZE_BYTES,*(unsigned long*)data);
    // for (auto i:test){
    //     dbg(i.key);
    //     dbg(i.value);
    // }
    dbg("done");
    







    // sei(); // start listening to interrupts
    // DDRB |= 1 << PORTB5; // sets builtin led to output mode
    // SoftwareSerial SerialAT(MODEM_RDX_PORT, MODEM_TDX_PORT); 
    // initialModem(&SerialAT);
    // conncectToSerevr();
    // //here after you achive a connection in single ip mode(default) you either send data or just in a loop waiting for data to be sent(data is pushed without warning)
    // RegisterToServer();
    // startInteractiveConsoleWithModem(SerialAT);
    return 0;
}