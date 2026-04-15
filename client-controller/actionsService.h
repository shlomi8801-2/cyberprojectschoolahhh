#pragma once
#include "modemCore.h"
#include <Hashtable.h>
void registerClient();
void loginClient();

String getValue(byte* data);   
byte* ToByteArr(String data,int* OutputSize);

Hashtable parseData(byte* data,long size);
byte* buildData(Hashtable data);
void RegisterToServer();
void ConnectToServer();
void StartConnectionToServer();
inline void clearNextBytes(void* data,int n){
    for (;n>0;){ ((char*)data)[--n]=0;}
    return;
}