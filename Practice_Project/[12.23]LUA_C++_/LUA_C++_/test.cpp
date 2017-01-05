#include <iostream>
#include "include\ScriptInclude.h"
#include "lua_tinker.h"


using namespace std;

void main()
{
	lua_State* L = lua_open();
	luaopen_base(L);


	lua_tinker::set(L, "value", 1);

	lua_tinker::dofile(L, "luas/test.lua");



	lua_close(L);
}