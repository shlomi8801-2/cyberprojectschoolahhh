from flask import Flask,request
from flask_cors import CORS
import users
import settings
import constants
import controllersActions 
import utils

app = Flask(__name__)
CORS(app,supports_credentials=True)


#login - done
#test - done
#register - done
#list of arduino modules(they will ask to register) - done
#option to assign module to user
#home page after logged in - done

def handleHeaders():
    #https://stackoverflow.com/questions/10434599/get-the-data-received-in-a-flask-request
    try:
        return request.headers
    except:
        #just in case
        raise Exception("error parsing headers")

def checkpermissions() -> tuple: # (int,dict)
    """checks with the token cookie and returnes the permission level and the user searched to save time searching again if needed
    if user not found returns (-1,{})"""
    # 0 - normal user
    # 1 - admin
    res = handleHeaders()
    if not ("token" in res): #takes the request object from the thread it's running in
        return (-1,{})
    user = users.searchUser(token=res.get("Token"))
    if (len(user) != 1):
        return (-1,{})
    user = user[0] # because its a list of tuples get the first one
    user = utils.makeSqlDict(user,constants.USERS_TABLE)
    if (not users.validateToken(user)):
        return (-1,{})
    return (int(user.get("permissions_level","0")),user)

@app.errorhandler(Exception)
def handle_bad_request(e):
    return {"code":1,"error":str(e)}, 400

@app.route("/")
def test()->str:
    return "OK(ye its working)"

@app.route("/register", methods = ['POST'])
def register()->dict:
    must = ["username","pass","cpass"]
    res = request.get_json()
    for x in must:
        if not (x in must):
            return {"error":"those fields does not present in request!","missing":",".join([y for y in must if not (y in res)]),"code":1}
    if (res["pass"] != res["cpass"]):
        return {"error":"passwords does not match!","code":1}
    token = users.register(str(res["username"]),str(res["pass"]))
    if not token:
        return {"error":"username already used!","code":1}
    return {"token":token,"code":0}

@app.route("/login", methods = ['POST'])
def login()->dict:
    must = ["username","pass"]
    res = request.get_json()
    for x in must:
        if not (x in must):
            return {"error":"those fields does not present in request!","missing":",".join([y for y in must if not (y in res)]),"code":1}
    token = users.login(str(res["username"]),str(res["pass"]))
    if not token:
        return {"error":"username or password are incorrect!","code":1}
    return {"token":token,"code":0}



@app.route("/logout", methods = ['POST'])
def logout()->dict:
    must = ["token"]
    res = handleHeaders()

    for x in must:
        if not (x in must):
            raise Exception("those fields does not present in request!\nmissing:"+",".join([y for y in must if not (y in res)]) )
    return {"code":0 if users.logout(res.get("token")) else 1}

@app.route("/list/<_type>/<rows>")
@app.route("/list/<_type>/<rows>/<offset>", defaults={'offset': 0})
def getList(_type:str,rows:int = 100,offset:int=0,filters:dict={},bypassPermissionChecking:bool=False)->dict:
    permsForTypes = {1:{"users","controllers"}} # 0 is default
    try:
        rows = int(rows)
        offset = int(offset)
    except:
        raise Exception("rows/offset should be a number")
        # return {"code":1,"error":"rows/offset should be a number"}
    perm,userDict = checkpermissions()
    if ((not perm >= 0) and not bypassPermissionChecking):
        raise Exception("permission level is too low - probably not logged in")
        return {"code":1,"error":"permission level is too low - probably not logged in"}
    
    #here its volnoruble for sql injection but insert and search functions in database.py handles it
    filters = utils.dictFromJson(request.headers.get("filters","{}"))
    #checking for the permissions required
    for x in permsForTypes:
        if bypassPermissionChecking:
            break
        if _type in permsForTypes[x]:
            if (not perm >= x):
                raise Exception("permission level is too low")
                # return {"code":1,"error":"permission level is too low"}
    match (_type):
        case "users":
            #get users
            usersList = users.getUsersList(maxRows=rows,offset=offset,filters=filters)
            #get the columns that sent
            #remove the password and token from the users to not show it
            keys = users.removeFromUsersList(usersList,("password","token"))
                    
            return {"code":0,"columns":keys,"users":usersList}
        case "controllers":
            ControllersList = controllersActions.getControllersList(maxRows=rows,offset=offset,filters=filters)
            keys = controllersActions.removeFromControllersList(ControllersList,("password"))
            return {"code":0,"columns":keys,"controllers":ControllersList}
        case "myControllers":
            #acuire user from token then search with username for controllers
            #uses previous checking for the user
            return getList("controllers",rows,offset,{"ownerUsername":userDict.get("username","")},bypassPermissionChecking=True)
        case "commands":
            #gets the id of the controller from the request then send back the result from commands that match the requirements in the database
            #get the id from the headers
            uuid = request.headers.get("uuid","")
            if (len(controllersActions.getControllersList(maxRows=2,offset=0,filters={"ownerUsername":userDict.get("username",""),"uuid":uuid}))!= 1):
                #multiple or no controllers fitting this info
                raise Exception("multiple or no controllers rather then 1 unique controller")
                # return {"error":1,"error":"multiple or no controllers rather then 1 unique controller"}
            return {"code":0,"columns":constants.CONTROLLERSCOMMANDS_TABLE[1],"commands":controllersActions.getControllerCommands()}
        case _:
            raise Exception(f"no such endpoint /list/{_type}")
            # return {"code":1,"error":f"no such type {_type}"}

@app.route("/controllers/<controllerId>/<option>",methods=['GET', 'POST'])
def controllerManagement(controllerId:int,option:str="get"):
    perm,user = checkpermissions()
    if (perm ==-1):#user not logged in
        raise Exception("not logged in")
    with getList("controllers",1,0,{"ownerUsername":user.get("username",""),"uuid":controllerId},bypassPermissionChecking=True) as controller:
        if (len(controller) == 0):
            raise f"no such controller with id {controllerId}"
        controller = controller[0]
    
    commandTitle = request.json().get("title","")

    if (request.method == "POST"):
        match (option):
            #only for the commands - no need to change the controller row
            case "update":
                #check if a command have the same title already
                if not controllersActions.checkCommandExistanceById(controllerId=controllerId,title=commandTitle):
                    raise f"command {commandTitle} does not exist for controller {controllerId}"
                pass
            case "add":
                #check if a command have the same title already
                if controllersActions.checkCommandExistanceById(controllerId=controllerId,title=commandTitle):
                    raise f"command {commandTitle} already exists for controller {controllerId}"
                pass
            case "delete":
                #check if a command have the same title 
                if not controllersActions.checkCommandExistanceById(controllerId=controllerId,title=commandTitle):
                    raise f"command {commandTitle} does not exist for controller {controllerId}"

                pass
            case _:
                raise f"no such option {option}"
    if (request.method == "GET"):
        pass
            
        

def startServer():
    app.run(host=settings.GetSetting("restApi.listen"),port=settings.GetSetting("restApi.port"))