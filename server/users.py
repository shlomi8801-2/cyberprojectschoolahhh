import database
import log
import hashlib

HASHINGALGO = hashlib.sha512
#tables: Users, Carmodules
USERS_TABLE = ["Users",{"username":"varchar(255) not null","password":"varchar(255) not null","token":"varchar(128)"}]
CARMODULES_TABLE = "Carmodules"
def deleteUser(username:str=None,token:str=None,algo:str="exact")->bool:
    args =searchUser(username,token,algo)
    if len(args) !=1:
        return False
    args = {list(USERS_TABLE[1].keys())[x]: args[0][x+1] for x in range(len(args))}
    database.Delete(USERS_TABLE[0],args)

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
    return database.Search(USERS_TABLE[0],args)
if (__name__=="__main__"):
    addUser("shlomi","pass")
    print(searchUser("shlomi"))