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
    global DB #point to the global variable
    match dbtype:
        case "sqlite":
            #https://www.geeksforgeeks.org/python/introduction-to-sqlite-in-python/
            dbfile = GetSetting("database.file")
            dbfile =(GetCurrentDir()+"/"+dbfile) if dbfile[0] !="/" else dbfile
            if (not os.path.isfile(dbfile)):
                log(f"database file not found {dbfile}!")
                return False
            DB = sqlite3.connect(dbfile)
            return True
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
def Insert(table:str,values:dict) -> bool:
    try:
        match dbtype:
            case "sqlite":
                    DB.execute(f"insert into {table} ({", ".join(list(values.keys()))}) values({", ".join(["?" for x in range(len(values.values()))])})",list(values.values()))
                    DB.commit()
                    return True
            case _:
                log(f"database type not found {dbtype}!")
                raise f"database type not found {dbtype}!"
    except Exception as e:
        log(f"something went wrong while inserting to database: {e}")
        return False
def Update(table:str,values:dict,searchArgs:str):
    """UPDATE table
    SET column_1 = new_value_1,
        column_2 = new_value_2
    WHERE
        search_condition 
    ORDER column_or_expression"""
    
    try:
        match dbtype:
            case "sqlite":
                    DB.execute(f"UPDATE {table} SET {','.join([x[0]+' = "'+x[1]+'"' for x in values.items()])} where {searchArgs}",list(values.values()))
                    DB.commit()
                    return True
            case _:
                log(f"database type not found {dbtype}!")
                raise f"database type not found {dbtype}!"
    except Exception as e:
        log(f"something went wrong while updating table: {e}")
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
def AddTable(table:str,args:dict)->bool:
    """CREATE TABLE table_name (
    column1 datatype,
    column2 datatype,
    column3 datatype,
    ....
    );"""
    try:
        match dbtype:
            case "sqlite":
                    DB.execute(f"CREATE TABLE IF NOT EXISTS {table} (ID INTEGER PRIMARY KEY AUTOINCREMENT,{','.join([' '.join(x) for x in args.items()])});")
                    DB.commit()
                    return True
            case _:
                log(f"database type not found {dbtype}!")
                raise f"database type not found {dbtype}!"
    except Exception as e:
        log(f"something went wrong while creating a table: {e}")
        return False
CheckConnection()