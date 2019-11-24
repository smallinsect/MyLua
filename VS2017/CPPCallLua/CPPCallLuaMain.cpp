
#include <stdio.h>

#include "../luasrc/lua.hpp"

int main(int argc, char* argv[]){

	char szFile[1024];
	char szFunc[1024];
	char szArg1[1024];

	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	while (true) {
		printf("test.lua func arg\n");
		printf(">>");
		scanf("%s%s%s", szFile, szFunc, szArg1);
		luaL_dofile(L, szFile);
		lua_getglobal(L, szFunc);
		lua_pushstring(L, szArg1);

		lua_call(L, 1, 1);

		printf("%s\n", lua_tostring(L, -1));
	}

	return 0;
}
