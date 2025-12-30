import base64
import random
import log
import time
import threading
import settings
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
    print(f"{keys}\n{data}")
    return dict((keys[x], data[x]) for x in range(len(data)))
def makeThreadAndStart(target,args:list=[]):
    tmp = threading.Thread(target=target,args=args)
    tmp.start()
    return tmp