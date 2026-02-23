#pragma once

//https://m2msupport.net/m2msupport/atcscs-select-te-character-set/
//the modem starts at IP INITIAL status then requires to set apn with AT+CSTT or something
//to switch to IP START then you can use the AT+CIICR then if successful switches to IP GPRSAC status

#define APNNAME "iisp"
#define GETSUBSCRIPTIONNUMBER "AT+CNUM"
#define TESTCOMMAND "AT"
#define STARTTCPCONNECTION "AT+CIPSTART" // section 8 in the document
#define ATCONSOLESPEED 115200
#define QUALITYREPORTCMD "AT+CSQ" // 3.2.16
//https://github.com/DFRobot/DFRobot_SIM/blob/master/DFRobot_SIMclient.cpp might be helpful
#define BRINGUPWIRELESSCONNECTIONGPRS "AT+CIICR" // 8.2.10
#define GETCURRSTATUSCMD "AT+CIPSTATUS" // 8.2.13 
#define SETAPNCMD "AT+CSTT" // add = at the end to set the apn 5.2.27
#define SETSERIALSPEEDCMD "AT+IPR" // use as AT+IPR=<rate> 2.2.32