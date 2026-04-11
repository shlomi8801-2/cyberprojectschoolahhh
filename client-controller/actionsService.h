#pragma once
#include "modemCore.h"
#include <Hashtable.h>
void registerClient();
void loginClient();
Hashtable<String,String> parseData(String data);
String buildData(Hashtable<String,String> data);
