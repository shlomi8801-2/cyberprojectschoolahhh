#include "modemCore.h"

String SendAT(String str, unsigned long Timeoutms, SoftwareSerial *AT)
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
    if (1)
        dbg<String>(">> " + (String)str);
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
        static const char *const modemStatues[] = {"IP INITIAL", "IP START", "IP CONFIG", "IP GPRSACT", "IP STATUS", " CONNECTING", "SERVER LISTENING", "CONNECT OK", " CLOSING", " CLOSED", "PDP DEACT", "IP PROCESSING","ERROR"};
        // const char * currStatus = nullptr; // const data in pointer but not the pointer
        byte currStatus = ~0;
        for (int i = 0; i < sizeof(modemStatues) / sizeof(modemStatues[0]); ++i)
        {
            if (res.indexOf(modemStatues[i]) != -1)
            {
                return i;
            }
        }
        dbg((String)res + "status not defined");
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
    // 12 the modem responded with ERROR
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
        SendAT("AT",65000); // the BRING_UP_WIRELESS_CONNECTION_GPRS returnes an output when it gets it but its not the response after that returns the response then waiting for it like that
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
            stopProgram();
            return;
        }
        sleep(1000);
        byte status = checkModemStatus();
        if (status == 3 || status == 4)
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
void conncectToSerevr(){
    //must run initialModem before
    byte tries = 50;
    do {
    String res = SendAT((String)CONNECT_TO_SERVER_CMD+"=\"TCP\",\""+SERVER_IP+"\","+SERVER_PORT); //returnes OK usually, for now not handling other types of outputs
    res = SendAT("",CONNECT_CMD_MAX_TIMEOUT_SEC*1000);
    dbg(res);
    if (res.indexOf("CONNECT OK") !=-1 || res.indexOf("ALREADY CONNECT") !=-1){
        dbg("connected to the server successfully!");
         return;
    }
    } while (--tries !=0);

    dbg("failed connecting to the server!");
    stopProgram();
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
