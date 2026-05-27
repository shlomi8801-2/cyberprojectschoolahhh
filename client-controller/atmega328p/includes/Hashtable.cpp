#include <Hashtable.h>
#include <client.h>

    
        void Hashtable::set(const char* str,char* val,const size_t length){
        for(auto i=0;i<container.size();i++){
            if(container[i]->key.equals(str)){
                char* tmp = (char*)malloc(length*sizeof(char));
                memcpy(tmp,val,length);
                container[i]->value = tmp;
                container[i]->valueLength=length;
            }
        }
        
        Pair* tmpPair = (Pair*)calloc(sizeof(Pair),1);
        if (tmpPair == nullptr){
            dbg("unable to allocate memory for hashtable!");
            stopProgram();
        }
        tmpPair->key=str;
        container.add(tmpPair);
        char* tmp = (char*)malloc(length*sizeof(char));
        memcpy(tmp,val,length);
        tmpPair->value = tmp;
        tmpPair->valueLength=length;
        
    }
    char* Hashtable::get(const String str,size_t &outputSize){
        for(auto i=0;i<container.size();i++){
            if(container[i]->key.equals(str)){
                outputSize = container[i]->valueLength;
                return container[i]->value;
            }
        }
    }
    Hashtable::Hashtable() {}
        const String Hashtable::ToString(){
            String output = "{\n";
                    // for(auto i=0;i<container.size();i++){

                // output +="\t"+container[i]->key+":\""+container[i]->value+"\",\n";
                // output +="\t"+(String)(int)&(container[i]->key)+":\""+(String)(int)&(container[i]->value)+"\",\n";

            // }
            for (auto i:*this){
                output += "\t";
                output+=i.key;
                output += ":\"";
                output+=i.value;
                output += "\",\n";
            }
            output +="}";
            return output;
        }
    Hashtable::~Hashtable(){
        for(auto i=0;i<container.size();i++){
            free(this->container[i]);
        }
    }


