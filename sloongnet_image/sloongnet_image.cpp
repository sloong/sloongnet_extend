#include "sloongnet_image.h"

#include <univ/lua.h>

using namespace Sloong;
using namespace Sloong::Universal;

enum ImageFormat{
	JPEG,
	PNG,
	GIF,
	BMP,
	WEBP,
}

CLog* g_pLog = nullptr;

/// 获取缩略图
// 参数：1，源文件路径
//		2，目标文件路径
//		3，宽
//		4，高
int Lua_getThumbImage(lua_State* l)
{
	auto src_path = CLua::GetString(l, 1);
	auto save_path = CLua::GetString(l, 2);
	auto width = CLua::GetDouble(l, 3);
	auto height = CLua::GetDouble(l, 4);
	
	if( save_path == "")
	{
		CLua::PushBoolen(l,false);
		CLua::PushString(l,"Save path is empty");
		return 2;
	}

	if (access(src_path.c_str(), ACC_E) == -1)
	{
		CLua::PushBoolen(l,false);
		CLua::PushString(l,"Cannot read the file.");
		return 2;
	}

	
	
	if (access(thumbpath.c_str(), ACC_W) != 0)
	{
		CUniversal::CheckFileDirectory(save_path);

		string str_cmd = CUniversal::Format("convert -sample %dx%d %s %s", width, height, src_path.c_str(), save_path.c_str());

		if( CUniversal::RunSystemCmd(str_cmd))
		{
			CLua::PushBoolen(l,true);
			CLua::PushString(l, thumbpath);
		}
		else
		{
			CLua::PushBoolen(l,false);
			CLua::PushString(l, "run convert cmd fialed.");
		}
	}
}


int lua_ResizeImage()
{
	auto src_path = CLua::GetString(l, 1);
	auto save_path = CLua::GetString(l, 2);
	auto width = CLua::GetDouble(l, 3);
	auto height = CLua::GetDouble(l, 4);
	auto quality = CLua::GetDouble(l, 5, -1);	
	
	if( save_path == "")
	{
		CLua::PushBoolen(l,false);
		CLua::PushString(l,"Save path is empty");
		return 2;
	}

	if (access(src_path.c_str(), ACC_E) == -1)
	{
		CLua::PushBoolen(l,false);
		CLua::PushString(l,"Cannot read the file.");
		return 2;
	}
	
	if (access(thumbpath.c_str(), ACC_W) != 0)
	{
		CUniversal::CheckFileDirectory(save_path);

		string str_cmd = CUniversal::Format("convert -resize %dx%d %s %s", width, height, src_path.c_str(), save_path.c_str());
		if ( quality > 0 )
		{
			str_cmd.append(CUniversal::Format(" -quality %d",quality))
		}
		if( CUniversal::RunSystemCmd(str_cmd))
		{
			CLua::PushBoolen(l,true);
			CLua::PushString(l, thumbpath);
		}
		else
		{
			CLua::PushBoolen(l,false);
			CLua::PushString(l, "run convert cmd fialed.");
		}
	}
	
	return 2;
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

/*////////////////////////////
需要支持的功能有：
	jpg,png,gif,bmp,webp 缩放
	jpg,png,gif,bmp 转换到webp
////////////////////////////*/
static const struct luaL_Reg sloongnet_image_Function[] =  
{
//	{ "new_jpeg",l_newJPEG },
	{ "GetThumbImage", Lua_getThumbImage },
	{ "Resize", lua_ResizeImage },
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
