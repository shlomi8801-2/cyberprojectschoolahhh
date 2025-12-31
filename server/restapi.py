from flask import Flask,request
from flask_cors import CORS
import users
import settings
import constants
import utils
app = Flask(__name__)
CORS(app,supports_credentials=True)


#login
#test
#register
#list of arduino modules(they will ask to register)
#option to assign module to user
#home page after logged in

def checkpermissions(permission_level:int = 1) -> bool:
    """checks with the token cookie if the user has the required permissions True if they have else False"""
    # 0 - normal user
    # 1 - admin
    if not ("token" in request.cookies): #takes the request object from the thread it's running in
        return False
    user = users.searchUser(token=request.cookies.get("token"))
    if (len(user) != 1):
        return False
    user = user[0] # because its a list of tuples get the first one
    user = utils.makeSqlDict(user,constants.USERS_TABLE)
    return int(user.get("permissions_level","0"))>=permission_level
        
    

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
    res = request.cookies
    for x in must:
        if not (x in must):
            return {"error":"those fields does not present in request!","missing":",".join([y for y in must if not (y in res)]),"code":1}
    return {"code":0 if users.logout(res.get("token")) else 1}
@app.route("/list/<_type>/<rows>")
def getList(_type:str,rows:int = 100,offset:int=0)->dict:
    try:
        rows = int(rows)
        offset = int(offset)
    except:
        return {"code":1,"error":"rows/offset should be a number"}
    if (not checkpermissions(1)):
        return {"code":1,"error":"permission level is too low"}
    match (_type):
        case "users":
            #get users
            usersList = users.getUsersList(maxRows=rows,offset=offset)
            return {"code":0,"users":usersList}
        case "controllers":
            pass
        case _:
            return {"code":1,"error":f"no such type {_type}"}
    pass
def startServer():
    app.run(host=settings.GetSetting("restApi.listen"),port=settings.GetSetting("restApi.port"))