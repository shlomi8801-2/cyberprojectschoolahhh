import settings
import socket
import log

class clientSock:
    def __init__(self,accept_res:tuple):
        """accept_res is the output of socker.accept()"""
        self.sock:socket.socket = accept_res[0]
        self.connected=True
        #todo: add a condition to check if still connected
    def sendcmd(self,cmdtype:str,data:dict) ->None:
        """gets a type and a dict for easier loading of the remote client(in the car) the packet will be built like: length of all the msg without the first num then 
        length:key:length:value:length:key...."""
        if not self.connected:
            return
        if (cmdtype and data):
            self.sock.sendall(buildata({cmdtype:buildata(data)}))
    def recievecmd(self)->tuple:
        """run in a loop, waits for bytes from the client then parsing it and returning it(as tuple of type and dict)
        gets the first couple of data for now (type:str:data:dict)"""
        if not self.connected:
            return ()
        res_len = int.from_bytes(self.sock.recv(settings.GetSetting("client.header_size")),"big") #the first one is without encoded byte which is the length
        res = self.sock.recv(res_len)
        print(res_len)
        print(res)
        try:
            output = parsedata(res)
            if (len(output) ==0):
                return {}
                raise Exception(f"blank after parsing!\n{output}")
            return list(output.items())[0] #gets the first couple of data for now (type:data)
        except Exception as e:
            #should be a warning not an error
            log.log(f"warning: an error occured reciving data from controller \n{e}")
            return {}
            raise Exception(f"error parsing data!\n{e}")

def parsedata(data: bytearray)->dict:
    '''byte array which looks like this <length><encoded?><data><length2><encoded?><data2> for exanple:   \x00\x00\x00\x04\x00code\x00\x00\x00\x05\x00abcde
    the length is always as the HEADER_SIZE the data is always encoded with the encoding in settings
    DON'T FORGET TO REMOVE THE DATA LENGTH FROM THE START'''
    datastructure = []
    output = {}
    offset = 0
    while offset < len(data):
        #get the length of the key
        #data[something] is the number of the bytes
        val_len = int.from_bytes(data[offset:offset + settings.GetSetting("client.header_size")]) #read settings.HEADER_SIZE bytes and returns a tuple so read the first element
        offset += settings.GetSetting("client.header_size")
        #get the value
        #always decode because its encoded always string or not
        val = data[offset:offset + val_len].decode(settings.GetSetting("client.encoding"))

        offset += val_len
        #make one key and then assign value
        if len(datastructure) == 0:
            datastructure.append(val)
        else:
            output[datastructure.pop(0)] = val
    return output

def buildata(data: dict)->bytearray:
    '''use settings.HEADER_SIZE bytes for the length of the value
    for 2^(8*settings.HEADER_SIZE) of data support
    takes the dict and turns into a simple string then bytearray'''
    
    output = bytearray()
    if len(data) ==0:
        raise ValueError(f"Data must be a dict with 1 or 2 items given:{len(data)}")
    for value in [x for y in data.items() for x in y]: #makes a list of keys and values combined
        # encoded = 0
        if not isinstance(value, (bytes, bytearray)):
            # encoded = 1
            if not value is str:
                value=str(value)
            value= bytes(value,settings.GetSetting("client.encoding"))
        datalength = len(value)
        for x in datalength.to_bytes( settings.GetSetting("client.header_size"),'big'):
            output.append(x)
        # output.append(encoded)
        for x in value:
            output.append(x)
    
    #add the whole array length without the first number before as only header value
    #as unsigned int at the size of client.header_size with big indian
    msglength = len(output)
    if msglength > (1<<8*settings.GetSetting("client.header_size")): #checking if we are able to have all of the dict in one byte-array
        raise "data length is too big! (may need to increase the header size)"
    output = msglength.to_bytes(settings.GetSetting("client.header_size"),"big",signed=False)+output    
    return output

def test_buildNparsedata():
    import random
    data = {''.join([chr(random.randint(0,255)) for x in range(random.randint(1,100))]):''.join([chr(random.randint(0,255)) for x in range(random.randint(1,100))])}
    datap = buildata(data)
    return data == parsedata(datap[settings.GetSetting("client.header_size"):])
if (__name__=="__main__"):
    print(test_buildNparsedata())