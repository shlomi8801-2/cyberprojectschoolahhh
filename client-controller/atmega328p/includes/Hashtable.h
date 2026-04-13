#pragma once
#define ARDUINO
#include <linkedlist.h>
#include <Arduino.h>

class Hashtable{
    struct Pair{
        String key;
        String value;
    };
    LinkedList<Pair*> container;
    public:
    String& operator[](const String str);

    // inline Iteator<Pair> begin() noexcept { return container.begin(); }
    // inline VectorIterator<Pair> end() noexcept { return container.end(); }
    Hashtable();
    const String ToString();
    
};
