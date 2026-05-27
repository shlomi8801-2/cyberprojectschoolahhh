#pragma once
#define ARDUINO
#include <linkedlist.h>
#include <Arduino.h>

class Hashtable{
    struct Pair{
        String key;
        char* value;
        size_t valueLength;
    };
    LinkedList<Pair*> container;
    public:
    void set(String str,char* val,const size_t length);
    char* get(const String str,size_t &outputSize);
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
inline Iterator end(){ return Iterator(length(),&container);}
    // inline Iteator<Pair> begin() noexcept { return container.begin(); }
    // inline VectorIterator<Pair> end() noexcept { return container.end(); }
    Hashtable();
    ~Hashtable();
    const String ToString();
    
};
