import hashlib

HASHINGALGO = hashlib.sha512
REMEMBERTOKENTIME = 5*60 # 5 mins
MAXREGISTERWAIT = 60 # 1 min
#tables: Users, Carmodules
USERS_TABLE = ["Users",
            {"username":"varchar(255) not null UNIQUE"
             ,"password":"varchar(128) not null"
             ,"token":"varchar(30)"
             ,"permissions_level":"INT DEFAULT 0"
             ,"token_date":"INT DEFAULT 0"
             ,"date_created":"INT DEFAULT 0"}]
CARMODULES_TABLE = ["Carmodules",
                    {"id":"varchar(30) not null UNIQUE"
                    ,"password":"varchar(128) not null",
                    "availablePins":"char(20)"}] #pin number as char for each char for example ab for pins 61,62
CONTROLLERSCOMMANDS_TABLE = ["ControllerCommands",{"ControllerId":"varchar(30)",
                                                   "title":"varchar(50)",
                                                   "actions":"varchar(255)",
                                                   "CONSTRAINT constraint_name ":"UNIQUE(ControllerId,title)"}]
ALLTABLES = [USERS_TABLE,CARMODULES_TABLE,CONTROLLERSCOMMANDS_TABLE]