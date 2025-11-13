import settings
import socket
import websocket

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
        encoded = int.from_bytes(data[offset:offset+1])#1 byte for the encoded variable
        offset +=1
        #get the value
        if encoded == 1:
            val = data[offset:offset + val_len].decode(settings.GetSetting("client.encoding"))
        else:
            val = data[offset:offset + val_len]
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
    for value in [x for y in data.items() for x in y]:
        encoded = 0
        if not isinstance(value, (bytes, bytearray)):
            encoded = 1
            if not value is str:
                value=str(value)
            value= bytes(value,settings.GetSetting("client.encoding"))
        datalength = len(value)
        for x in datalength.to_bytes( settings.GetSetting("client.header_size"),'big'):
            output.append(x)
        output.append(encoded)
        for x in value:
            output.append(x)
    
    #add the whole array length without the first number before as only header value
    #as unsigned int at the size of client.header_size with big indian
    msglength = len(output)
    if msglength > (1<<8*settings.GetSetting("client.header_size")): #checking if we are able to have all of the dict in one byte-array
        raise "data length is too big! (may need to increase the header size)"
    output += msglength.to_bytes(settings.GetSetting("client.header_size"),"big",signed=False)

    
    return output

