require "sloongnet_image"

local image_path = 'test.jpg';
local res = sloongnet_image.GetJPEGThumbnail(image_path,100,100,10,'/tmp');
print(res)

