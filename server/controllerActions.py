import boardDevicesServer
from socket import socket
from constants import CONTROLLERSCOMMANDS_TABLE,CARMODULES_TABLE
import utils
from client_coms import clientSock
import database
class clientCommand:
    action:str = ""
    controllerId:str = ""
    buttonTitle:str =""
    def __init__(self, sqlRow:tuple):
        sqlRow = utils.makeSqlDict(sqlRow,CONTROLLERSCOMMANDS_TABLE)
        self.action = sqlRow.get("actions","")
        self.controllerId = sqlRow.get("ControllerId","")
        self.buttonTitle = sqlRow.get("title","")

class Controller:
    Id:str = ""
    Csock:clientSock = None
    connected:bool = True
    availablePins:list = None
    def __init__(self, sqlRow:tuple,accept_res:tuple):
        if (len(sqlRow) ==0): #couldn't login the controller
            self.connected = False
            return
        sqlRow:dict = utils.makeSqlDict(sqlRow,CARMODULES_TABLE)
        self.Id = sqlRow.get("id",None)
        self.Csock = clientSock(accept_res)
        self.availablePins = [ord(x) for x in sqlRow.get("availablePins")]
    def sendCommand(self,command:clientCommand)->None: # for action
        if (not self.connected):
            return
        self.Csock.sendcmd("action",command.action)
        
    def sendCommand(self,cmdtype:str,data:dict)->None:
        if (not self.connected):
            return
        self.Csock.sendcmd(cmdtype,data)
