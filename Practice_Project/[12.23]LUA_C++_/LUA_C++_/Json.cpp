#include <iostream>
#include "include\ScriptInclude.h"
#include "lua_tinker.h"

using namespace std;

//void luaInitialize(lua_State* L)
//{
//	lua_tinker::def(L, "require", my_require);    // ��ư� ���� �Լ��� �����ǰ� ����
//
//	lua_tinker::dostring(L, "JSON/main.lua");
//}
//
//int my_require(lua_State* state)
//{
//	const char* name = lua_tostring(state, 1);    // require ������ ������ �����´�
//	const char* relpaths[] =                      // ��� ��� ������ ����
//	{
//		"Lua/SubPath/%s.lua",
//		"Lua/TowPath/%s.lua",
//	};
//
//	for (int i = 0; i < _countof(relpaths); ++i)
//	{
//		TCHAR bf[256] = { 0 };
//		_stprintf(bf, (TCHAR*)relpaths[i], name);          // ��� ��θ� �����Ͽ� ��� ����
//		
//		string script = ::getScript(bf);
//		auto err = luaL_loadbuffer(state, script.c_str(), script.size(), bf);
//		if (err)
//			lua_error(state);
//		break;
//	}
//
//	lua_call(state, 0, 1);    // �ٽ�. �̰� ���� 2�ð� ��̴�.
//	return 1;
//}


int main()
{
	// Lua �� �ʱ�ȭ �Ѵ�.
	lua_State* L = lua_open();

	//luaopen_base(L);
	//luaopen_io(L);
	//luaopen_package(L);

	//luaopen_package(L);
	luaopen_base(L);
	//luaopen_io(L);
	luaL_openlibs(L);
	lua_tinker::enum_stack(L);
	

	lua_tinker::dofile(L, "LUA TextRPG 0.1/main.lua");
	
	//luaL_dofile(L, "LUA TextRPG 0.1/main.lua");
	//luaL_dofile(L, "JSON/main.lua");
	//lua_tinker::enum_stack(L);

	// ���α׷� ����
	lua_close(L);
	
	return 0;
}

