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
    
}
byte _checkModemStatus(){
    
}
void _powerCycleModem(){
    dbg("starting power cycle");
    pinMode(PWRKEY_PIN,OUTPUT); //software serial uses it so use it aswell
    digitalWrite(PWRKEY_PIN,LOW);
    sleep(200);
    digitalWrite(PWRKEY_PIN,HIGH);
    pinMode(PWRKEY_PIN,INPUT);
}
void _resetPDPDeact(){
    //didnt find a way yet only restart or just try to reactivate the pdp
}
void _conncectToSerevr(){
    //must run initialModem before
    //maybe a connection is saved on the modem but is down so close it in case
    SendAT((String)CLOSE_CONNECTION_CMD+"="+DEFAULT_CID_IDX);
    byte tries = 50;
    do {
    String res = SendAT((String)CONNECT_TO_SERVER_CMD+"="+DEFAULT_CID_IDX+","+DEFAULT_PDP_IDX+","+"TCP"+",\""+SERVER_IP+"\","+SERVER_PORT,CONNECT_CMD_MAX_TIMEOUT_SEC*1000); //returnes OK usually, for now not handling other types of outputs

    res = SendAT((String)CONNECT_TO_SERVER_CMD+"?");

    if (res.indexOf((String)+DEFAULT_CID_IDX+","+DEFAULT_PDP_IDX+","+"TCP") !=-1){// supposing the rest is right
        dbg("connected to the server successfully!");
         return;
    }
    } while (--tries !=0);

    dbg("failed connecting to the server!");
    stopProgram();
}
byte _BringUpGPRSConnection(){
    dbg("trying to use mobile data");
        String res = SendAT(BRING_UP_WIRELESS_CONNECTION_GPRS,65*1000);
        SendAT("AT",65000); // the BRING_UP_WIRELESS_CONNECTION_GPRS returnes an output when it gets it but its not the response after that returns the response then waiting for it like that
        String localIp = SendAT(GET_LOCAL_IP_ADDRESS_CMD);
        //returnes +CNACT line for each connection with +CNACT: <pdpidx>,<statusx>,<addressx>  7.2.1
            if (localIp.indexOf((String)"+CNACT: "+DEFAULT_PDP_IDX+",1") !=-1) //
            
            { // 3 IP GPRSACT means connected
                strtok(localIp.begin(),"\"");
                for(byte i=1;i<DEFAULT_PDP_IDX*2;++i)
                    strtok(NULL,"\""); 

                dbg((String)"local ip is:" + strtok(NULL,"\""));
                return 1;
            }
    
        
    return 0;
}
void _StartDataSend(size_t dataLength){
    SendAT((String)SEND_DATA_CMD+"="+DEFAULT_CID_IDX+","+"dataLength"); //if something went wrong, after 5 seconds(by default) cancel the sending 
    //returns ok by default so no need to save the output
}
byte* _StopDataSend(){
    return (byte*)malloc(1); // just so the free has something to free
    //do nothing for now, StartDataSend handles the whole input with expected size
}