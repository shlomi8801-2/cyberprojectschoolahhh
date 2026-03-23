// my arduino chip is atmega328p
#define __AVR_ATmega328P__
#include <avr/io.h>
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <util/delay.h>
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

String SendAT(String str, unsigned long Timeoutms = 1000, SoftwareSerial *AT = nullptr)
{
    // sends a string to the AT serial and then returns the reponse
    static SoftwareSerial *_AT;
    if (AT)
    {
        _AT = AT;
        _AT->begin(AT_CONSOLE_SPEED);
        dbg("using speed:");
        dbg(AT_CONSOLE_SPEED);
    }
    else if (!_AT)
        return "NO AT SERIAL OBJECT";
    dbg(">> " + str);
    _AT->println(str);
    _AT->flush();

    while (Timeoutms > 0 && _AT->available() <= 0)
    {
        sleep(10);
        Timeoutms -= 10;
    }
    if (Timeoutms <= 0 && _AT->available() <= 0)
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

        const String res = SendAT(GET_CURR_STATUS_CMD);
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
    // 4 IP STATUS
    // 5 TCP CONNECTING/UDP CONNECTING
    // 6 SERVER LISTENING
    // 7 CONNECT OK
    // 8 TCP CLOSING/UDP CLOSING
    // 9 TCP CLOSED/UDP CLOSED
    // 10 PDP DEACT
    // 11 IP PROCESSING
}
byte waitForATResponse(unsigned int maxTimeoutSec)
{
    // execute AT until response - not setting the serial object in SendAT function
    maxTimeoutSec *= 100;
    while (maxTimeoutSec > 0)
    {
        String res = SendAT("AT", 1000);
        sleep(1000);
        maxTimeoutSec -= 100; // maxtimeout is seconds times 100 so -5 means -50ms
        if (res.indexOf("OK") != -1)
        {
            return 1;
        }
    }
    return 0; // no response
}
void setModemAPN()
{
    dbg(SendAT((String)SET_APN_CMD + "=" + APN_NAME,APN_TASK_MAX_RESPONSE_TIME_SEC*1000));
    
}
void resetModemAndWait()
{
    dbg("Rebooting might take some time");
    rebootModem();
    String res = SendAT("", 30*1000);
    while (res.indexOf("SMS ") == -1 && res.indexOf("OK") == -1)
    { // while not responding
        //try again each 3 seconds
        sleep(3000);
        res = SendAT("AT", 1000);
    }
}
void resetPDPDeact(){
    SendAT(RESET_PDP_DEACT_STATE_CMD);
}
byte BringUpGPRSConnection(){
    //must be in status 1 or 2 before
    //output should be either 0 or 1 0 means it failed to connect 1 means it connected
    dbg("trying to use mobile data");
        SendAT(BRING_UP_WIRELESS_CONNECTION_GPRS);
        SendAT("",65000); // the BRING_UP_WIRELESS_CONNECTION_GPRS returnes an output when it gets it but its not the response after that returns the response then waiting for it like that
        byte status = checkModemStatus();
        dbg("status is:" + (String)status);
        // finally
        if (status == 3)
        { // 3 IP GPRSACT means connected
            dbg("local ip is:" + SendAT(GET_LOCAL_IP_ADDRESS_CMD));
            return 1;
        }
    return 0;
}
void initialModem(SoftwareSerial *AT)
{
    dbg("initializing modem!");
    SendAT("AT", 0, AT); // assign the object as static in the function
    // should bring the modem from any status to 3 which is IP GPRSACT
    for( byte tries = 10;--tries > 0;/*SEGA*/)
    {
        if (!waitForATResponse(DEFAULT_TIMEOUT_SEC))
        {
            dbg("module not responding");
            return;
        }
        sleep(1000);
        byte status = checkModemStatus();
        if (status == 3)
            break;
        dbg("setting apn");
        setModemAPN();
        status = checkModemStatus();
        if (status != 1){
            dbg("status is:" + (String)status);
            switch (status){
                case 10:{
                    resetPDPDeact();
                    break;
                }
                case 255:
                    return;
                default:
                    resetModemAndWait();
            continue; // re run this block
        }
    }
        if (BringUpGPRSConnection()) break;
    }
    dbg("modem initionlized!");
}
inline void rebootModem()
{
    SendAT(REBOOT_MODEM_CMD);
}
void startInteractiveConsoleWithModem(SoftwareSerial &SerialAT){
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
            Serial.write(c); // replay
        }
        if (SerialAT.available())
        {

            while (SerialAT.available())
            {
                byte c = SerialAT.read();
                Serial.write(fixATchar(c));
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

    SoftwareSerial SerialAT(2, 3); // connect the rxd to port 3 and the txd to port 2
    initialModem(&SerialAT);

    // dbg(SendAT(TCP_EXAMPLE_CONNECT_REMOTE_ECHO_SERVER));
    // dataMode = 1;
    // SendAT("AT", 0, &SerialAT); // assign the object as static in the function

    startInteractiveConsoleWithModem(SerialAT);

    return 0;
}