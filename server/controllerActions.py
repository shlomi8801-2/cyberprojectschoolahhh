import boardDevicesServer
from socket import socket
from constants import CONTROLLERSCOMMANDS_TABLE,CARMODULES_TABLE
import utils
import database
class clientCommand:
    action:str = ""
    controllerId:str = ""
    buttonTitle:str =""
    def __init__(self, sqlRow:tuple):
        sqlRow = utils.makeSqlDict(sqlRow,CONTROLLERSCOMMANDS_TABLE)
        self.action = sqlRow

class Controller:
    Id:str = ""
    Socket:socket = None
    connected:bool = True
    availablePins:list = None
    def __init__(self, sqlRow:tuple,CSock:socket):
        if (len(sqlRow) ==0): #couldn't login the controller
            self.connected = False
            return
        sqlRow:dict = utils.makeSqlDict(sqlRow,CARMODULES_TABLE)
        self.Id = sqlRow.get("id",None)
        self.Socket = CSock
        self.availablePins = [ord(x) for x in sqlRow.get("availablePins")]
    def sendCommand(self,command:clientCommand)->None:
        if (not self.connected):
            return
        self.Socket.send
        
