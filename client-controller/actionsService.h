#pragma once
#include "modemCore.h"
#include <Hashtable.h>
void registerClient();
void loginClient();

String getValue(byte* data);   
byte* ToByteArr(String data,int* OutputSize);

Hashtable parseData(byte data[],long size);
byte* buildData(Hashtable data,int* outputSize);