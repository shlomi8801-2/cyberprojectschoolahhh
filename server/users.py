import database
from log import log
import hashlib
import base64
import time
import random

HASHINGALGO = hashlib.sha512
REMEMBERTOKENTIME = 5*60 # 5 mins
#tables: Users, Carmodules
USERS_TABLE = ["Users",{"username":"varchar(255) not null UNIQUE","password":"varchar(128) not null","token":"varchar(30)","token_date":"INT DEFAULT 0","date_created":"INT DEFAULT 0"}]
CARMODULES_TABLE = "Carmodules"
def getNowEpoc():
    now = time.time()
    return round(now)
def encodeUsername(username:str)->str:
    return base64.b64encode(username.encode("ascii")).decode("ascii")
def decodeUsername(username:str)->str:
    return base64.b64decode(username.encode("ascii")).decode("ascii")
def hashPassword(password:str)->str:
    return HASHINGALGO(password.encode()).hexdigest()

def deleteUser(username:str=None,token:str=None,algo:str="exact")->bool:
    args =searchUser(username,token,algo)
    if args == None:
        return False
    if  len(args) !=1:
        return False
    args = makedict(args[0])
    if (algo=="exact"):
        args = " AND ".join([f'{x[0]}=="{x[1]}"' for x in args.items() if x[1] != None])
    elif(algo=="like"):
        args = " AND ".join([f'{x[0]} like "%{x[1]}%"' for x in args.items() if x[1] != None])
    elif(algo=="contains"):
        args = " AND ".join([f'contains({x[0]},"{x[1]}")' for x in args.items() if x[1] != None])
    else:
        log.log(f"searchUser went wrong no algo:{algo}")
        return None
    database.Delete(USERS_TABLE[0],args)
    return True

def addUser(username:str,password:str)->bool:
    database.AddTable(*USERS_TABLE)
    return database.Insert(USERS_TABLE[0],{"username":encodeUsername(username),"password":hashPassword(password),"date_created":getNowEpoc(),"token":""})

def searchUser(username:str=None,token:str=None,algo:str="exact")->str:
    """algo is [exact,like,contains]"""
    # database.AddTable(*USERS_TABLE)
    args = {"username":encodeUsername(username),"token":token}
    if (algo=="exact"):
        args = " AND ".join([f'{x[0]}=="{x[1]}"' for x in args.items() if x[1] != None])
    elif(algo=="like"):
        args = " AND ".join([f'{x[0]} like "%{x[1]}%"' for x in args.items() if x[1] != None])
    elif(algo=="contains"):
        args = " AND ".join([f'contains({x[0]},"{x[1]}")' for x in args.items() if x[1] != None])
    else:
        log.log(f"searchUser went wrong no algo:{algo}")
        return None
    try:
        return database.Search(USERS_TABLE[0],args)
    except Exception as e:
        log(f"something went wrong while searching: {e}")
        return None
def makedict(user:tuple)->dict:
    """gets a user tuple and assigns the data to dict with its columns in the table"""
    if not isinstance(user,tuple):
        return None
    return dict((list(USERS_TABLE[1].keys())[x], user[x+1]) for x in range(len(user)-1))
def updateUser(values:dict,args:dict,algo:str="exact")->bool:
    """args should look like the makedict output for example: {"username":username,"token":token}"""
    if (algo=="exact"):
        args = " AND ".join([f'{x[0]}=="{x[1]}"' for x in args.items() if x[1] != None])
    elif(algo=="like"):
        args = " AND ".join([f'{x[0]} like "%{x[1]}%"' for x in args.items() if x[1] != None])
    elif(algo=="contains"):
        args = " AND ".join([f'contains({x[0]},"{x[1]}")' for x in args.items() if x[1] != None])
    else:
        log.log(f"searchUser went wrong no algo:{algo}")
    return database.Update(USERS_TABLE[0],values,args)
    pass
def generateToken()->str:
    return ''.join(chr(random.randint(0,255)) for _ in range(30))


#the more simple functions
def login(username:str,password:str)->str:
    """gets username and password returnes token or none"""
    res = searchUser(username)
    if (res == None):
        return None
    if (len(res)!=1):
        return None
    res = makedict(res[0])
    if (res["password"] == hashPassword(password)):
        if (getNowEpoc()-int(res["token_date"]) <REMEMBERTOKENTIME):
            #update the token-date
            #return the old token
            updateUser({"token_date":getNowEpoc()},res)
            return res["token"]            
        else:
            #generate new token
            #update the token-date
            #return the new token
            token = generateToken()
            updateUser({"token_date":getNowEpoc(),"token":token},res)
            return token
def register(username:str,password:str)->str:
    """gets username and password returnes token or none"""
    res = searchUser(username)
    
    if (res != None and len(res)>=1): # if there is a user with that username
        return None
    addUser(username,password)
    return login(username,password)


if (__name__=="__main__"):
    deleteUser("shlomi")
    register("shlomi","1234")
    print(login("shlomi","1234"))
    pass
    