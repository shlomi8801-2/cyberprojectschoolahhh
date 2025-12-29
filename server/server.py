import threading
import restapi
import boardDevicesServer

#available real world options(examples):
# turn on ac(optional for each car)
# lock it / unlock it
# view it on the map(gps)
# roll windows

#basic funcctions for server
# login with token
# register and register car(will think about how)
# logout from all devices(just deleting the tokens)
#dont forget using a function(even one line) for each outside module for example database for easy change of modules
restthread = threading.Thread(target=restapi.startServer)
contollersthread = threading.Thread(target=boardDevicesServer.startServer)



if (__name__ == "__main__"):
    restthread.start()
    contollersthread.start()