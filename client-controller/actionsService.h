#pragma once
#include "modemCore.h"
#include "dict.h"
void registerClient();
void loginClient();
dict parseData(String data);
String buildData(dict data);
