import socket
import settings
import database
from constants import *
import client_coms
import utils
import log
import controllersActions
import time
waitingToRegister = dict() # id -> password in plain text
connectedClients = dict() # id -> Controller object


def getClients(args:dict,algo:str="exact")->list:
    #all the dict keys MUST be columns in the table in the database
    if (len(args) ==0):
        return []
    return database.Search(CARMODULES_TABLE[0],args)

def loginClient(_id:str,password:str)->tuple:
    """searching the client in the database if found and the password and id match,
    returnes the row of the client in the database as tuple"""
    return database.Search(CARMODULES_TABLE[0],{"uuid":_id,"password":password})

def addClientToDatabase(_id:str,password:str,availablePins:str) ->None:
    database.AddTable(*CARMODULES_TABLE)
    print("".join(sorted(list(set(x for x in availablePins)))))
    database.Insert(CARMODULES_TABLE[0],{"uuid":_id,"password":password,"availablePins":"".join(sorted(list(set(x for x in availablePins))))}) # the availablePins compacting is to prevent sql injection and use the laest amount of chars
def expirId(_id:str)->None:
    #used as thread waits until MAXREGISTERWAIT seconds pass then removs the id from the waitingToRegister dict
    time.sleep(MAXREGISTERWAIT)
    if (_id in waitingToRegister):
        del waitingToRegister[_id]
def handleCon(cSock:client_coms.clientSock,msg:dict)->None:
    #gets the uuid from the client and checks password
    if (type(msg) != dict):
        log.log(f"msg type is not dict!\nmsg:{msg}")
        return
    controllerRow = loginClient(msg.get("uuid",""),msg.get("psd","")) #list of tuples of data
    if (controllerRow == None):
        cSock.sendcmd("CON",{"code":"1"}) #invalid login details
        return
    #logged in
    #might change later to new thread for less memory usage because this function ends after the handleClient function ends
    controller = controllersActions.Controller(controllerRow[0],cSock)
    cSock.sendcmd("CON",{"code":"0"})
    handleClient(controller)

def registerClient(cSock:client_coms.clientSock,msg:dict)->None:
    #msg is the message after the parsedata function applied to it
    #when gettings register message it will generate unique id and password both will be sent to the controller
    #the controller must send the id and password back in order to complete the registration
    #the password will be saved as hash in the database
    #the id as unique key
    #the availablePins column will have max of 20 pins each is a char representing a number of pin
    log.log("starting to register client")
    if not ("uuid" in msg and "psd" in msg):
        #create a request to register new client
        _id = utils.generateToken()
        _password = utils.generateToken()
        # _id = "123456789123457891234567891231"
        # _password ="123456789123457891234567891231"
        while (len(getClients({"uuid":_id}))==1): #acquiring a unique id
                _id = utils.generateToken()
        waitingToRegister[_id] = _password
        log.log("sending respose with login details")
        cSock.sendcmd("REG",{"uuid":_id,"psd":_password})
        utils.makeThreadAndStart(expirId,[_id])
    else:
        #got response to the register msg
        _id = msg.get("uuid","")
        if not (_id in waitingToRegister):
            # cSock.sendcmd("REG",{"error":f"request for uuid {_id} doesn't exist","code":"1"})
            cSock.sendcmd("REG",{"code":"1"})
            return
        if (msg.get("psd","") != waitingToRegister[_id]):
            # cSock.sendcmd("REG",{"error":"passwords does not match","code":"2"})
            cSock.sendcmd("REG",{"code":"2"})
            return
        addClientToDatabase(_id,msg["psd"],msg.get("AP",""))
        cSock.sendcmd("REG",{"code":0}) #success
        log.log("registered a controller")
        del waitingToRegister[_id]

def handleClient(controllerObj:controllersActions.Controller)->None:
    #listen to each client and handle commands
    connectedClients[controllerObj.Id] = controllerObj
    while (controllerObj.connected):
        msg = controllerObj.Csock.recievecmd() # (type:str,data:dict)
        if (len(msg) == 0):
            continue
        match (msg.get("type","NOTYPE")): #commands are here
            case "PING":
                pass
            case "LOGOUT":
                del connectedClients[controllerObj.Id]
                controllerObj.connected =False
                return
            case _:
                log.log(f"warning: uknown command {msg.get("type","NOTYPE")}")
                break
    # del connectedClients[controller.Id] #client not connected

def indentifyClient(clientSock:client_coms.clientSock):
    """used for client first messages only for REG and CON"""
    known:bool = False
    # log.log("got new client!")
    while (not known):
        msg = clientSock.recievecmd() # (type:str,data:dict)
        if (len(msg) == 0):
            continue
        if (type(msg) != dict):
            log.log(f"msg type is not dict!\nmsg:'''{msg}'''")
            continue
        match (msg.get("type","NOTYPE")): #commands are here
            case "REG":
                registerClient(clientSock,msg)
                continue
            case "CON": #connect - first message
                handleCon(clientSock,msg)
                continue
            case _:
                log.log(f"warning: uknown command {msg.get("type","NOTYPE")}")
                clientSock.sock.close()
                break
def listen(host:str,port:int)->None:
    #Reg - to register client to the database and give unique id
    #Con - connect login the client
    #Action - tell the controller what to do on what pins
    #list - give the device details like available pins
    server = socket.socket(socket.AF_INET,socket.SOCK_STREAM)# ipv4,tcp
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1) #fixing the binding address again
    server.bind((host,port))
    server.listen(5)
    while (True):
        cSock = client_coms.clientSock(server.accept())
        utils.makeThreadAndStart(indentifyClient,[cSock])


def startServer():
    host = settings.GetSetting("server.listen")
    port =int(settings.GetSetting("server.port"))
    log.log(f"starting controllers server at {host}:{port}")
    listen(host,port)