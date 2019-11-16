
#include <windows.h>

#include "..\luasrc\lua.hpp"

static int util_sums(lua_State *L) {
	int number = lua_gettop(L);//获取栈顶上的元素个数

	lua_Number sum = 0.0;//累计参数和
	for (int i = 1; i <= number; ++i) {
		if (!lua_isnumber(L, i)) {//检测
			lua_pushliteral(L, "参数有误");
			lua_error(L);
		}
		sum += lua_tonumber(L, i);
	}

	lua_pushnumber(L, sum);
	lua_pushstring(L, "爱白菜的小昆虫");

	return 2;//返回值的个数
}

static const luaL_Reg util[] = {
	{"xqlSums", util_sums},
	{NULL, NULL}
};

extern "C" __declspec(dllexport)
int luaopen_MyUtil(lua_State *L) {
	luaL_newlib(L, util);
	return 1;
}