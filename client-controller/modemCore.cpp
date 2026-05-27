#include "modemCore.h"
#include "atmega328p/modem.h"


char fixATchar(const char c, byte dataMode)
{
    // bitSet(UCSR0A,U2X0);
    static byte _dataMode;
    if (dataMode != _dataMode)
        _dataMode = dataMode;
    switch (_dataMode)
    {
    case 2:
        //data mode range 0-128
        //         0    
        // 1    
        // 2    
        // 3    
        // 4    
        // 5    
        // 6    
        // 7    
        // 8    
        // 9    
        // 10    
        // 11    
        // 12    
        // 13    
        // 14    
        // 15    
        // 16    
        // 17    
        // 18    
        // 19    
        // 20    
        // 21    
        // 22    
        // 23    
        // 24    
        // 25    
        // 26    
        // 27    
        // 28    
        // 29    
        // 30    
        // 31    //////////////////////////////// +64
        // 96    `
        // 97    a
        // 98    b
        // 99    c
        // 100    d
        // 101    e
        // 102    f
        // 103    g
        // 104    h
        // 105    i
        // 106    j
        // 107    k
        // 108    l
        // 109    m
        // 110    n
        // 111    o
        // 112    p
        // 113    q
        // 114    r
        // 115    s
        // 116    t
        // 117    u
        // 118    v
        // 119    w
        // 120    x
        // 121    y
        // 122    z
        // 123    {
        // 124    |
        // 125    }
        // 126    ~
        // 127    
        // 128    �
        // 129    �
        // 130    �
        // 131    �
        // 132    �
        // 133    �
        // 134    �
        // 135    �
        // 136    �
        // 137    �
        // 138    �
        // 139    �
        // 140    �
        // 141    �
        // 142    �
        // 143    �
        // 144    �
        // 145    �
        // 146    �
        // 147    �
        // 148    �
        // 149    �
        // 150    �
        // 151    �
        // 152    �
        // 153    �
        // 154    �
        // 155    �
        // 156    �
        // 157    �
        // 158    �
        // 159    �//////////////////////// +64
        // 224    �
        // 225    �
        // 226    �
        // 227    �
        // 228    �
        // 229    �
        // 230    �
        // 231    �
        // 232    �
        // 233    �
        // 234    �
        // 235    �
        // 236    �
        // 237    �
        // 238    �
        // 239    �
        // 240    �
        // 241    �
        // 242    �
        // 243    �
        // 244    �
        // 245    �
        // 246    �
        // 247    �
        // 248    �
        // 249    �
        // 250    �
        // 251    �
        // 252    �
        // 253    �
        // 254    �
        // 255    �
        if ((byte)c>159)
            return c-128;
        if ((byte)c >31)
            return (byte)c -64;
        return c;
    case 3:
        return c;

    case 0:
    {
        if (c & 1 << 7)
            return c ^ (11 << 6);
        else
            return c & 63; // 0011 1111
    }
    case 4:
        if (c & 1 << 7)
            return c ^ (1 << 7);
        else
            return c;
    case 5:
        return c-48;
        return c^ (1 << 7);
    default:
        return c ^ (1 << 7);
    }
}
String SendAT(String str, unsigned long Timeoutms, SoftwareSerial *AT)
{
    dbg(">> " + str);
    return SendATHelper(str, str.length(), Timeoutms, AT);
}
String SendAT(const char *str, unsigned long Timeoutms, SoftwareSerial *AT)
{
    return SendAT((String)str, Timeoutms, AT);
}
String SendATArr(const char *str, unsigned long size, unsigned long Timeoutms, SoftwareSerial *AT)
{
    return SendATHelper(str, size, Timeoutms, AT);
}
static SoftwareSerial *_AT;

