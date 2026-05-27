#pragma once

//https://m2msupport.net/m2msupport/tcp-ip-testing-with-simcom-sim7070-sim7080-modules/ is very helpful
#define CONNECT_TO_SERVER_CMD "AT+CAOPEN"
#define PWRKEY_PIN 16 //A2 soon to be implamented
#define CONFIG_PDP_CMD "AT+CNCFG" // 7.2.2
#define DEFAULT_PDP_IDX 0
#define DEFAULT_CID_IDX 0 // what connection slot to use
#define CONNECT_CMD_MAX_TIMEOUT_SEC 10
#define SET_TCP_MAX_TIMEOUT_CMD "AT+CACFG=\"TIMEOUT\",0,"
#define SEND_DATA_CMD "AT+CASEND"
#define RECEIVE_DATA_CMD "AT+CARECV"

//must be defined in each sim header
#define SET_APN_CMD_FULL CONFIG_PDP_CMD +"="+DEFAULT_PDP_IDX+",1,"+APN_NAME
#define REBOOT_MODEM_CMD "AT+CFUN=6"
#define TEST_COMMAND "AT"
#define CLOSE_CONNECTION_CMD "AT+CACLOSE"
#define GET_LOCAL_IP_ADDRESS_CMD "AT+CNACT?"
#define BRING_UP_WIRELESS_CONNECTION_GPRS "AT+CNACT=0,1"





//this file is for each modemtype implamentation because for example sim7000 has different start up procedure then sim7080
//used in modemCore that is importing some specific implamentation used in this file for example AT commands
void _initialModem(SoftwareSerial *AT){
    SendAT((String)SET_TCP_MAX_TIMEOUT_CMD+CONNECT_CMD_MAX_TIMEOUT_SEC);
    //by default is at multi connection mode
    SendAT("ATE0"); // disable command echo in from the modem
    SendAT("AT+CSCS=\"UCS2\""); // set the correct charset to use
    
}
byte _checkModemStatus(){
    
}
void _powerCycleModem(){
    dbg("starting power cycle");
    pinMode(PWRKEY_PIN,OUTPUT); //software serial uses it so use it aswell
    digitalWrite(PWRKEY_PIN,LOW);
    sleep(1100);
    digitalWrite(PWRKEY_PIN,HIGH);
    pinMode(PWRKEY_PIN,INPUT);
}
void _resetPDPDeact(){
    //didnt find a way yet only restart or just try to reactivate the pdp
}
bool _conncectToSerevr(){
    //must run initialModem before
    //maybe a connection is saved on the modem but is down so close it in case
    SendAT((String)CLOSE_CONNECTION_CMD+"="+DEFAULT_CID_IDX);
    byte tries = 5;
    do {
    String res = SendAT((String)CONNECT_TO_SERVER_CMD+"="+DEFAULT_CID_IDX+","+DEFAULT_PDP_IDX+","+"TCP"+",\""+SERVER_IP+"\","+SERVER_PORT,CONNECT_CMD_MAX_TIMEOUT_SEC*1000); //returnes OK usually, for now not handling other types of outputs
    dbg(res);
    sleep(200);
    res = SendAT((String)CONNECT_TO_SERVER_CMD+"?");

    if (res.indexOf((String)+DEFAULT_CID_IDX+","+DEFAULT_PDP_IDX+","+"TCP") !=-1){// supposing the rest is right
        dbg("connected to the server successfully!");
         return true;
    }
    } while (--tries !=0);

    dbg("failed connecting to the server!");
    return false;
}
byte _BringUpGPRSConnection(){
    dbg("trying to use mobile data");
        String res = SendAT(BRING_UP_WIRELESS_CONNECTION_GPRS,65*1000);
        SendAT("AT",65000); // the BRING_UP_WIRELESS_CONNECTION_GPRS returnes an output when it gets it but its not the response after that returns the response then waiting for it like that
        String localIp = SendAT(GET_LOCAL_IP_ADDRESS_CMD);
        //returnes +CNACT line for each connection with +CNACT: <pdpidx>,<statusx>,<addressx>  7.2.1
            if (localIp.indexOf((String)"+CNACT: "+DEFAULT_PDP_IDX+",1") !=-1) //
            { 
                strtok(localIp.begin(),"\"");
                for(byte i=1;i<DEFAULT_PDP_IDX*2;++i)
                    strtok(NULL,"\""); 

                dbg((String)"local ip is:" + strtok(NULL,"\""));
                return 1;
            }
    
        
    return 0;
}
void _StartDataSend(size_t dataLength){
    String Query = (String)SEND_DATA_CMD+"="+DEFAULT_CID_IDX+","+dataLength;
    
    dbg(SendAT(Query,5000)); //if something went wrong, after 5 seconds(by default) cancel the sending 
    //returns ok by default so no need to save the output
}
byte* _StopDataSend(){
    dbg(SendAT("",10000)); //wait for ok from the server
    return (byte*)malloc(1); // just so the free has something to free
    //do nothing for now, StartDataSend handles the whole input with expected size
}
byte* _waitForServerResponse(unsigned long &size, unsigned long &Timeoutms){
    //implament receiving data on the server connection
    String res = SendAT((String)RECEIVE_DATA_CMD+"?");
    while(res.indexOf("+CARECV")==-1){
        res = SendAT((String)RECEIVE_DATA_CMD+"?");
        if(Timeoutms>1){
            Timeoutms -=100;
        }else{
            //no response in time
            size=0;
            return nullptr;
        }
    }
    unsigned long tmpSize=0;
    unsigned long newSize =~tmpSize;
    while (newSize != tmpSize)
    {
        newSize=tmpSize;
        //example response for that: "+CARECV: 4,IMK"
        res = SendAT((String)RECEIVE_DATA_CMD+"?");
        strtok(res.begin(),((String)DEFAULT_CID_IDX+",").begin());
        char* buf = strtok(nullptr,"\n")+1;// skip first char
        sscanf(buf,"%d",&tmpSize);//gets the string after "<DEFAULT_CID_IDX>," to "\n"
        if(strlen(buf)==0){
            newSize = ~tmpSize;
        }
    }
    
    size=tmpSize;

    dbg("buffer size for receiving is:",0);
    dbg(size);
    //do size checks here altho the modem cant have infinite buffer also
    SendAT((String)RECEIVE_DATA_CMD+"="+DEFAULT_CID_IDX+","+size,0);
     newSize =0;
    skipUntilChar(',');
    fixATchar(0,2); // date mode(just temporarly to make it work)
    byte* output = GetATResponse(newSize,Timeoutms);
    // printArr(output,newSize); // for debugging if needed
    fixATchar(0,0);
    newSize -= strlen("\r\n\r\nOK\r\n");
    if(newSize !=size){
        dbg("incorrect size or buffer overflow while getting data from server in waitForServerResponse");
        return output;
    }
    
    return (byte*)reallocSafe(output,newSize);//relaese the end of the array because we shifted it down
}