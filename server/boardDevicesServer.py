import socket
import settings
import database
from constants import *
import client_coms
import utils
import log
import time
waitingToRegister = dict() # id -> password in plain text

def getClients(args:dict,algo:str="exact")->list:
    #all the dict keys MUST be columns in the table in the database
    if (len(args ==0)):
        return []
    return database.Search(CARMODULES_TABLE[0],args)
def addClientToDatabase(_id:str,password:str,availablePins:str) ->None:
    database.AddTable(*CARMODULES_TABLE)
    database.Insert(CARMODULES_TABLE[0],{"id":_id,"password":password,"availablePins":"".join(sorted(list(set(x for x in availablePins))))}) # the availablePins compacting is to prevent sql injection and use the laest amount of chars
def expirId(_id:str)->None:
    #used as thread waits until MAXREGISTERWAIT seconds pass then removs the id from the waitingToRegister dict
    time.sleep(MAXREGISTERWAIT)
    if (_id in waitingToRegister):
        del waitingToRegister[_id]
    
def registerClient(cSock:client_coms.clientSock,msg:dict)->None:
    #msg is the message after the parsedata function applied to it
    #when gettings register will generate unique id and password both will be sent to the controller
    #the controller must send the id and password back in order to complete the registration
    #the password will be saved as hash in the database
    #the id as unique key
    #the availablePins column will have max of 20 pins each is a char representing a number of pin
    if not ("id" in msg and "password" in msg):
        #create a request to register new client
        _id = utils.generateToken()
        _password = utils.generateToken()
        while (len(getClients({"id":_id}))==1): #acquiring a unique id
                _id = utils.generateToken()
        waitingToRegister[_id] = _password
        cSock.sendcmd("REG",{"id":_id,"password":_password})
        utils.makeThreadAndStart(expirId,[_id])
    else:
        #got response to the register msg
        _id = msg["id"]
        if not (_id in waitingToRegister):
            cSock.sendcmd("REG",{"error":f"request for id {_id} doesn't exist"})
            return
        if (_id != waitingToRegister[_id]):
            cSock.sendcmd("REG",{"error":"passwords does not match"})
            return
        addClientToDatabase(_id,msg["password"],msg.get("availablePins",""))
        del waitingToRegister[_id]
    

def handleClient(clientSock:client_coms.clientSock)->None:
    #listen to each client and handle commands
    conncted = True
    while (conncted):
        msg = clientSock.recievecmd()
        if (len(msg) == 0):
            continue
        match (msg[0]): #commands are here
            case "REG":
                registerClient(clientSock,msg[1])
                break
            case _:
                log.log(f"warning: uknown command {msg[0]}")
                break

def listen(host:str,port:int)->None:
    #Reg - to register client to the database and give unique id
    #Action - tell the controller what to do on what pins
    #list - give the device details like available pins
    server = socket.socket(socket.AF_INET,socket.SOCK_STREAM)# ipv4,tcp
    server.bind((settings.GetSetting("server.listen"),int(settings.GetSetting("server.port"))))
    server.listen(5)
    while (True):
        cSock = client_coms.clientSock(server.accept())
        utils.makeThreadAndStart(handleClient,[cSock])


def startServer():
    listen(settings.GetSetting("server.listen"),int(settings.GetSetting("server.port")))