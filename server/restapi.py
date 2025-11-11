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
    print(request)
    return "hello world!"

app.run(debug=True)