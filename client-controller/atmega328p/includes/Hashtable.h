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
    inline unsigned int length() {return container.size();}
    class Iterator{
        unsigned int idx;
        LinkedList<Pair*>* container;
        public:
        Iterator(unsigned int _idx,LinkedList<Pair*>* _container): idx(_idx),container(_container){}
        inline void operator++(){
        idx++;
        }
        inline bool operator!=(const Iterator& B){return this->idx != B.idx;}
        inline Pair const &  operator*(){return *(container->get(idx));}

    };
    inline Iterator begin(){ return Iterator(0,&container);}
inline Iterator end(){ return Iterator(length()-1,&container);}
    // inline Iteator<Pair> begin() noexcept { return container.begin(); }
    // inline VectorIterator<Pair> end() noexcept { return container.end(); }
    Hashtable();
    const String ToString();
    
};
