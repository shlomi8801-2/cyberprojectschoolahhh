#pragma once
#include "modemCore.h"
#include <Hashtable.h>
void registerClient();
void loginClient();

String getValue(byte* data,int& CurrentIdx);   
byte* ToByteArr(String data,int* OutputSize);

Hashtable parseData(byte* data);
byte* buildData(Hashtable data);
void RegisterToServer();
void ConnectToServer();
void StartConnectionToServer();
inline void clearNextBytes(void* data,int n){
    for (;n>0;){ ((char*)data)[--n]=0;}
    return;
}