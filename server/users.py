import database
from log import log
import hashlib

HASHINGALGO = hashlib.sha512
#tables: Users, Carmodules
USERS_TABLE = ["Users",{"username":"varchar(255) not null","password":"varchar(255) not null","token":"varchar(128)"}]
CARMODULES_TABLE = "Carmodules"
def deleteUser(username:str=None,token:str=None,algo:str="exact")->bool:
    args =searchUser(username,token,algo)
    if args == None:
        return False
    if  len(args) !=1:
        return False
    args = {list(USERS_TABLE[1].keys())[x]: args[0][x+1] for x in range(len(args))}
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
    return database.Insert(USERS_TABLE[0],{"username":username,"password":HASHINGALGO(password.encode()).hexdigest(),"token":""})

def searchUser(username:str=None,token:str=None,algo:str="exact")->str:
    """algo is [exact,like,contains]"""
    # database.AddTable(*USERS_TABLE)
    args = {"username":username,"token":token}
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
if (__name__=="__main__"):
    deleteUser("shlomi")
    addUser("shlomi","pass")
    print(searchUser("shlomi"))
    deleteUser("shlomi")
    print(searchUser("shlomi"))
