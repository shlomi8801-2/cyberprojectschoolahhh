#pragma once
#include "modemCore.h"
#include <Hashtable.h>
void registerClient();
void loginClient();

String getValue(byte* data);   
byte* ToByteArr(String data,int* OutputSize);

Hashtable<String,String> parseData(byte data[],long size);
byte* buildData(Hashtable<String,String> data,int* outputSize);