void skipUntilChar(char c,size_t Timeoutms){
    //skips the output from the AT serial until certain char or timeout
     while (Timeoutms > 0)
    {
        while (_AT->available())
        { // sometimes it comes with delay from each character
            char tmp = fixATchar(_AT->read());
            if (tmp == c){
                return;
            }
            
        }
        sleep(10);
        Timeoutms -= 10;
    }
}
void SkipNATCharacters(int n, unsigned long Timeoutms)
{
    // waits for n characters from the modem with max timeout
    while (Timeoutms > 0 && _AT->available() <= 0 && n > 0)
    {
        sleep(10);
        Timeoutms -= 10;
        while (_AT->available())
        { // sometimes it comes with delay from each character
            _AT->read();
            --n;
        }
    }
}
byte *GetATResponseFixedSize(unsigned long size, unsigned long Timeoutms, SoftwareSerial *AT){
while (Timeoutms > 0 && _AT->available() <= 0)
    {
        sleep(10);
        Timeoutms -= 10;
    }
    if (Timeoutms <= 0 && _AT->available() <= 0)
    {
        byte *res = (byte *)malloc(11);
        memcpy(res, "NO RESPONSE", 11);
        return res;
    }

    byte *output = (byte *)malloc(sizeof(byte)*size);
    
    for (unsigned long i = 0; _AT->available(); i++)
    {
        char c = _AT->read();
        c = fixATchar(c);
        output[i] = c;
    }
    return output;
}
byte *GetATResponse(unsigned long &size, unsigned long Timeoutms, SoftwareSerial *AT)
{
    //reads from the AT serial buffer
    while (Timeoutms > 0 && _AT->available() <= 0)
    {
        sleep(10);
        Timeoutms -= 10;
    }
    if (Timeoutms <= 0 && _AT->available() <= 0)
    {
        byte *res = (byte *)malloc(11);
        memcpy(res, "NO RESPONSE", 11);
        return res;
    }

    byte *output = (byte *)malloc(sizeof(byte));
    if (output == nullptr)
        {
            dbg("out of memory in GetATResponse");
            stopProgram();
        }
    
    size = 1;
    unsigned long i = 0; // must be outside so it keeps the output
    while(_AT->available()){
        for (; _AT->available(); i++)
    {
        output = (byte *)reallocSafe(output, ++size);
        if (output == nullptr)
        {
            dbg("out of memory in GetATResponse");
            stopProgram();
        }
        char c = _AT->read();
        c = fixATchar(c);
        output[i] = c;
    }
    sleep(1);//in case there was a break that was too big
    }
    
    return (byte *)reallocSafe(output, --size);
}
byte* SendATchrArr(const char str, unsigned long &size, unsigned long Timeoutms, SoftwareSerial *AT)
{
    SendATHelper(&str, 1, 0, AT);
    return GetATResponse(size, Timeoutms, AT);
}

template <typename T> // support for strings and char arrays
String SendATHelper(const T str, unsigned long size, unsigned long Timeoutms, SoftwareSerial *AT)
{
    // sends a string to the AT serial and then returns the reponse

    if (AT)
    {
        _AT = AT;
        _AT->begin(AT_CONSOLE_SPEED);
        dbg("using speed:", 0);
        dbg(AT_CONSOLE_SPEED);
    }
    else if (!_AT)
        return "NO AT SERIAL OBJECT";
    for (unsigned long i = 0; i < size; i++)
    {
        _AT->write((char)str[i]);
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
        String res = SendAT((String) "AT", 1000);
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
    SendAT((String)SET_APN_CMD_FULL, APN_TASK_MAX_RESPONSE_TIME_SEC * 1000);
}
void resetModemAndWait()
{
    dbg("Rebooting might take some time");
    rebootModem();
    String res = SendAT("", 30 * 1000);
    while (res.indexOf("SMS ") == -1 && res.indexOf("OK") == -1)
    { // while not responding
        // try again each 3 seconds
        sleep(3000);
        res = SendAT("AT", 1000);
    }
}
void resetPDPDeact()
{
    _resetPDPDeact();
}
byte BringUpGPRSConnection()
{
    return _BringUpGPRSConnection();
}

void closeConnectionToServer()
{
    SendAT(CLOSE_CONNECTION_CMD);
}

void initialModem(SoftwareSerial *AT)
{
    dbg("initializing modem!");
    fixATchar('0', 0);

    SendAT("AT", 0, AT); // assign the object as static in the function


    for (byte tries = 10; --tries > 0; /*SEGA*/)
    {

        if (!waitForATResponse(DEFAULT_TIMEOUT_SEC))
        {
            dbg("module not responding");
            #ifdef PWRKEY_PIN
                // https://github.com/MikulasP/sim7080/blob/master/sim7080g.cpp#L1064
                _powerCycleModem();
                sleep(500);
            #endif
            continue;
        }
        // should bring the modem into to a point that it can activate network so BringUpGPRSConnection succeed
        _initialModem(AT);
        setModemAPN();
        if (BringUpGPRSConnection())
        dbg("modem initionlized!");
            return;
    }
    dbg("failed to use network!");
    stopProgram();
}

inline void rebootModem()
{
    SendAT(REBOOT_MODEM_CMD);
}
bool conncectToSerevr()
{
    return _conncectToSerevr();
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

void StartDataSend(size_t dataLength){
    _StartDataSend(dataLength);
}
byte* StopDataSend(){
    return _StopDataSend();
}
byte* waitForServerResponse(unsigned long &size, unsigned long Timeoutms){
    return _waitForServerResponse(size,Timeoutms);
}
void clearATBuffer(){
    while (_AT->available()){
        _AT->read();
    }
}