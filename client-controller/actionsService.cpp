#include "actionsService.h"


void registerClient(){

}
void loginClient(){

}
    String getValue(byte* data){
    long currSize =0;
        //get the size of the value
        // cout<<currSize;
        for (int _ =0; _<HEADER_SIZE_BYTES;_++){ //HEADER_SIZE_BYTES must be under 5
            currSize += data[0]<<sizeof(data[0])*(_);
            data++;
        }
        // data++; //skip the encode bit for now
        //read the value
        String output = "";
        for(long _=0;_<currSize;_++){
            output +=*data;
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
    byte* output =  (byte*)calloc(sizeof(char)*HEADER_SIZE_BYTES,1);
    unsigned int outputSize=HEADER_SIZE_BYTES;
    unsigned int idx=HEADER_SIZE_BYTES;
    for (auto i:data){
        output[idx]=i.key.length(); // writing 2 bytes i think(using buffer overflow)
        idx +=HEADER_SIZE_BYTES;
        outputSize +=i.key.length()+HEADER_SIZE_BYTES;
        output = (byte*)realloc(output,outputSize);
        for(auto c :i.key){
            output[idx++]=c;
        }
        dbg((String)i.key+" ");
        dbg((String)i.value+" ");

        output[idx]=i.value.length(); // writing 2 bytes i think(using buffer overflow)
        idx +=HEADER_SIZE_BYTES;
        outputSize +=i.value.length()+HEADER_SIZE_BYTES;
        output = (byte*)realloc(output,outputSize);
        for(auto c :i.value){
            output[idx++]=c;
        }
    }
    output = (byte*)realloc(output,outputSize-HEADER_SIZE_BYTES);
    output[0] = outputSize;
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