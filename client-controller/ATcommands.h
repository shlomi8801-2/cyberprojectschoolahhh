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

#define SERVER_IP "10.8.5.121"
#define SERVER_PORT 1234
#define HEADER_SIZE_BYTES 4 //HEADER_SIZE_BYTES must be under 5
#define RAMTOTAL RAMEND-RAMSTART

// -----------------define your modem file(located in atmega328p/modem.h)----------------
// #define sim7000;
#define sim7080;
//---------------------------------------------------------------------------------------

// #define _SS_MAX_RX_BUFF 256 // packets arriving are stored in a buffer for now at least because the modem just sends all of the data at once so change the max buffer from 64
//changed in the upload to arduino script as compiler flag