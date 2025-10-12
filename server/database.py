import sqlite3
from settings import GetSetting,GetCurrentDir
from log import log
import os.path

dbtype = GetSetting("database.type")
DB = None

def CheckConnection()->bool:
    """returns true or false wether the conenction to the database was succesful
    and optionally prints if """
    match dbtype:
        case "sqlite":
            #https://www.geeksforgeeks.org/python/introduction-to-sqlite-in-python/
            dbfile = GetSetting("database.file")
            dbfile =(GetCurrentDir()+"/"+dbfile) if dbfile[0] !="/" else dbfile
            if (not os.path.isfile(dbfile)):
                log(f"database file not found {dbfile}!")
                return False
            DB = sqlite3.connect(dbfile)
        case _:
            log(f"database type not found {dbtype}!")
            return False
def Search(table:str,args:str)->list:
    """returns list of lines found in database select query with the args as the string after where for example:
    select * from <table> where <args>;"""
    pass
def Insert():
    pass
def Delete():
    pass