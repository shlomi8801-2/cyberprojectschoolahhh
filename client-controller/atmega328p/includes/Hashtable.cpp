#include <Hashtable.h>

    String& Hashtable::operator[](const String str){
        for(auto i:container){
            if(i->key.equals(str)){
                return i->value;
            }
        }
        Pair* tmp = (Pair*)malloc(sizeof(Pair));
        tmp->key=str;
        container.push_back(tmp);
        return tmp->value;
    }
    Hashtable::Hashtable(int _size=10) {
        storage = (Pair**)malloc(sizeof(Pair*)*_size);
        container.setStorage(storage,this->StorageSize,0);
    }
        const String Hashtable::ToString(){
            String output = "{\n";
            for(auto i:*this){
                output +="\t"+i->key+":\""+i->value+"\",\n";
            }
            output +="}";
            return output;
        }



