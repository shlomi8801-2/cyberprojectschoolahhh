import sqlite3
from settings import GetSetting,GetCurrentDir
from log import log
import os.path

dbtype = GetSetting("database.type")
DB :sqlite3.Connection = None
problem = None

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
    """returns list of lines(tuples) found in database select query with the args as the string after where for example:
    select * from <table> where <args>;"""
    #convert everything to base64 to prevent sql injection
    match dbtype:
        case "sqlite":
            res = DB.execute(f"select * from {table} where {args}")
            return res.fetchall() 
        case _:
            log(f"database type not found {dbtype}!")
            raise f"database type not found {dbtype}!"
def Insert(table:str,values:tuple) -> bool:
    try:
        match dbtype:
            case "sqlite":
                    DB.execute(f"insert into {table} values({", ".join(["?" for x in range(len(values))])})",values)
                    DB.commit()
                    return True
            case _:
                log(f"database type not found {dbtype}!")
                raise f"database type not found {dbtype}!"
    except Exception as e:
        log(f"something went wrong while inserting to database: {e}")
        return False
def Delete(table:str,args:str)->bool:
    try:
        match dbtype:
            case "sqlite":
                    DB.execute(f"delete from {table} where {args}")
                    DB.commit()
                    return True
            case _:
                log(f"database type not found {dbtype}!")
                raise f"database type not found {dbtype}!"
    except Exception as e:
        log(f"something went wrong while deleting from database: {e}")
        return False