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

    byte* output = (byte*)calloc(sizeof(char)*HEADER_SIZE_BYTES,2);

    unsigned int outputSize=sizeof(char)*HEADER_SIZE_BYTES*2;

    unsigned int idx=HEADER_SIZE_BYTES;
    
    for (byte p=0;p<data.getSize();++p){
        auto i = data.get(p);
        clearNextBytes(output+idx,sizeof(byte)*HEADER_SIZE_BYTES);

        output[idx]=i->key.length(); // using buffer overflow
  
        idx +=HEADER_SIZE_BYTES;

        outputSize +=i->key.length()+HEADER_SIZE_BYTES;
  
        output = (byte*)reallocSafe(output,sizeof(byte)*outputSize);


        for(auto c :i->key){
            output[idx++]=c;
        }

    
        // idx +=1; adding encoding bit
        
        // outputSize+=1;
        
        
        
        clearNextBytes(output+idx,sizeof(byte)*HEADER_SIZE_BYTES);
        
        
        output[idx]=i->valueLength; // using buffer overflow
        
        
        idx +=HEADER_SIZE_BYTES;
        
        
        outputSize +=i->valueLength+HEADER_SIZE_BYTES;
        
        output = (byte*)reallocSafe(output,sizeof(byte)*outputSize);
        
        
        for(size_t x=0;x<i->valueLength;++x){
            output[idx++]=i->value[x];
        }
        
    }
    checkMemory(50);
    outputSize -= HEADER_SIZE_BYTES;
    checkMemory(50);
    output = (byte*)reallocSafe(output,sizeof(byte)*outputSize);
    checkMemory(50);
    *(unsigned long*)output = outputSize;
    checkMemory(50);
    return output;
}

void SnedCMD(Hashtable& data){
    //send raw data to the modem

    byte* dataBytes = buildData(data);
    unsigned long dataSize=0;
    //get the bytes array size by how its built
    for (byte i=0;i<HEADER_SIZE_BYTES;i++){
        dataSize+=dataBytes[i]<<8*i;
    }
    
    if (dataSize ==0 ){
        if (*(unsigned long*)dataBytes >RAMTOTAL){
            dbg((String)"data is too big "+*(unsigned long*)dataBytes);
            return;
        }
        else
            dataSize = *(unsigned long*)dataBytes;
    }
    dbg("checking modem response");
    if (!waitForATResponse(10000)){
        dbg("modem did not respond");
        return;
    }
    StartDataSend(dataSize);
    
    // dbg((String)"sending "+data["type"]);
    
    dbg("sending command with size:",0);
    dbg(dataSize);
    // printArr(dataBytes,dataSize);
    SendATArr((char*)dataBytes,dataSize,0);

    byte* output = StopDataSend();// because the modem doesn't wait for the server aknowlagment it should respond instantly
    free(output);// to not lose the pointer withput freeing it

    
}

bool RegisterToServer(){
    for (byte tries=0;tries<5;++tries){
        dbg("registering to server");
    Hashtable test;
    test.set("type","REG",3);
    size_t n;
    // dbg(test.get("type",n));
    // dbg(n);
    

    SnedCMD(test);
    dbg("waiting for data");
    sleep(200);
    unsigned long outputSize=-1;
    byte* output = waitForServerResponse(outputSize,10000);//10 sec timeout
    if (output == nullptr){
        dbg("error in sendATArr");
        return false;
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
    size_t valLen=0;
    char* val =(char*)outputPackage.get("uuid",valLen);
    test.set("uuid",val,valLen);
    val =(char*)outputPackage.get("password",valLen);
    free(output);
    test.set("password",val,valLen);
    // test.set("password","hello",5);
    // val =getAvailablePins(*((byte*)&valLen));
    // char arr[] = {1,2,3,4,5,6,7,8,9,10,11,12,13};
    // test.set("availablepins","abc",3);
    SnedCMD(test);
    dbg("waiting for data");
    
    // output = waitForServerResponse(outputSize,10000);//10 sec timeout
    // outputPackage = dataPackage(output,outputSize);
    // val = (char*)outputPackage.get("code",valLen);
    // dbg("got code ",0);
    // dbg(val,1,valLen);
    
    break; // if got here it didn't fail
    }
    
    }
    
    return true;

}
void StartConnectionToServer();