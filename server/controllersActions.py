from constants import CONTROLLERSCOMMANDS_TABLE,CARMODULES_TABLE
import utils
import database
from client_coms import clientSock

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
    def __init__(self, sqlRow:tuple,Csock:clientSock):
        if (len(sqlRow) ==0): #couldn't login the controller
            self.connected = False
            return
        sqlRow:dict = utils.makeSqlDict(sqlRow,CARMODULES_TABLE)
        self.Id = sqlRow.get("id",None)
        self.Csock = Csock
        self.availablePins = [ord(x) for x in sqlRow.get("availablePins")]
    def sendCommand(self,command:clientCommand)->None: # for action
        if (not self.connected):
            return
        self.Csock.sendcmd("action",command.action)
        
    def sendCommand(self,cmdtype:str,data:dict)->None:
        if (not self.connected):
            return
        self.Csock.sendcmd(cmdtype,data)
def getControllersList(filters:dict={},maxRows:int=100,offset:int=0,algo:str="exact")->list:
    output = database.Search(CARMODULES_TABLE,utils.buildWhereQuery(filters,algo),maxRows,offset)
    return [] if output is None else output
def removeFromControllersList(ControllersList:list,columns:tuple)->list:
    return utils.removeFromSqlList(ControllersList,columns,CARMODULES_TABLE)