#pragma once
//this file is for each modemtype implamentation because for example sim7000 has different start up procedure then sim7080
//used in modemCore that is importing some specific implamentation used in this file for example AT commands
void _initialModem(SoftwareSerial *AT){
    byte status = checkModemStatus();
        if (status == 3 || status == 4)
            return;
        dbg("setting apn");
        setModemAPN();
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