// my arduino chip is atmega328p
#define __AVR_ATmega328P__
#include <avr/io.h>
// #include <avr/iom328p.h>
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <util/delay.h>
// #include <eeprom.h> // used to use the arduino rom(as much as i know)
#include "ATcommands.h"
#include "client.h"


byte dataMode = 0;
char fixATchar(char c){
    if (dataMode) return c;
if (c & 1<<7)
            c ^= 11<<6;
else c &= 63; // 0011 1111
            return c;
}

String SendAT(String str,byte Timeout=1000,SoftwareSerial* AT=nullptr){
    //sends a string to the AT serial and then returns the reponse
    static SoftwareSerial* _AT;
    if (AT){
        _AT = AT;
        _AT->begin(ATCONSOLESPEED);
    }
    else if (!_AT)
        return "NO AT SERIAL OBJECT";
    dbg(">> "+str);
        _AT->println(str);
    _AT->flush();
    
    while (Timeout >0 && _AT->available()<=0){
        sleep(10);
        Timeout -=10;
    }
    if (Timeout <=0 && _AT->available()<=0){
        return "NO RESPONSE";
    }
    
    String output = "";
    while (_AT->available())
    {
        char c = _AT->read();
        c = fixATchar(c);
        output += c;
    }
    
    return output;
}
byte checkModemStatus(){
    const String res = SendAT(GETCURRSTATUSCMD);
    static const char* const modemStatues[] ={"IP INITIAL","IP START","IP CONFIG","IP GPSACT","IP STATUS"," CONNECTING","SERVER LISTENING","CONNECT OK"," CLOSING"," CLOSED","PDP DEACT","IP PROCESSING"};
    // const char * currStatus = nullptr; // const data in pointer but not the pointer
    byte currStatus = ~0;
    for (int i=0;i< sizeof(modemStatues)/sizeof(modemStatues[0]);++i ){
        if (res.indexOf(modemStatues[i]) !=-1){
            return i;
        }
    }
    dbg(res +" status not defined");
    return -1;
    
    // -1/255 status not defined
    // 0 IP INITIAL
    // 1 IP START
    // 2 IP CONFIG
    // 3 IP GPRSACT
    // 4 IP STATU
    // 5 TCP CONNECTING/UDP CONNECTING
    // 6 SERVER LISTENING
    // 7 CONNECT OK
    // 8 TCP CLOSING/UDP CLOSING
    // 9 TCP CLOSED/UDP CLOSED
    // 10 PDP DEACT
    // 11 IP PROCESSING
}
byte waitForATResponse(unsigned short maxTimeout,SoftwareSerial* Sim){ // maxTimout in seconds max 650 secconds
    //execute AT until response - not setting the serial object in SendAT function
    maxTimeout *=100;
    while (maxTimeout>0)
    {
        Sim->println("AT");
        Sim->flush();
        sleep(50);
        maxTimeout -=5; // maxtimeout is seconds times 100 so -5 means -50ms
        if (Sim->available()){
            while (Sim->available()){ // clear the buffer
                Sim->read();
            }
            sleep(50);//instead of checking if the response is "OK"
            return 1;
            break;
        }
    }
    //maxTimoue reached <=0
    return 0; // no response
    
    
}
void initialModem(SoftwareSerial* AT){
    if(!waitForATResponse(5,AT)){
        dbg("module not responding");
        return;
    }
    byte status = checkModemStatus();
    String res =SendAT((String)SETAPNCMD+"="+APNNAME,1000,AT);
    // dbg(res);
    byte tries = 10;
    status = checkModemStatus();
    while (status !=1 && --tries >0){
        //if the modem is not on ip start mode
        //then restart the modem
        //ping it - TODO
        rebootModem();
        SendAT((String)SETAPNCMD+"="+APNNAME);
        status = checkModemStatus();
    }
    SendAT(BRINGUPWIRELESSCONNECTIONGPRS);
    status = checkModemStatus();
    if (status == 4){ // IP STATUS means connected
        dbg("local ip is:"+SendAT(GETLOCALIPADDRESSCMD));
    }else {
        dbg("status is not 4\nstatus:"+status);
    }
    
    
    
    
    //Serial.println(SendAT(BRINGUPWIRELESSCONNECTIONGPRS));
}
void rebootModem(){
    SendAT(REBOOTMODEMCMD,30*1000);
    
}
void startInteractiveConsoleWithModem(SoftwareSerial& SerialAT){
    Serial.println(
        F("***********************************************************"));
    Serial.println(F(" You can now send AT commands"));
    Serial.println(
        F(" Enter \"AT\" (without quotes), and you should see \"OK\""));
    Serial.println(
        F(" If it doesn't work, select \"Both NL & CR\" in Serial Monitor"));
    Serial.println(
        F("***********************************************************"));
while (1)
    {
        
        
        while (Serial.available())
        {
            char c = Serial.read();
            SerialAT.write(c);
            Serial.write(c);
        }
        if (SerialAT.available()){
            
        while (SerialAT.available())
        {
            byte c = SerialAT.read();
            Serial.write(fixATchar(c));
            
            // Serial.println((byte)fixATchar(c));
        }}
        if (Serial.available()<=0 && SerialAT.available() <=0){
            Serial.flush();
            SerialAT.flush();

        }
    }
}
SoftwareSerial SerialAT(2, 3);
int main()
{
    sei(); // start listening to interrupts
    DDRB |= 1 << PORTB5;
    Serial.begin(115200);
    Serial.print("using speed:");
    Serial.println(ATCONSOLESPEED);
    Serial.flush();

    // Access AT commands from Serial Monitor
    
    // SerialAT.print(SETSERIALSPEEDCMD);
    // SerialAT.print("=");
    // SerialAT.println(ATCONSOLESPEED);
    // SerialAT.flush();
    sleep(100);
    // Serial.println(SendAT("AT",10,&SerialAT));
    initialModem(&SerialAT);
startInteractiveConsoleWithModem(SerialAT);
    
    return 0;
}