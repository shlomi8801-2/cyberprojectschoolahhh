#include "actionsService.h"


void registerClient(){

}
void loginClient(){

}
    String getValue(byte* data,int& CurrentIdx){
    unsigned long currSize =0;
        //get the size of the value
        // cout<<currSize;
        for (int _ =0; _<HEADER_SIZE_BYTES;_++){ //HEADER_SIZE_BYTES must be under 5
            currSize += data[0]<<sizeof(data[0])*(_);
            ++data;
        }
        // data++; //skip the encode bit for now
        //read the value
        String output = "";
        for(long _=0;_<currSize;_++){
            output +=(char)*data;
            ++data;
        }
        return output;
}

Hashtable parseData(byte* data){
    //byte array which looks like this <length><encoded?><data><length2><encoded?><data2> for exanple:   \x00\x00\x00\x04\x00code\x00\x00\x00\x05\x00abcde
    //encoded means is the data bin or chars
    //the length is always as the HEADER_SIZE the data is always encoded with the encoding in settings
    Hashtable output;
    dbg("size:",0);
    dbg(*(unsigned long*)data);
    dbg((int)*data );
    dbg((int)*(data+1));
    dbg((int)*(data+2));
    dbg((int)*(data+3));
    for(int i =HEADER_SIZE_BYTES;i<*(unsigned long*)data;i++){
        String key =getValue(data+i,i);
        dbg(key);
        i+=key.length();
        i+=HEADER_SIZE_BYTES;
        String value =getValue(data+i,i);
        dbg(value);
        i+=HEADER_SIZE_BYTES;
        i+=value.length();
        output[key]=value;
    }
    return output;
        

}


byte* buildData(Hashtable& data){
    //use HEADER_SIZE bytes for the length of the value
    // for 2^(8*HEADER_SIZE) of data support
    // takes the dict and turns into a simple string then bytearray
    byte* output =  (byte*)calloc(sizeof(char)*HEADER_SIZE_BYTES,2);
    unsigned int outputSize=sizeof(char)*HEADER_SIZE_BYTES*2;
    unsigned int idx=HEADER_SIZE_BYTES;
    for (auto i:data){
        clearNextBytes(output+idx,sizeof(byte)*HEADER_SIZE_BYTES);
        output[idx]=i.key.length(); // using buffer overflow
        idx +=HEADER_SIZE_BYTES;
        outputSize +=i.key.length()+HEADER_SIZE_BYTES;
        output = (byte*)reallocSafe(output,sizeof(byte)*outputSize);

        for(auto c :i.key){
            output[idx++]=c;
        }
    
        // idx +=1; adding encoding bit
        
        // outputSize+=1;
        
        
        
        clearNextBytes(output+idx,sizeof(byte)*HEADER_SIZE_BYTES);
        
        
        output[idx]=i.value.length(); // using buffer overflow
        
        
        idx +=HEADER_SIZE_BYTES;
        
        
        outputSize +=i.value.length()+HEADER_SIZE_BYTES;
        
        output = (byte*)reallocSafe(output,sizeof(byte)*outputSize);
        
        
        for(auto c :i.value){
            output[idx++]=c;
        }
    }
    
    outputSize -= HEADER_SIZE_BYTES;
    
    output = (byte*)reallocSafe(output,sizeof(byte)*outputSize);
    
    *(unsigned long*)output = outputSize;
    
    return output;
}

void SnedCMD(Hashtable& data,unsigned long& outputSize,unsigned long dataSize){// TODO fix this function catching after 4 bytes
    SendAT(ENTER_DATA_MODE_CMD,0); // returnes ">"
    
    
    // dbg((String)"sending "+data["type"]);
    byte* dataBytes = buildData(data);
    if (dataSize ==0 ){
        if (*(unsigned long*)dataBytes >RAMTOTAL){
            dbg((String)"data is too big "+*(unsigned long*)dataBytes);
            return;
        }
        else
            dataSize = *(unsigned long*)dataBytes;
    }
    dbg("sending command");
    SendATArr((char*)dataBytes,dataSize,0);
    byte* output = SendAT((0x1a),outputSize,1000);// because the modem doesn't wait for the server aknowlagment it should respond instantly
    free(output);

    
}
void RegisterToServer(){

    dbg("registering to server");
    Hashtable test;
    test["type"]="REG";
    char* dataBytes = (char*)buildData(test);
    unsigned long outputSize;
    int n = 5;
    SnedCMD(test,outputSize);
    sleep(1000);
            dbg("waiting for data");
            fixATchar('0',4);
            byte* output = GetATResponse(outputSize,10000);//10 sec timeout
            fixATchar('0',0);
            if (output == nullptr){
                dbg("error in sendATArr");
                stopProgram();
            }

    dbg((int)outputSize);
    for (int i=0;i<(int)outputSize;i++){
         dbg((char)output[i],0);
         dbg("    ",0);
        dbg(output[i]);
    }
       
    test.~Hashtable();
    dbg(0[output]);
    // test = parseData(output);
    // for(auto i: test){
    //     dbg(i.key,0);
    //     dbg(": ",0);
    //     dbg(i.value);
    // }
    free(output);

}
void ConnectToServer();
void StartConnectionToServer();