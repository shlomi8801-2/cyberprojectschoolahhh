#pragma once
#include "modemCore.h"
#include <Hashtable.h>
#include <dataPackage.h>

char* getUuidFromeMem();
char* getPsdFromeMem();
bool loginClient();
String getValue(byte* data,int& CurrentIdx);   
byte* ToByteArr(String data,int* OutputSize);

byte* buildData(Hashtable& data);
void SnedCMD(Hashtable& data);
bool RegisterToServer();
void StartConnectionToServer();
inline void clearNextBytes(void* data,int n){
    for (;n>0;){ ((char*)data)[--n]=0;}
    return;
}