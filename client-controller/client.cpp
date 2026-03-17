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
char fixATchar(char c)
{
    if (dataMode)
        return c;
    if (c & 1 << 7)
        c ^= 11 << 6;
    else
        c &= 63; // 0011 1111
    return c;
}

String SendAT(String str, int Timeout = 1000, SoftwareSerial *AT = nullptr)
{
    // sends a string to the AT serial and then returns the reponse
    static SoftwareSerial *_AT;
    if (AT)
    {
        _AT = AT;
        _AT->begin(ATCONSOLESPEED);
        dbg("using speed:");
        dbg(ATCONSOLESPEED);
    }
    else if (!_AT)
        return "NO AT SERIAL OBJECT";
    dbg(">> " + str);
    _AT->println(str);
    _AT->flush();

    while (Timeout > 0 && _AT->available() <= 0)
    {
        sleep(10);
        Timeout -= 10;
    }
    if (Timeout <= 0 && _AT->available() <= 0)
    {
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
byte checkModemStatus()
{
    byte tries = 5;
    while (--tries > 0)
    {

        const String res = SendAT(GETCURRSTATUSCMD);
        static const char *const modemStatues[] = {"IP INITIAL", "IP START", "IP CONFIG", "IP GPRSACT", "IP STATUS", " CONNECTING", "SERVER LISTENING", "CONNECT OK", " CLOSING", " CLOSED", "PDP DEACT", "IP PROCESSING"};
        // const char * currStatus = nullptr; // const data in pointer but not the pointer
        byte currStatus = ~0;
        for (int i = 0; i < sizeof(modemStatues) / sizeof(modemStatues[0]); ++i)
        {
            if (res.indexOf(modemStatues[i]) != -1)
            {
                return i;
            }
        }
        dbg((String)res + " status not defined");
        sleep(1000);
    }

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
byte waitForATResponse(unsigned int maxTimeout)
{ // maxTimout in seconds max 650 secconds
    // execute AT until response - not setting the serial object in SendAT function
    maxTimeout *= 100;
    while (maxTimeout > 0)
    {
        String res = SendAT("AT", 1000);
        sleep(1000);
        maxTimeout -= 100; // maxtimeout is seconds times 100 so -5 means -50ms
        if (res.indexOf("OK") != -1)
        {
            return 1;
        }
    }
    // maxTimoue reached <=0
    return 0; // no response
}
void initialModem(SoftwareSerial *AT)
{
    // should bring the modem from any status to 3 which is IP GPRSACT
    while (true)
    {
        if (!waitForATResponse(5))
        {
            dbg("module not responding");
            return;
        }
        byte status = checkModemStatus();
        if (status == 3)
            break;
        // get status code 1 first
        SendAT((String)SETAPNCMD + "=" + APNNAME);
        status = checkModemStatus();
        if (status != 1)
        {
            dbg("status is:" + (String)status);
            dbg("Rebooting might take some time");
            rebootModem();
            while (!waitForATResponse(10))
            { // while not responding
                sleep(3000);
            }
            continue; // re run this block
        }
        dbg("status found IP START");
        // try to get to status code 3
        dbg("trying to use mobile data");
        SendAT(BRINGUPWIRELESSCONNECTIONGPRS, 65000);
        status = checkModemStatus();
        dbg("status is:" + (String)status);
        // finally
        if (status == 3)
        { // 3 IP GPRSACT means connected
            dbg("local ip is:" + SendAT(GETLOCALIPADDRESSCMD));
            break;
        }
        else if (status == 10)
        {
            dbg("yes its dact");
            continue;
        }
        else
        {
            continue;
        }
    }
    dbg("modem initionlized!");
}
inline void rebootModem()
{
    SendAT(REBOOTMODEMCMD, 30 * 1000);
}
void startInteractiveConsoleWithModem(SoftwareSerial &SerialAT)
{
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
        if (SerialAT.available())
        {

            while (SerialAT.available())
            {
                byte c = SerialAT.read();
                Serial.write(fixATchar(c));

                // Serial.println((byte)fixATchar(c));
            }
        }
        if (Serial.available() <= 0 && SerialAT.available() <= 0)
        {
            Serial.flush();
            SerialAT.flush();
        }
    }
}
int main()
{
    sei(); // start listening to interrupts
    DDRB |= 1 << PORTB5;
    Serial.begin(115200);

    // Access AT commands from Serial Monitor

    // SerialAT.print(SETSERIALSPEEDCMD);
    // SerialAT.print("=");
    // SerialAT.println(ATCONSOLESPEED);
    // SerialAT.flush();
    sleep(100);
    // Serial.println(SendAT("AT",10,&SerialAT));
    SoftwareSerial SerialAT(2, 3); // connect the rxd to port 3 and the txd to port 2
    SendAT("AT", 0, &SerialAT);    // assign the object as static in the function
    initialModem(&SerialAT);
    startInteractiveConsoleWithModem(SerialAT);

    return 0;
}