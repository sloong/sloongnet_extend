require "sloongnet_mysql"

local Sql_Req = {};

local db = sloongnet_mysql.new();
local db2 = sloongnet_mysql.new();
local res,msg = db:Connect('10.0.0.20',3306,'dev','develop','dev');
db2:Connect('10.0.0.20',3306,'dev','develop','dev');

print(msg)
local res,num = db:Query("INSERT INTO `styles` ( `name`,`parent`,`subimage`,`creater` ) VALUES ( 'test2','1',0,'' )");
--db2:Query("INSERT INTO `styles` ( `name`,`parent`,`subimage`,`creater` ) VALUES ( 'test2','1',0,'' )");
print(res)
print(num)
res, num=db:Query("SELECT @@IDENTITY")
db2:Query("SELECT @@IDENTITY")

print(res)
print(num)
res, num=db2:Query("SELECT @@IDENTITY")
print(num)
