require "sloongnet_mysql"

local Sql_Req = {};

local db = sloongnet_mysql.new();
local res,msg = db:Connect('10.0.0.20',3306,'walls_dev','sloong','walls_dev');
print(msg);


