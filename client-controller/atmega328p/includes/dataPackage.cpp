#include <dataPackage.h>

byte* dataPackage::get(const String key,size_t &outputSize){
    //returns nullptr if not found
    for(int i =HEADER_SIZE_BYTES;i<this->size;){
        size_t currSize =getSizeFromHeader(this->data+i); //<header><data>
        i+=HEADER_SIZE_BYTES;//<data>
        if(strncmp((char*)(this->data+i),key.begin(),currSize)==0){
            outputSize=getSizeFromHeader(data+i+currSize);
            return (this->data)+i+currSize+HEADER_SIZE_BYTES;
        }
        i+=currSize;
    }
    outputSize=0;
    return nullptr;
}
void dataPackage::printPackage(){
    for(size_t i =HEADER_SIZE_BYTES;i<this->size;){
        size_t currSize =getSizeFromHeader(this->data+i); //<header><data>
        i+=HEADER_SIZE_BYTES;//<data>
        for (byte x=0;x<currSize;++x)
        dbg(*(char*)(this->data+i+x),0);
        dbg(" : ",0);
        size_t outputSize=getSizeFromHeader(data+i+currSize);
        for (byte x=0;x<outputSize;++x)
        dbg(*(char*)((this->data)+i+currSize+HEADER_SIZE_BYTES+x),0);
        dbg(",");
        i+=currSize+HEADER_SIZE_BYTES+outputSize;
    }
}