#pragma once
#define ARDUINO
#include <Vector.h>
#include <Arduino.h>
class Hashtable{
    struct Pair{
        String key;
        String value;
    };
    Pair* storage[10];
    Vector<Pair*> container;
    public:
    Pair& operator[](const String str);
    inline VectorIterator<Pair*> begin() noexcept { return container.begin(); }
    inline VectorIterator<Pair*> end() noexcept { return container.end(); }
    Hashtable(int _size);
};
