# import traceback # use with traceback.print_exc() for more detailed error
import sqlite3
from settings import GetSetting,GetCurrentDir
from log import log
import os.path
from utils import checkSqlInjection, buildWhereQuery

dbtype = GetSetting("database.type")
DB :sqlite3.Connection = None
problem = None
def checkSqlInjectionInIter(itera)->None:
    """outputs error if a possible sql injection was found"""
    for x in itera:
        if (checkSqlInjection(x)):        
            raise Exception(f"possible sql injection detected in string: {x}")
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
def Search(table:str,filters:dict, maxrows:int =-1,offset:int=0,algo:str="exact")->list:
    """returns list of lines(tuples) found in database select query with the args as the string after where for example:
    select * from <table> where <args>;"""
    #convert everything to base64 to prevent sql injection - instead use a functionn to check for sql injection
    #the args are usually from buildwherequery
    args = buildWhereQuery(filters,algo=algo)
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
        # after all the checking outside check again for sql injections here
       
        checkSqlInjectionInIter(list(values.values()) + list(values.keys()))
        
        match dbtype:
            case "sqlite":
                    DB.execute(f"insert into {table} ({", ".join(list(values.keys()))}) values({", ".join(["?" for x in range(len(values))])})",list(values.values()))
                    DB.commit()
                    return True
            case _:
                log(f"database type not found {dbtype}!")
                raise f"database type not found {dbtype}!"
    except Exception as e:
        log(f"something went wrong while inserting to database: {e}\n\ttable:{table}\n\tvalues:{values}")
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
def getTableColumns(table:str)->list:
    try:
        match dbtype:
            case "sqlite":
                    res = DB.execute(f"pragma table_info({table})")
                    return [x[1] for x in res.fetchall()][1:] # ("<row number>,<column name>,<columns...>") first one will be ID which is default for sqlite in this present
            case _:
                log(f"database type not found {dbtype}!")
                raise f"database type not found {dbtype}!"
    except Exception as e:
        log(f"something went wrong while reading a table: {e}\n({table}")
        return []
def addAllColumns(table:list)->list:
    """tries to add all the columns in the table defenition(does not update columns types if the columns exist currently)
    and returnes a list of names of the columns added"""
    output = []
    try:
        for column_name in table[1]:
            match dbtype:
                case "sqlite":
                    try:
                        DB.execute(f"alter table {table[0]} add column {column_name} {table[1][column_name]}")
                        DB.commit()
                        output.append(column_name)
                    except Exception as e:
                        # log(e)
                        continue
                case _:
                    log(f"database type not found {dbtype}!")
                    raise f"database type not found {dbtype}!"
    except Exception as e:
        log(f"something went wrong while adding columns to a table: {e}\n({table[0]},{table[1]}")

def AddTable(table_name:str,args:dict,specialAttr:dict={})->bool:
    """CREATE TABLE table_name (
    column1 datatype,
    column2 datatype,
    column3 datatype,
    ....
    );"""
    try:
        match dbtype:
            case "sqlite":
                    existingColumns = getTableColumns(table_name)
                    if (len(existingColumns)>0 and set(existingColumns) != set(args.keys())): #appearently it checks for the items in lists and not the pointer
                        #need to add the columns
                        #its used only in development and upgrades but is not a key feature
                        #will error if the table in the database have too many columns(more then in "args")
                        addAllColumns([table_name,args])
                        log(f"table {table_name} was found outdated(missmatched number of columns {len(existingColumns)}!={len(args)})")
                        pass
                    DB.execute(f"CREATE TABLE IF NOT EXISTS {table_name} (ID INTEGER PRIMARY KEY AUTOINCREMENT,{','.join([' '.join(x) for x in list(args.items())+list(specialAttr.items())])});")
                    DB.commit()
                    return True
            case _:
                log(f"database type not found {dbtype}!")
                raise f"database type not found {dbtype}!"
    except Exception as e:
        log(f"something went wrong while creating a table: {e}\n({table_name},{args}")
        return False

from constants import ALLTABLES
def setupDatabase():
    CheckConnection()
    for table in ALLTABLES:
        AddTable(*table)