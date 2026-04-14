#include <Hashtable.h>

    String& Hashtable::operator[](const String str){

        for(auto i=0;i<container.size();i++){
            if(container[i]->key.equals(str)){
                return container[i]->value;
            }
        }
        Pair* tmp = (Pair*)malloc(sizeof(Pair));
        if (tmp == nullptr){
            
        }
        tmp->key=str;
        container.add(tmp);
        return tmp->value;
    }
    Hashtable::Hashtable() {}
        const String Hashtable::ToString(){
            String output = "{\n";
                    for(auto i=0;i<container.size();i++){

                output +="\t"+container[i]->key+":\""+container[i]->value+"\",\n";
                output +="\t"+(String)(int)&(container[i]->key)+":\""+(String)(int)&(container[i]->value)+"\",\n";

            }
            output +="}";
            return output;
        }



