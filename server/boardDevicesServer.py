import socket
import settings
import database
from constants import *
import client_coms
import utils
import log

waitingToRegister = set() # set of sets having each id and password

def getClients(args:dict,algo:str="exact")->list:
    #all the dict keys MUST be columns in the table in the database
    if (len(args ==0)):
        return []
    return database.Search(CARMODULES_TABLE[0],args)        
    pass

def registerClient(cSock:client_coms.clientSock)->None:
    #when gettings register will generate unique id and password both will be sent to the controller
    #the controller must send the id and password back in order to complete the registration
    #the password will be saved as hash in the database
    #the id as unique key
    #the availablePins column will have max of 20 pins each is a char representing a number of pin
    database.AddTable(*CARMODULES_TABLE)
    _id = utils.generateToken()
    _password = utils.generateToken()
    while (len(getClients({"id":_id}))==1): #acquiring a unique id
            _id = utils.generateToken()
    client = {"id":_id,"password":_password}
    waitingToRegister.add(client)
    cSock.sendcmd("REG",client)

def handleClient():
    #listen to each client and handle commands
    pass    

def listen(host:str,port:int)->None:
    #Reg - to register client to the database and give unique id
    #Action - tell the controller what to do on what pins
    #list - give the device details like available pins

    pass


def startServer():
    listen(settings.GetSetting("server.listen"),int(settings.GetSetting("server.port")))