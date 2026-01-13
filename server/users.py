import database
from log import log
from utils import *

from constants import *

FIRST_USER = -1 # if the user table just created in this execution of the code it will give the first user permissions by setting its value to 1



def deleteUser(username:str=None,token:str=None,algo:str="exact")->bool:
    args =searchUser(username,token,algo)
    if args == None:
        return False
    if  len(args) !=1:
        return False
    args = makeUserDict(args[0])
    args = buildWhereQuery(args,algo)
    database.Delete(USERS_TABLE[0],args)
    return True

def addUser(username:str,password:str)->bool:
    global FIRST_USER
    database.AddTable(*USERS_TABLE)
    if (FIRST_USER == -1): # might be slow for alot of users
        if (database.Search(USERS_TABLE[0],{},1) != None):
            FIRST_USER = 1; #the first user
            log.log("first user!")
        else:
            FIRST_USER = 0; #not the first user
    elif (FIRST_USER not in [ 0,1]): #to not encounter some error relating to FIRST_USER not being in the right range of values
        FIRST_USER = 0
    return database.Insert(USERS_TABLE[0],{"username":username,"password":hashString(password),"permissions_level":FIRST_USER,"date_created":getNowEpoc(),"token":""})

def searchUser(username:str=None,token:str=None,algo:str="exact")->list:
    """algo is [exact,like,contains]"""
    # database.AddTable(*USERS_TABLE)
    args = {"username":username,"token":token}
    
    try:
        return database.Search(USERS_TABLE[0],args,algo=algo)
    except Exception as e:
        log(f"something went wrong while searching: {e}")
        return []
def makeUserDict(user:tuple)->dict:
    """gets a user tuple and assigns the data to dict with its columns in the table"""
    if not isinstance(user,tuple):
        return None
    return makeSqlDict(user,USERS_TABLE)
def updateUser(values:dict,args:dict,algo:str="exact")->bool:
    """args should look like the makedict output for example: {"username":username,"token":token}"""
    args = buildWhereQuery(args,algo)
    if (args == None):
        log.log(f"searchUser went wrong no algo:{algo}")
    return database.Update(USERS_TABLE[0],values,args)

def getUsersList(filters:dict={},maxRows:int=100,offset:int=0,algo:str="exact")->list:
    output =database.Search(USERS_TABLE[0],filters,maxRows,offset,algo)
    return [] if output is None else output

#the more simple functions
def login(username:str,password:str)->str:
    """gets username and password returnes token or none"""
    res = searchUser(username)
    if (res == None):
        return None
    if (len(res)!=1):
        return None
    res = makeUserDict(res[0])
    if (res["password"] == hashString(password)):
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
            tmp = searchUser(token=token)
            while (tmp != None and len(tmp) >0):
                token = generateToken() #unique token for each user might be slow but safest for now
                tmp = searchUser(token=token)
            updateUser({"token_date":getNowEpoc(),"token":token},res)
            return token

def logout(token:str)->bool:
    """gets token and searches for it if found deletes it and returns true else returns false"""
    if (token == None):
        return True
    res = searchUser(token=token)
    if (res == None):
        return False
    if (len(res) != 1):
        return False
    
    res = makeUserDict(res[0])
    updateUser({"token_date":0},res) #sets the token date to 0 so its considered expired
    return True
    
def register(username:str,password:str)->str:
    """gets username and password returnes token or none"""
    res = searchUser(username)
    if (res != None and len(res)>=1): # if there is a user with that username
        return None
    addUser(username,password)
    return login(username,password)
def removeFromUsersList(UsersList:list,columns:tuple)->list:
    """returns the keys given from the output of the sql table to not send not needed columns"""
    return removeFromSqlList(UsersList,columns,USERS_TABLE)

if (__name__=="__main__"):
    deleteUser("shlomi")
    register("shlomi","1234")
    print(login("shlomi","1234"))
    pass
    