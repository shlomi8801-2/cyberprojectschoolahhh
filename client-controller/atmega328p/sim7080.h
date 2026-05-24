#pragma once




//must be defined in each sim header
#define SET_APN_CMD_FULL "testapn"
#define REBOOT_MODEM_CMD "AT+CFUN=6"
#define TEST_COMMAND "AT"
#define CLOSE_CONNECTION_CMD "AT+CIPCLOSE"
#define GET_LOCAL_IP_ADDRESS_CMD "AT+CIFSREX" //8.2.12
#define BRING_UP_WIRELESS_CONNECTION_GPRS "AT+CIICR" // 8.2.10





//this file is for each modemtype implamentation because for example sim7000 has different start up procedure then sim7080
//used in modemCore that is importing some specific implamentation used in this file for example AT commands
void _initialModem(SoftwareSerial *AT){

}
byte _checkModemStatus(){
    
}
void _resetPDPDeact(){
    
}
void _conncectToSerevr(){
    //must run initialModem before
    byte tries = 50;
    do {
    // String res = SendAT((String)CONNECT_TO_SERVER_CMD+"=\"TCP\",\""+SERVER_IP+"\","+SERVER_PORT); //returnes OK usually, for now not handling other types of outputs
    // res = SendAT("",CONNECT_CMD_MAX_TIMEOUT_SEC*1000);
    // dbg(res);
    // if (res.indexOf("CONNECT OK") !=-1 || res.indexOf("ALREADY CONNECT") !=-1){
    //     dbg("connected to the server successfully!");
    //      return;
    // }
    } while (--tries !=0);

    dbg("failed connecting to the server!");
    stopProgram();
}