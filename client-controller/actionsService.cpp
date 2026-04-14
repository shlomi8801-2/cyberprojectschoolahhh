#include "actionsService.h"


void registerClient(){

}
void loginClient(){

}
String getValue(byte* data){
    long currSize =0;
        //get the size of the value
        for (byte _ =0; _<HEADER_SIZE_BYTES;_++){ //HEADER_SIZE_BYTES must be under 5
            currSize += data[0];
            currSize <<= 8; // move 1 byte
            data++;
        }
        data++; //skip the encode bit for now
        //read the value
        String output = "";
        for(long _=0;_<currSize;_++){
            output +=(char)data[0];
            data++;
        }
        return output;
}
Hashtable parseData(byte* data,int size){
    //byte array which looks like this <length><encoded?><data><length2><encoded?><data2> for exanple:   \x00\x00\x00\x04\x00code\x00\x00\x00\x05\x00abcde
    //encoded means is the data bin or chars
    //the length is always as the HEADER_SIZE the data is always encoded with the encoding in settings
    Hashtable output;
    for(int i =0;i<size;i++){
        String key =getValue(data);
        String value =getValue(data);
        output[key]=value;
    }
    return output;
        

}
// byte* ToByteArr(String data,int* OutputSize){
//         if (data.equals("")){
//             return 0;
//         }
//         byte* output = (byte*)malloc(1);
//         *OutputSize = 0;
//         for (byte i: data){
//             output[*OutputSize++] = i;
//         }
//         return output;
// }

byte* buildData(Hashtable data){
    //use HEADER_SIZE bytes for the length of the value
    // for 2^(8*HEADER_SIZE) of data support
    // takes the dict and turns into a simple string then bytearray
    byte* output =  (byte*)malloc(sizeof(byte)*4);
    unsigned int outputSize=4;
    unsigned int idx=0;
    for (auto i: data){
        output[idx++]=0;
        output[idx++]=0;//because int is 2 bytes in the arduino im using i set the first 2 to 0
        output[idx++]=i.key.length(); // writing 2 bytes i think(using buffer overflow)
        outputSize +=i.key.length();
        realloc(output,outputSize);
        for(auto c :i.value){
            output[idx++]=c;
        }
    }
    realloc(output,outputSize+1);
    output[idx]=0;//ending char
    return output;
}

void RegisterToServer(){
    dbg("registering to server");
    SendAT(ENTER_DATA_MODE_CMD);
    Hashtable test;
    test["type"]="REG";
    dbg("sending REG");
    String cmd((char*)buildData(test));
    
    SendAT("hello world",0);
    SendAT((String)(char)(0x1a));
    
}
void ConnectToServer();
void StartConnectionToServer();