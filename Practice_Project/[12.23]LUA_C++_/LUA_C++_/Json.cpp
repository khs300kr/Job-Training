#include <iostream>
#include "include\ScriptInclude.h"
#include "lua_tinker.h"

using namespace std;

//void luaInitialize(lua_State* L)
//{
//	lua_tinker::def(L, "require", my_require);    // 루아가 만든 함수도 재정의가 가능
//
//	lua_tinker::dostring(L, "JSON/main.lua");
//}
//
//int my_require(lua_State* state)
//{
//	const char* name = lua_tostring(state, 1);    // require 다음의 파일을 가져온다
//	const char* relpaths[] =                      // 상대 경로 조합을 위한
//	{
//		"Lua/SubPath/%s.lua",
//		"Lua/TowPath/%s.lua",
//	};
//
//	for (int i = 0; i < _countof(relpaths); ++i)
//	{
//		TCHAR bf[256] = { 0 };
//		_stprintf(bf, (TCHAR*)relpaths[i], name);          // 상대 경로를 조합하여 경로 생성
//		
//		string script = ::getScript(bf);
//		auto err = luaL_loadbuffer(state, script.c_str(), script.size(), bf);
//		if (err)
//			lua_error(state);
//		break;
//	}
//
//	lua_call(state, 0, 1);    // 핵심. 이걸 몰라서 2시간 헤맸다.
//	return 1;
//}


int main()
{
	// Lua 를 초기화 한다.
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

	// 프로그램 종료
	lua_close(L);
	
	return 0;
}

