#include "actionsService.h"
#include <dataPackage.h>

void registerClient(){

}
void loginClient(){

}
    String getValue(byte* data,int& CurrentIdx){
    size_t currSize =getSizeFromHeader(data);
        //get the size of the value
        // cout<<currSize;
        
        // data++; //skip the encode bit for now
        //read the value
        String output = "";
        for(long _=0;_<currSize;_++){
            output +=(char)*data;
            ++data;
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
        
        
        output[idx]=i.valueLength; // using buffer overflow
        
        
        idx +=HEADER_SIZE_BYTES;
        
        
        outputSize +=i.valueLength+HEADER_SIZE_BYTES;
        
        output = (byte*)reallocSafe(output,sizeof(byte)*outputSize);
        
        
        for(size_t x=0;x<i.valueLength;++x){
            output[idx++]=i.value[x];
        }
    }
    
    outputSize -= HEADER_SIZE_BYTES;
    
    output = (byte*)reallocSafe(output,sizeof(byte)*outputSize);
    
    *(unsigned long*)output = outputSize;
    
    return output;
}

void SnedCMD(Hashtable& data,unsigned long dataSize){
    //send raw data to the modem
    StartDataSend(dataSize);
    
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
    byte* output = StopDataSend();// because the modem doesn't wait for the server aknowlagment it should respond instantly
    free(output);// to not lose the pointer withput freeing it

    
}

void RegisterToServer(){

    dbg("registering to server");
    Hashtable test;
    test.set("type","REG",3);
    char* dataBytes = (char*)buildData(test);
    unsigned long outputSize=0;

    //get the bytes array size by how its built
    for (byte i=0;i<HEADER_SIZE_BYTES;i++){
        outputSize+=dataBytes[i]<<8*i;
    }

    SnedCMD(test,outputSize);
    dbg("waiting for data");
    sleep(200);
    byte* output = waitForServerResponse(outputSize,10000);//10 sec timeout
    if (output == nullptr){
        dbg("error in sendATArr");
        stopProgram();
    }
    
    
    
    
    // StartDataSend(outputSize);
    // SendATArr((char*)output,outputSize);
    // StopDataSend();

    bool verified = confirmDataSize(output,outputSize);
    dbg("verified:",0);
    dbg(verified);
    if (verified){
        

    dataPackage outputPackage(output,outputSize);
    
    outputPackage.printPackage();

    //sending reg back with uuid and password
    size_t uuidLen=0;
    char* uuid =(char*)outputPackage.get("uuid",uuidLen);
    test.set("uuid",uuid,uuidLen);
    uuid =(char*)outputPackage.get("password",uuidLen);
    test.set("password",uuid,uuidLen);
    SnedCMD(test,)
    }
    
    free(output);

}
void ConnectToServer();
void StartConnectionToServer();