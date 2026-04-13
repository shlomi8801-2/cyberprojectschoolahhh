#pragma once
#define ARDUINO
#include <Vector.h>
#include <Arduino.h>

class Hashtable{
    struct Pair{
        String key;
        String value;
    };
    unsigned int StorageSize=10;
    Pair** storage;
    Vector<Pair*> container;
    public:
    String& operator[](const String str);

    inline VectorIterator<Pair*> begin() noexcept { return container.begin(); }
    inline VectorIterator<Pair*> end() noexcept { return container.end(); }
    Hashtable(int _size);
    const String ToString();
};
