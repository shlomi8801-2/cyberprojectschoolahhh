#pragma once

//https://m2msupport.net/m2msupport/atcscs-select-te-character-set/
//the modem starts at IP INITIAL status then requires to set apn with AT+CSTT or something
//to switch to IP START then you can use the AT+CIICR then if successful switches to IP GPRSAC/IP STATUS status

#define APN_NAME "iisp"
#define DEFAULT_TIMEOUT_SEC 5
#define APN_TASK_MAX_RESPONSE_TIME_SEC 120
#define AT_CONSOLE_SPEED 115200
#define MODEM_RDX_PORT 14 // connect the txd of the modem to port 14(A0) and the rxd to port 15(A1)
#define MODEM_TDX_PORT 15
#define SERVER_IP "192.168.161.170"
#define SERVER_PORT 1234



#define GET_SUBSCRIPTION_NUMBER "AT+CNUM"
#define TEST_COMMAND "AT"
#define START_TCP_CONNECTION "AT+CIPSTART" // section 8 in the document
#define QUALITY_REPORT_CMD "AT+CSQ" // 3.2.16
//https://github.com/DFRobot/DFRobot_SIM/blob/master/DFRobot_SIMclient.cpp might be helpful
#define BRING_UP_WIRELESS_CONNECTION_GPRS "AT+CIICR" // 8.2.10
#define GET_CURR_STATUS_CMD "AT+CIPSTATUS" // 8.2.13 
#define SET_APN_CMD "AT+CSTT" // add = at the end to set the apn 5.2.27
#define SET_SERIAL_SPEED_CMD "AT+IPR" // use as AT+IPR=<rate> 2.2.32
#define SET_MODEM_FUNCTIONALITY_CMD "AT+CFUN" //3.2.19
#define REBOOT_MODEM_CMD "AT+CFUN=6"
#define RESET_PDP_DEACT_STATE_CMD "AT+CIPSHUT"
#define GET_LOCAL_IP_ADDRESS_CMD "AT+CIFSREX" //8.2.12
#define TCP_EXAMPLE_CONNECT_REMOTE_ECHO_SERVER "AT+CIPSTART=\"TCP\",\"45.79.112.203\",4242" //https://tcpbin.com
#define CONNECT_TO_SERVER_CMD "AT+CIPSTART"// must be at IP STATUS state witch executed by using AT+CIFSR, send with args for like this ...="<TCP/UDP>"","<host>",<port> 
#define CONNECT_CMD_MAX_TIMEOUT_SEC 160
#define ENTER_DATA_MODE_CMD "AT+CIPSEND" // data received as normal 8 bit each time and at every sent data it has to add 0x1a to execute the message sending

