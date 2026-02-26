// my arduino chip is atmega328p
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
#define SerialMon Serial
typedef unsigned char byte; 

//TODO move those to header file
void changeModemPowerStart(byte state);
void rebootModem(SoftwareSerial& Sim);


inline void sleep(unsigned int ms)
{ // inline because _delay_ms throws error when its non inline function
    _delay_ms(ms);
}
void toggleLed()
{
    PORTB ^= 1 << PORTB5;
}
char fixATchar(char c){
if (c & 1<<7)
            c ^= 11<<6;
else c &= 63; // 0011 1111
            return c;
}
template <class T>
void dbg(T str){
    //print to serial console
    Serial.println(str);
    Serial.flush();
}
String SendAT(String str,byte Timeout=1000,SoftwareSerial* AT=nullptr){
    //sends a string to the AT serial and then returns the reponse
    static SoftwareSerial* _AT;
    if (AT)
        _AT = AT;
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
byte waitForResponse(unsigned short maxTimeout,SoftwareSerial* Sim){ // maxTimout in seconds max 650 secconds
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
    AT->begin(ATCONSOLESPEED);
    sleep(100);
    if(!waitForResponse(5,AT)){
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
        rebootModem(*AT);
        SendAT((String)SETAPNCMD+"="+APNNAME);
        sleep(100);
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
void changeModemPowerStart(byte state){ // used reset command instead
    //https://deepbluembedded.com/arduino-port-manipulation-registers-example/
    DDRB |= 1<< MODEMPOWERPIN; //analog direction
    if (state){
        PORTB |=1<<MODEMPOWERPIN;
    }else{
        PORTB &= ~(1<<MODEMPOWERPIN);
    }
    // DDRC |= ~0;
    // PORTC |= ~0;
}
void rebootModem(SoftwareSerial& Sim){
    Sim.println(REBOOTMODEMCMD);
    Sim.flush();
    waitForResponse(-1,&Sim);
    
}
void startInteractiveConsoleWithModem(SoftwareSerial& SerialAT){
    SerialMon.println(
        F("***********************************************************"));
    SerialMon.println(F(" You can now send AT commands"));
    SerialMon.println(
        F(" Enter \"AT\" (without quotes), and you should see \"OK\""));
    SerialMon.println(
        F(" If it doesn't work, select \"Both NL & CR\" in Serial Monitor"));
    SerialMon.println(
        F("***********************************************************"));
while (1)
    {
        
        
        while (SerialMon.available())
        {
            char c = SerialMon.read();
            SerialAT.write(c);
            SerialMon.write(c);
        }
        if (SerialAT.available()){
            
        while (SerialAT.available())
        {
            byte c = SerialAT.read();
            SerialMon.write(fixATchar(c));
            
            // SerialMon.println((byte)fixATchar(c));
        }}
        if (SerialMon.available()<=0 && SerialAT.available() <=0){
            Serial.flush();
            SerialAT.flush();

        }
    }
}
SoftwareSerial SerialAT(2, 3);
int main()
{
    // changeModemPowerStart(1);
    static uint32_t rates[] = {115200, 57600,  38400, 19200, 9600,  74400, 74880,
                             230400, 460800, 2400,  4800,  14400, 28800};
    sei();
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