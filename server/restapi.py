from flask import Flask,request
from flask_cors import CORS
import users
import settings
import constants
import controllersActions as controllersActions
import utils
app = Flask(__name__)
CORS(app,supports_credentials=True)


#login
#test
#register
#list of arduino modules(they will ask to register)
#option to assign module to user
#home page after logged in

def checkpermissions() -> tuple: # (int,dict)
    """checks with the token cookie and returnes the permission level and the user searched to save time searching again if needed
    if user not found returns (-1,{})"""
    # 0 - normal user
    # 1 - admin
    if not ("token" in request.headers): #takes the request object from the thread it's running in
        return (-1,{})
    user = users.searchUser(token=request.headers.get("Token"))
    if (len(user) != 1):
        return (-1,{})
    user = user[0] # because its a list of tuples get the first one
    user = utils.makeSqlDict(user,constants.USERS_TABLE)
    return (int(user.get("permissions_level","0")))
        
    

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
    res = request.headers
    for x in must:
        if not (x in must):
            return {"error":"those fields does not present in request!","missing":",".join([y for y in must if not (y in res)]),"code":1}
    return {"code":0 if users.logout(res.get("token")) else 1}
@app.route("/list/<_type>/<rows>")
@app.route("/list/<_type>/<rows>/<offset>", defaults={'offset': 0})
def getList(_type:str,rows:int = 100,offset:int=0,filters:dict={})->dict:
    permsForTypes = {1:{"users","controllers"}} # 0 is default
    try:
        rows = int(rows)
        offset = int(offset)
    except:
        return {"code":1,"error":"rows/offset should be a number"}
    perm,userDict = checkpermissions()
    if (not perm >= 0):
        return {"code":1,"error":"permission level is too low - probably not logged in"}
    
    try:
        #here its volnoruble for sql injection but insert and search functions in database.py handles it
        filters = utils.dictFromJson(request.headers.get("filters","{}"))
        #checking for the permissions required
        for x in permsForTypes:
            if _type in permsForTypes[x]:
                if (not perm >= x):
                    return {"code":1,"error":"permission level is too low"}
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
                return getList("controllers",rows,offset,{"ownerUsername":userDict.get("username","")})
            case _:
                return {"code":1,"error":f"no such type {_type}"}
    except Exception as e:
        return {"code":1,"error":e}

def startServer():
    app.run(host=settings.GetSetting("restApi.listen"),port=settings.GetSetting("restApi.port"))