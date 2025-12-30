import sqlite3
from settings import GetSetting,GetCurrentDir
from log import log
import os.path

dbtype = GetSetting("database.type")
DB :sqlite3.Connection = None
problem = None

def CheckConnection(times:int=0)->bool:
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
                #create the file
                f = open(dbfile,"w")
                f.close()
                if (times >=5):
                    return False
                return CheckConnection(times+1)
            DB = sqlite3.connect(dbfile,check_same_thread=False)
            return True
        case _:
            log(f"database type not found {dbtype}!")
            return False
def Search(table:str,args:str, maxrows:int =-1,offset:int=0)->list:
    """returns list of lines(tuples) found in database select query with the args as the string after where for example:
    select * from <table> where <args>;"""
    #convert everything to base64 to prevent sql injection
    try:
        match dbtype:
            case "sqlite":
                query = f"select * from {table} where {args} {("limit " +str(maxrows) )if maxrows >=1 else ""} {("offset "+str(offset)) if offset >0 else ""}"
                res = DB.execute(query)
                output = res.fetchall()
                #for sqlite it adds the row number before so skip it
                output = [x[1:] for x in output]
                return output
            case _:
                log(f"database type not found {dbtype}!")
                raise f"database type not found {dbtype}!"
    except Exception as e:
        log(f"something went wrong searching in the database:{e}\n{table},{args}\n{query if query else ""}")
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
        log(f"something went wrong while inserting to database: {e}\n({table},{values}")
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
                    DB.execute(f"UPDATE {table} SET {','.join([x[0]+' = "'+str(x[1])+'"' for x in values.items()])} where {searchArgs}")
                    DB.commit()
                    return True
            case _:
                log(f"database type not found {dbtype}!")
                raise f"database type not found {dbtype}!"
    except Exception as e:
        log(f"something went wrong while updating table: {e}\n({table},{values},{searchArgs})")
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
        log(f"something went wrong while deleting from database: {e}\n({table},{args}")
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
        log(f"something went wrong while creating a table: {e}\n({table},{args}")
        return False
CheckConnection()