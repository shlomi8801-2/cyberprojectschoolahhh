#include <Hashtable.h>
#include <client.h>

    String& Hashtable::operator[](const String str){
        for(auto i=0;i<container.size();i++){
            if(container[i]->key.equals(str)){
                return container[i]->value;
            }
        }
        Pair* tmp = (Pair*)calloc(sizeof(Pair),1);
        if (tmp == nullptr){
            dbg("unable to allocate memory for hashtable!");
            stopProgram();
        }
        tmp->key=str;
        container.add(tmp);
        return tmp->value;
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


