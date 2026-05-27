#include "actionsService.h"
#include <dataPackage.h>
#include <eeprom/EEPROM.h>

bool loginClient(){
    Hashtable test;
    test.set("type","CON",3);
    char* uuid = getUuidFromeMem();
    char* psd = getPsdFromeMem();
    test.set("uuid",uuid,uuid_LENGTH);
    test.set("psd",psd,psd_LENGTH);
    free(uuid);//not used anymore and has been copied
    free(psd);
    dbg(F("trying to connect to server with"));
    dbg(uuid,1,uuid_LENGTH);
    dbg(psd,1,psd_LENGTH);
    SnedCMD(test);
    unsigned long outputSize =0;
    byte* res = waitForServerResponse(outputSize,5000);
    dataPackage resPackage(res,outputSize);
    resPackage.printPackage();
    byte* code = resPackage.get("code",*(size_t*)&outputSize);
    if(code != nullptr){
        switch (code[0])
        {
        case '0':
            //logged in
            dbg(F("logged in!"));
            return 1;
        case '1':
            return 0;
        default:
            return 0;
            break;
        }
    }
    
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
    outputSize -= HEADER_SIZE_BYTES;
    output = (byte*)reallocSafe(output,sizeof(byte)*outputSize);
    *(unsigned long*)output = outputSize;
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
    if (dataSize>=127){
        //between arduino and modem
        dbg(F("sending more then 127 chars may fail"));
    }
    
    if (dataSize ==0 ){
        if (*(unsigned long*)dataBytes >RAMTOTAL){
            dbg((String)"data is too big "+*(unsigned long*)dataBytes);
            return;
        }
        else
            dataSize = *(unsigned long*)dataBytes;
    }
    dbg(F("checking modem response"));
    if (!waitForATResponse(10000)){
        dbg(F("modem did not respond"));
        return;
    }
    StartDataSend(dataSize);
    
    // dbg((String)"sending "+data["type"]);
    
    dbg(F("sending command with size:"),0);
    dbg(dataSize);
    // printArr(dataBytes,dataSize);
    SendATArr((char*)dataBytes,dataSize,0);
    free(dataBytes);
    byte* output = StopDataSend();// because the modem doesn't wait for the server aknowlagment it should respond instantly
    free(output);// to not lose the pointer withput freeing it
    
    
}
char* getUuidFromeMem(){
    char* output = (char*)calloc(sizeof(char)*uuid_LENGTH,1);
    for (byte x=0;x<uuid_LENGTH;++x)
        output[x] = (char)EEPROM.read(uuid_ADDR+x);
    return output;
}
char* getPsdFromeMem(){
    //get password
    char* output = (char*)calloc(sizeof(char)*psd_LENGTH,1);
    for (byte x=0;x<psd_LENGTH;++x)
        output[x] = (char)EEPROM.read(psd_ADDR+x);
    return output;
}
bool RegisterToServer(){
    for (byte tries=0;tries<5;++tries){
        dbg(F("registering to server"));
    Hashtable test;
    test.set(F("type"),"REG",3);
    size_t n;
    // dbg(test.get("type",n));
    // dbg(n);
    

    SnedCMD(test);
    dbg(F("waiting for data"));
    sleep(200);
    unsigned long outputSize=-1;
    byte* output = waitForServerResponse(outputSize,2000);//2 sec timeout
    if (output == nullptr){
        dbg(F("error in sendATArr"));
        return false;
    }
    
    
    
    
    // StartDataSend(outputSize);
    // SendATArr((char*)output,outputSize);
    // StopDataSend();

    bool verified = confirmDataSize(output,outputSize);
    dbg(F("verified:"),0);
    dbg(verified);
    if (verified){
        dataPackage outputPackage(output,outputSize);
        
        outputPackage.printPackage();
        //sending reg back with uuid and password
        size_t valLen=0;

        //getting and saving uuid
        char* val =(char*)outputPackage.get(F("uuid"),valLen);
        if (abs(valLen-uuid_LENGTH)>5){ dbg(F("difference of lengths for uuid is bigger then 5!"));stopProgram();}
        
        dbg(F("writing to eeprom uuid:"),0);
        dbg(val,1,uuid_LENGTH);
        for( byte x=0;x<uuid_LENGTH;++x)
            EEPROM.write(uuid_ADDR+x,val[x]);
        test.set(F("uuid"),val,valLen);
        //getting and saving psd
        val =(char*)outputPackage.get("psd",valLen);

        if (abs(valLen-psd_LENGTH)>5){ dbg(F("difference of lengths for psd is bigger then 5!"));stopProgram();}
        dbg(F("writing to eeprom psd:"),0);
        dbg(val,1,psd_LENGTH);
        for( byte x=0;x<psd_LENGTH;++x)
            EEPROM.write(psd_ADDR+x,val[x]);
        test.set(F("psd"),val,valLen);
        free(output);//the moment you stop using it
        
        SnedCMD(test);
        dbg(F("waiting for data"));

        output = waitForServerResponse(outputSize,10000);//10 sec timeout
        outputPackage = dataPackage(output,outputSize);
        outputPackage.printPackage();
        val = (char*)outputPackage.get(F("code"),valLen);
        
        char code = valLen>0 ? val[0]:'E';
        free(output);
        dbg(F("got code "),0);
        dbg(code);
        switch (code)
        {
        case '0':{
            //no errors
            dbg(F("registered successfully!"));
            char* uuid = getUuidFromeMem();
            dbg(F("uuid:"),0);
            dbg(uuid,1,uuid_LENGTH);
            free(uuid);
            char* psd = getPsdFromeMem();
            dbg(F("password:"),0);
            dbg(psd,1,psd_LENGTH);
            free(psd);
            
            return true; // if got here it didn't fail
        }
        
        case '1':
        //id not found on the server - reregister
        continue;
        case '2':
        //passwords did not match - reregister
            continue;
        case 'E':
            dbg(F("no code provided by the server after registering!"));
            stopProgram();
        default:
            continue;
        }
        
        }
    //package came back corrupted 
    }
    return false;

}
void StartConnectionToServer();