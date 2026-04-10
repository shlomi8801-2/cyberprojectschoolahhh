#pragma once
#include "client.h" // for arduino stdlib mostly
#include <string.h>
class dict{
    //less efficient hashmap for usage in arduino
    int size=10;
    String* keys;
    String* values;
    public:
    String* operator[](const String key);
    char indexof(const String key);

};
