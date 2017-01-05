#include <iostream>
#include "include/scriptinclude.h"
#include "lua_tinker.h"
using namespace std;

void main()
{
	// Lua 를 초기화 한다.
	lua_State* L = lua_open();

	luaL_openlibs(L);
	lua_tinker::enum_stack(L);


	lua_tinker::dofile(L, "../x64/Debug/main.lua");


	// 프로그램 종료
	lua_close(L);

}
