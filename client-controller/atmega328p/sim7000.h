#pragma once



#define GET_SUBSCRIPTION_NUMBER "AT+CNUM"

#define START_TCP_CONNECTION "AT+CIPSTART" // section 8 in the document
#define QUALITY_REPORT_CMD "AT+CSQ" // 3.2.16
//https://github.com/DFRobot/DFRobot_SIM/blob/master/DFRobot_SIMclient.cpp might be helpful
#define GET_CURR_STATUS_CMD "AT+CIPSTATUS" // 8.2.13 
#define SET_APN_CMD "AT+CSTT" // add = at the end to set the apn 5.2.27
#define SET_SERIAL_SPEED_CMD "AT+IPR" // use as AT+IPR=<rate> 2.2.32
#define SET_MODEM_FUNCTIONALITY_CMD "AT+CFUN" //3.2.19

#define RESET_PDP_DEACT_STATE_CMD "AT+CIPSHUT"
#define TCP_EXAMPLE_CONNECT_REMOTE_ECHO_SERVER "AT+CIPSTART=\"TCP\",\"45.79.112.203\",4242" //https://tcpbin.com
#define CONNECT_TO_SERVER_CMD "AT+CIPSTART"// must be at IP STATUS state witch executed by using AT+CIFSR, send with args for like this ...="<TCP/UDP>"","<host>",<port> 
#define CONNECT_CMD_MAX_TIMEOUT_SEC 160
#define ENTER_DATA_MODE_CMD "AT+CIPSEND" // data received as normal 8 bit each time and at every sent data it has to add 0x1a to execute the message sending
#define WAIT_FOR_SERVER_AKNOLAGEMENTCMD "AT+CIPQSEND" // = either 0 or 1 https://electronics.stackexchange.com/questions/508699/sim7000e-modem-stops-sending-send-ok-reply

//must be defined in each sim header
#define SET_APN_CMD_FULL SET_APN_CMD + "=" + APN_NAME
#define REBOOT_MODEM_CMD "AT+CFUN=6"
#define TEST_COMMAND "AT"
#define CLOSE_CONNECTION_CMD "AT+CIPCLOSE"
#define GET_LOCAL_IP_ADDRESS_CMD "AT+CIFSREX" //8.2.12
#define BRING_UP_WIRELESS_CONNECTION_GPRS "AT+CIICR" // 8.2.10





//this file is for each modemtype implamentation because for example sim7000 has different start up procedure then sim7080
//used in modemCore that is importing some specific implamentation used in this file for example AT commands
void _initialModem(SoftwareSerial *AT){
    byte status = checkModemStatus();
        if (status == 3 || status == 4)
            return;
        dbg("setting apn");
        setModemAPN();
        //set modem settings here
        SendAT((String)WAIT_FOR_SERVER_AKNOLAGEMENTCMD+"=1");
        SendAT("ATE0"); // disable command echo(the modem usually echoing the command used)

        status = checkModemStatus();
        if (status != 1){
            dbg("status is:" + (String)status);
            switch (status){
                case 10:{
                    resetPDPDeact();
                    return;
                }
                case 8:
                    sleep(100);
                case 9:
                case 3:
                case 4:
                    return;
                case 7:
                    closeConnectionToServer();
                    return;
                case 12:
                    dbg("error occoured in the modem whil quiring status see 8.2.13 starting interactive shell...");
                    startInteractiveConsoleWithModem(*AT);
                case 255:
                    return;
                default:
                    resetModemAndWait();
        }
}

}
byte _checkModemStatus(){
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
        sleep(250);
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
void _resetPDPDeact(){
    SendAT(RESET_PDP_DEACT_STATE_CMD);
}
void _conncectToSerevr(){
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
byte _BringUpGPRSConnection(){
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