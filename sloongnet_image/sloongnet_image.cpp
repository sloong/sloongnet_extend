#include "sloongnet_image.h"

#include <univ/lua.h>

using namespace Sloong;
using namespace Sloong::Universal;

CLog* g_pLog = nullptr;

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

/*static int l_newJPEG(lua_State* l)
{
	CJPEG **p = (CJPEG**)lua_newuserdata(l, sizeof(CJPEG *));
	*p = new CJPEG;
	luaL_getmetatable(l, SLOONGNET_IMAGE_METHOD_NAME);
	lua_setmetatable(l, -2);
	return 1;
}

static int lua_auto_gc(lua_State* L)
{
	CJPEG **s = (CJPEG**)luaL_checkudata(L, 1, SLOONGNET_IMAGE_METHOD_NAME);
	if (s)
	{
		delete *s;
	}
	return 0;
}


static int lua_toString(lua_State* L)
{
	CJPEG* s = CJPEG::TryGet(L);
	if (s)
		lua_pushfstring(L, "Sloongnet extend model for mysql object");
	return 1;
}


static int Lua_setLog(lua_State* L)
{
	g_pLog = static_cast<CLog*>(CLua::GetPointer(L, 1));
	assert(g_pLog);
	return 0;
}*/

static const struct luaL_Reg sloongnet_image_Function[] =  
{
//	{ "new_jpeg",l_newJPEG },
	{ "GetJPEGThumbnail", Lua_getThumbImage },
//	{ "SetLog", Lua_setLog },
	{ NULL,NULL }
};

static const struct luaL_Reg sloongnet_image_Methods[] = 
{
//	{ "GetThumbnail", Lua_getThumbImage },
//	{ "__gc", lua_auto_gc },
//	{ "__tostring", lua_toString },
	{ NULL,NULL }
};

int luaopen_sloongnet_image_register(lua_State *L)
{
	luaL_newmetatable(L, SLOONGNET_IMAGE_METHOD_NAME);
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	luaL_setfuncs(L, sloongnet_image_Methods, 0);
	luaL_newlib(L, sloongnet_image_Function);
	return 1;
}

extern "C"
int luaopen_sloongnet_image(lua_State* L)
{
	luaL_requiref(L, "sloongnet_image", luaopen_sloongnet_image_register, 1);
	return 1;
}
