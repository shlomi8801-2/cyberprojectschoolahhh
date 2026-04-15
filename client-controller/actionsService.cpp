#include "actionsService.h"


void registerClient(){

}
void loginClient(){

}
    String getValue(byte* data){
    unsigned long currSize =0;
        //get the size of the value
        // cout<<currSize;
        for (int _ =0; _<HEADER_SIZE_BYTES;_++){ //HEADER_SIZE_BYTES must be under 5
            dbg(data[0]);
            currSize += data[0]<<sizeof(data[0])*(_);
            data++;
        }
        dbg("got size:");
        dbg(currSize);
        // data++; //skip the encode bit for now
        //read the value
        String output = "";
        for(long _=0;_<currSize;_++){
            output +=*data;
            data++;
        }
        dbg(output);
        return output;
}
Hashtable parseData(byte* data,long size){
    //byte array which looks like this <length><encoded?><data><length2><encoded?><data2> for exanple:   \x00\x00\x00\x04\x00code\x00\x00\x00\x05\x00abcde
    //encoded means is the data bin or chars
    //the length is always as the HEADER_SIZE the data is always encoded with the encoding in settings
    Hashtable output;
    
    for(int i =0;i<size;i++){
        String key =getValue(data);
        i+=key.length();
        String value =getValue(data);
        i+=key.length();

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
    byte* output =  (byte*)calloc(sizeof(char)*HEADER_SIZE_BYTES,2);
    unsigned int outputSize=sizeof(char)*HEADER_SIZE_BYTES*2;
    unsigned int idx=HEADER_SIZE_BYTES;
    for (auto i:data){
        clearNextBytes(output+idx,HEADER_SIZE_BYTES);
        output[idx]=i.key.length(); // using buffer overflow
        // for(int x=0;x<outputSize;x++)
        //     dbg((String)output[x]+" ",0);
        //     dbg("",1);
        idx +=HEADER_SIZE_BYTES;
        outputSize +=i.key.length()+HEADER_SIZE_BYTES;
        
        output = (byte*)realloc(output,outputSize);
         
        for(auto c :i.key){
            output[idx++]=c;
        }
       
        
        
        
        // idx +=1; adding encoding bit
        // outputSize+=1;
        
        clearNextBytes(output+idx,HEADER_SIZE_BYTES);
        
        output[idx]=i.value.length(); // using buffer overflow
        
        idx +=HEADER_SIZE_BYTES;
        
        outputSize +=i.value.length()+HEADER_SIZE_BYTES;
        output = (byte*)realloc(output,outputSize);
        
        for(auto c :i.value){
            output[idx++]=c;
        }
        
    }
    outputSize -= HEADER_SIZE_BYTES;
    output = (byte*)realloc(output,outputSize);
    *(unsigned long*)output = outputSize;
    
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