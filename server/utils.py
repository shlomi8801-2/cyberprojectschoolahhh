import base64
import random
import log
import time
import threading
import json
from constants import *

def getNowEpoc():
    now = time.time()
    return round(now)
def encodeUsername(username:str)->str:
    if username == None:
        return ""
    return base64.b64encode(username.encode("ascii")).decode("ascii")
def decodeUsername(username:str)->str:
    if username == None:
        return ""
    return base64.b64decode(username.encode("ascii")).decode("ascii")
def hashString(string:str)->str:
    if string == None:
        return ""
    return HASHINGALGO(string.encode()).hexdigest()
def generateToken(length:int = 30)->str:
    allowedChars = [x for x in "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890"] #must be allowed inside a browser cookie
    return ''.join(random.choice(allowedChars) for _ in range(length))
def buildWhereQuery(args:dict,algo:str="exact") -> str :
    # filters = {} # in case del function casuse problems use it
    # for key in args:
    #     if not (args[key] == "" or args[key] == None):
    #         filters[key] = args[key]
    # args = filters
    
    for key in list(args.keys()):
        if (args[key] == "" or args[key] == None):
            del args[key]

    if (len(args)==0):
        log.log("warning: args must have items")
        return "1=1" #for all used like "... where 1=1" instead of "... where "
    output = ""
    if (algo=="exact"):
        output = " AND ".join([f'{x[0]}=="{x[1]}"' for x in args.items() if x[1] != None])
    elif(algo=="like"):
        output = " AND ".join([f'{x[0]} like "%{x[1]}%"' for x in args.items() if x[1] != None])
    elif(algo=="contains"):
        output = " AND ".join([f'contains({x[0]},"{x[1]}")' for x in args.items() if x[1] != None])
    else:
        log.log(f"searchUser went wrong no algo:{algo}")
        return "1=1"
    return output
def makeSqlDict(data:tuple,table:list)->dict:
    """gets a tuple and assigns the data to dict with its columns in the table"""
    #for sqlite it appends another item before so skip it before sending into this function - fixed in the database script
    if not isinstance(data,tuple):
        return {}
    keys = list(table[1].keys())
    return dict((keys[x], data[x]) for x in range(len(data)))
def makeThreadAndStart(target,args:list=[]):
    tmp = threading.Thread(target=target,args=args)
    tmp.start()
    return tmp
def removeFromSqlList(List:list,columnsToRemove:tuple,Table:list):
    """returns the keys left in the table"""
    #its low numbers here for the lengths so it matters less what is the complexity
    indexesToRemove =[]
    keys = list(Table[1].keys())
    for key in keys:
        if (key in columnsToRemove):
            indexesToRemove.append(keys.index(key))
    for i in range(len(List)):
        List[i] = [List[i][x] for x in range(len(keys)) if x not in indexesToRemove] # the length of keys and every "row" in usersList are the same(if the function is used correctly)
    keys = [keys[x] for x in range(len(keys)) if not(x in indexesToRemove)]
    return keys
def dictFromJson(text:str)->dict:
    return json.loads(text)
def checkSqlInjection(text:str)->bool:
    """if found some type of sql injection(problematic string that might be) returnes True else False"""
    problematicStrings = ["--","'",'"',""]
    for x in problematicStrings:
        if x in text:
            return True
    return False