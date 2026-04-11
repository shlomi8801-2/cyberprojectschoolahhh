#include "actionsService.h"


void registerClient(){

}
void loginClient(){

}
Hashtable<String,String> parseData(String data){
    //byte array which looks like this <length><encoded?><data><length2><encoded?><data2> for exanple:   \x00\x00\x00\x04\x00code\x00\x00\x00\x05\x00abcde
    //encoded means is the data bin or chars
    //the length is always as the HEADER_SIZE the data is always encoded with the encoding in settings
    //DON'T FORGET TO REMOVE THE DATA LENGTH FROM THE START
}
String buildData(Hashtable<String,String> data){
    //use HEADER_SIZE bytes for the length of the value
    // for 2^(8*HEADER_SIZE) of data support
    // takes the dict and turns into a simple string then bytearray
}
