#include "dict.h"

String* dict::operator[](const String key){
        int i = this->indexof(key);
        return i !=-1 ?&values[i]:&((String)"");
    }
char dict::indexof(const String key){
    for (int i=0;i<size;i++){
        if (keys[i].compareTo(key) == 0){
            return i;
        }
    }
    return -1;
}