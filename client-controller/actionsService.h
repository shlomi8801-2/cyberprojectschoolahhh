#pragma once
#include "modemCore.h"
#include <Hashtable.h>
void registerClient();
void loginClient();

String getValue(byte* data);   
byte* ToByteArr(String data,int* OutputSize);

Hashtable parseData(byte* data,int size);
byte* buildData(Hashtable data);
void RegisterToServer();
void ConnectToServer();
void StartConnectionToServer();