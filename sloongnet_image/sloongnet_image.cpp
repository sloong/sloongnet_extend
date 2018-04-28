#include "sloongnet_image.h"

#include <univ/lua.h>

using namespace Sloong;
using namespace Sloong::Universal;


int Lua_getThumbImage(lua_State* l)
{
	auto path = CLua::GetString(l, 1);
	auto width = CLua::GetDouble(l, 2);
	auto height = CLua::GetDouble(l, 3);
	auto quality = CLua::GetDouble(l, 4);
	auto folder = CLua::GetString(l, 5, "");
	
	if (access(path.c_str(), ACC_E) != -1)
	{
		if (folder == "")
			folder = path.substr(0, path.find_last_of('/'));

		string fileName = path.substr(path.find_last_of('/') + 1);
		string extension = fileName.substr(fileName.find_last_of('.') + 1);
		fileName = fileName.substr(0, fileName.length() - extension.length() - 1);
		string thumbpath = CUniversal::Format("%s/%s_%d_%d_%d.%s", folder, fileName, width, height, quality, extension);
		CUniversal::CheckFileDirectory(thumbpath);
		if (access(thumbpath.c_str(), ACC_E) != 0)
		{
			CImg<UCHAR> img(path.c_str());
			double ratio = (double)img.width() / (double)img.height();
			if (ratio > 1.0f)
			{
				height = width / ratio;
			}
			else
			{
				width = height * ratio;
			}
			if (width == 0 || height == 0)
			{
				CLua::PushString(l, path);
				return 1;
			}
			img.resize(width, height);
			img.save(thumbpath.c_str());
		}
		CLua::PushString(l, thumbpath);
		return 1;
	}
	CLua::PushString(l, "");
	return 1;
}


static const struct luaL_Reg sloongnet_image_Methods[] =  //������Ԫ����
{
	{ "GetThumbnail", Lua_getThumbImage },
	{ NULL, NULL }
};



extern "C"
int luaopen_sloongnet_image(lua_State* L)
{
	luaL_register(L, "sloongnet_image", sloongnet_image_Methods);
	return 1;
}