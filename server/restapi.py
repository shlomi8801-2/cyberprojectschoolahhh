from flask import Flask,request
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
def register()->str:
    must = ["username","pass","cpass"]
    res = request.json()
    for x in must:
        if not (x in must):
            return {"error":"those fields does not present in request!","missing":",".join([y for y in must if not (y in res)]),"code":1}
    
    return "hello world!"

app.run(debug=True)