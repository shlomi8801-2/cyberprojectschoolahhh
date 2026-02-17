from constants import CONTROLLERSCOMMANDS_TABLE,CARMODULES_TABLE
import utils
import database
from client_coms import clientSock
import constants

class clientCommand:
    action:str = ""
    controllerId:str = ""
    buttonTitle:str =""
    def __init__(self, sqlDict:dict):
        sqlDict = utils.makeSqlDict(sqlDict,CONTROLLERSCOMMANDS_TABLE)
        self.action = sqlDict.get("actions",None)
        self.controllerId = sqlDict.get("ControllerId",None)
        self.buttonTitle = sqlDict.get("title",None)
    def toDict(self)->dict:
        """generates a dict from this object to add to database"""
        #{"ControllerId":"varchar(30)",
        #"title":"varchar(50)",
        #"actions":"varchar(255)"}
        self.validateMissingData()
        return {"ControllerId":self.controllerId,"title":self.buttonTitle,"actions":self.action}
    def addToDatabase(self):
        database.Insert(CONTROLLERSCOMMANDS_TABLE[0],self.toDict())
    def deleteFromDatabase(self):
        if not (self.buttonTitle and self.controllerId):
            raise Exception("missing controllerId and title to identify command")
        database.Delete(CONTROLLERSCOMMANDS_TABLE[0],{"title":self.buttonTitle,"ControllerId":self.controllerId})
    def updateDatabase(self):
        database.Update(CONTROLLERSCOMMANDS_TABLE[0],self.toDict(),{"ControllerId":self.controllerId,"title":self.buttonTitle})
    def validateMissingData(self)->None:
        """if any of the data members in the object is missing raises an error"""
        if not (self.action and self.controllerId and self.buttonTitle):
            raise Exception(f"missing data: {"action" if self.action else ""} {"controllerId" if self.controllerId else ""} {"title" if self.buttonTitle else ""}")



class Controller:
    Id:str = ""
    Csock:clientSock = None
    connected:bool = True
    availablePins:list = None
    def __init__(self, sqlRow:tuple,Csock:clientSock = None):
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
    # def addCommand(cmd:clientCommand)->None:
    #     cmd.addToDatabase()
    # def updateCommand(cmd:clientCommand)->None: 
    #     """set in the database the command with the id of 'cmd' to the current values"""
    #     cmd.updateDatabase()
def getControllerCommands(controllerId:str,maxRows:int=100,offset:int=0) -> list:
    return database.Search(CONTROLLERSCOMMANDS_TABLE[0],{"ControllerId":controllerId},maxRows,offset,"exact")
def getControllersList(filters:dict={},maxRows:int=100,offset:int=0,algo:str="exact")->list:
    output = database.Search(CARMODULES_TABLE[0],filters,maxRows,offset,algo)
    return [] if output is None else output
def checkCommandExistanceByTitle(controllerId:str,title:str) -> bool:
    """returnes true if the command was found in the database else returns false"""
    return len(database.Search(CONTROLLERSCOMMANDS_TABLE[0],{"ControllerId":controllerId,"title":title})) >=1
def checkCommandExistanceByRow(controllerRow:tuple,title:str) -> bool:
    """returnes true if the command was found in the database else returns false"""
    return len(database.Search(CONTROLLERSCOMMANDS_TABLE[0],{"ControllerId":controllerRow[[constants.CONTROLLERSCOMMANDS_TABLE[1].keys()].index("ControllerId")],"title":title})) >=1
def removeFromControllersList(ControllersList:list,columns:tuple)->list:
    #removing columns from the list of rows(tuples)
    return utils.removeFromSqlList(ControllersList,columns,CARMODULES_TABLE)