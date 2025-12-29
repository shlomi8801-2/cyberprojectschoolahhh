import base64
import random
import log
import time
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
def buildWhereQuery(args:dict,algo:str="exact") -> str | None:
    output = ""
    if (algo=="exact"):
        output = " AND ".join([f'{x[0]}=="{x[1]}"' for x in args.items() if x[1] != None])
    elif(algo=="like"):
        output = " AND ".join([f'{x[0]} like "%{x[1]}%"' for x in args.items() if x[1] != None])
    elif(algo=="contains"):
        output = " AND ".join([f'contains({x[0]},"{x[1]}")' for x in args.items() if x[1] != None])
    else:
        log.log(f"searchUser went wrong no algo:{algo}")
        return None
    return output