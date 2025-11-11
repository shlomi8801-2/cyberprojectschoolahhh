import json
from log import log
import commentjson #if the file doesnt contain comments then import making this module optional


#can implament a caching system using the hash of the settings file but its a small file so it doesnt matter for now

cache = {} #currently if the file changes while the code is running it will not update the cache
def GetSetting(key =None)->list | str | None | dict:
    '''
    if key is given gives value if not gives a list of tree of keys in settings file
    key can look like this "dir1.dir2.val" for json {"dir1": {"dir2": {"val":5}}}
    '''
    #if key is given gives value if not gives a list of tree of keys in settings file
    if key in cache:
        cache[key]
    try:
        with open(GetCurrentDir()+"/settings.json","r") as f:
            jf = NormalizeJson(f.read())
            if (not key):
                return list(jf.keys())
            else:
                output = jf
                for x in key.split("."):
                    if x in output:
                        output = output[x]
                    else:
                        log(f"key {x} was not found!")
                        return None
                cache[key] = output
                return output
    except FileNotFoundError:
        log("settings.json file not found")
        return None
    except Exception as e:
        print(e)

def GetCurrentDir()->str:
    """returns the dir of the server.py file"""
    return "/".join(__file__.split("/")[:-1])

def NormalizeJson(string:str)->dict: #removes the comments from there for json module to load it
    """loads a json file and returns a dict object of the contents"""
    try: 
        return dict(json.loads(string))
    except:
        return dict(commentjson.loads(string))
    