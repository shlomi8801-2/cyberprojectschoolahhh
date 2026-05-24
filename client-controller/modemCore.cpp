#include "modemCore.h"

#include "atmega328p/modem.h"

char fixATchar(const char c,byte dataMode)
{
    // bitSet(UCSR0A,U2X0);
    static byte _dataMode;
    if(dataMode !=2)
        _dataMode=dataMode;
    switch (_dataMode)
    {
    case 3:
        return c;

    case 0:{
        if (c & 1 << 7)
        return  c ^ (11 << 6);
        else
            return c & 63; // 0011 1111
    }
    case 4:
        if (c & 1 << 7)
        return c ^ (1<<7);
        else
        return c;
    default:
        return c ^ (1<<7);
    }   
}
String SendAT(String str, unsigned long Timeoutms, SoftwareSerial *AT){
    dbg(">> "+str);
    return SendATHelper(str,str.length(),Timeoutms,AT);
}
String SendAT(const char* str, unsigned long Timeoutms, SoftwareSerial *AT){
    return SendAT((String)str,Timeoutms,AT);
}
String SendATArr(const char* str,unsigned long size, unsigned long Timeoutms, SoftwareSerial *AT){
    return SendATHelper(str,size,Timeoutms,AT);
}
static SoftwareSerial *_AT;

void SkipNATCharacters(int n,unsigned long Timeoutms){
    //waits for n characters from the modem with max timeout
    while (Timeoutms > 0 && _AT->available() <= 0 && n>0)
    {
        sleep(10);
        Timeoutms -= 10;
        while(_AT->available()){//sometimes it comes with delay from each character
            _AT->read();
            --n;
        }
    }
}



byte* GetATResponse(unsigned long& size,unsigned long Timeoutms, SoftwareSerial *AT){
    while (Timeoutms > 0 && _AT->available() <= 0)
    {
        sleep(10);
        Timeoutms -= 10;
    }
    if (Timeoutms <= 0 && _AT->available() <= 0)
    {
        byte* res = (byte*)malloc(11);
        memcpy(res,"NO RESPONSE",11);
        return res;
    }    
   
    byte* output = (byte*)malloc(sizeof(byte));
    // for(byte i=0;i<4;i++)
    //         output[i]=_AT->read();
    // output = (byte*)realloc(output,sizeof(byte)* (*(unsigned long*)output));

    size=1;
    for (unsigned long i=0;_AT->available();i++)
    {
        output = (byte*)reallocSafe(output,++size);
        if (output == nullptr){
            dbg("out of memory in GetATResponse");
            stopProgram();
        }
        char c = _AT->read();
        c = fixATchar(c);
        output[i]=c;
    }
    return (byte*)reallocSafe(output,--size);
}
byte* SendAT(const char str ,unsigned long& size,unsigned long Timeoutms, SoftwareSerial *AT){
    SendATHelper(&str,1,0,AT);
 return GetATResponse(size,Timeoutms,AT);
}

template <typename T> // support for strings and char arrays
String SendATHelper(const T str,unsigned long size, unsigned long Timeoutms, SoftwareSerial *AT)
{
    // sends a string to the AT serial and then returns the reponse
    
    if (AT)
    {
        _AT = AT;
        _AT->begin(AT_CONSOLE_SPEED);
        dbg("using speed:",0);
        dbg(AT_CONSOLE_SPEED);
    }
    else if (!_AT)
        return "NO AT SERIAL OBJECT";
    for(unsigned long i=0;i<size;i++){
        _AT->print((char)str[i]);
    }
    _AT->print("\r\n");
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
    return _checkModemStatus();
}
byte waitForATResponse(unsigned int maxTimeoutSec)
{
    // execute AT until response - not setting the serial object in SendAT function
    maxTimeoutSec *= 100;
    while (maxTimeoutSec > 0)
    {
        SendAT((String)(char)(0x1a));
        String res = SendAT((String)"AT", 1000);
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
    dbg(SendAT((String)SET_APN_CMD_FULL,APN_TASK_MAX_RESPONSE_TIME_SEC*1000));
    
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
    _resetPDPDeact();
}
byte BringUpGPRSConnection(){
    dbg("trying to use mobile data");
        SendAT(BRING_UP_WIRELESS_CONNECTION_GPRS);
        SendAT("AT",65000); // the BRING_UP_WIRELESS_CONNECTION_GPRS returnes an output when it gets it but its not the response after that returns the response then waiting for it like that
        // finally
        String localIp = SendAT(GET_LOCAL_IP_ADDRESS_CMD);
        if (localIp.indexOf('.')!=-1) // 
        { // 3 IP GPRSACT means connected
            dbg("local ip is:" + localIp);
            return 1;
        }
    return 0;
}

void closeConnectionToServer(){
    SendAT(CLOSE_CONNECTION_CMD);
}
void initialModem(SoftwareSerial *AT)
{
    dbg("initializing modem!");
    fixATchar('0',0);

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
        _initialModem(AT);
        if (BringUpGPRSConnection()) break;
    }
    dbg("modem initionlized!");
}

inline void rebootModem()
{
    SendAT(REBOOT_MODEM_CMD);
}
void conncectToSerevr(){
    _conncectToSerevr();
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
