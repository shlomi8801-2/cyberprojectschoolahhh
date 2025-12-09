from flask import Flask,request
import users
import settings
app = Flask(__name__)

#login
#test
#register
#list of arduino modules(they will ask to register)
#option to assign module to user
#home page after logged in

@app.route("/")
def test()->str:
    return "OK(ye its working)"

@app.route("/register", methods = ['POST'])
def register()->dict:
    must = ["username","pass","cpass"]
    res = request.json()
    for x in must:
        if not (x in must):
            return {"error":"those fields does not present in request!","missing":",".join([y for y in must if not (y in res)]),"code":1}
    if (res["pass"] != res["cpass"]):
        return {"error":"passwords does not match!","code":1}
    token = users.register(res["username"],res["pass"])
    if not token:
        return {"error":"username already used!","code":1}
    return {"token":token,"code":0}

@app.route("/login", methods = ['POST'])
def login()->dict:
    must = ["username","pass"]
    res = request.json()
    for x in must:
        if not (x in must):
            return {"error":"those fields does not present in request!","missing":",".join([y for y in must if not (y in res)]),"code":1}
    token = users.login(res["username"],res["pass"])
    if not token:
        return {"error":"username or password are incorrect!","code":1}
    return {"token":token,"code":0}
app.run(host=settings.GetSetting("restApi.listen"),port=settings.GetSetting("restApi.port"))