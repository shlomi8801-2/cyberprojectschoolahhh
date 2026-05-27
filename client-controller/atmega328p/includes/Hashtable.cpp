#include <Hashtable.h>
#include <client.h>

    
        void Hashtable::set(String str,const char* val,const size_t length){
        for(auto i=0;i<container.size();i++){
            // dbg(container[i]->key);
            // dbg(str);
            // dbg(container[i]->key.equals(str));
            if(container[i]->key.equals(str)){
                char* tmp = (char*)malloc(length*sizeof(char));
                memcpy(tmp,val,length);
                free(container[i]->value);
                container[i]->value = tmp;
                container[i]->valueLength=length;
                return;
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
            for (byte p=0;p<getSize();++p){
                auto i = get(p);
                output += "\t";
                output+=i->key;
                output += ":\"";
                output+="..."; // since the change to char* with length instead of just String object this is disabled
                output+=(String)i->valueLength;
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


