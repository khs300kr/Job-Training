#include <iostream>
#include "include/scriptinclude.h"
#include "lua_tinker.h"
using namespace std;

void main()
{
	// Lua �� �ʱ�ȭ �Ѵ�.
	lua_State* L = lua_open();

	luaL_openlibs(L);
	lua_tinker::enum_stack(L);


	lua_tinker::dofile(L, "../x64/Debug/main.lua");


	// ���α׷� ����
	lua_close(L);

}
