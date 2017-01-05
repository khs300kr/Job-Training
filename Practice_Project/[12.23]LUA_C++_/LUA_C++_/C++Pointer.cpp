#include<iostream>
#include "include/scriptinclude.h"
#include "lua_tinker.h"

using namespace std;


int cpp_func(int arg1, int arg2)
{
	return arg1 + arg2;
}

int main()
{
	// variables
	int* p;
	int a = 100;
	p = &a;

	// Lua 를 초기화 한다.
	lua_State* L = lua_open();
	luaopen_base(L);

	cout << "lua 가기전 : " << *p << endl;

	// set
	lua_tinker::set(L, "pointer", p);
	// do
	lua_tinker::dofile(L, "luas/pointer_test.lua");

	// get
	int* lua_int = lua_tinker::get<int*>(L, "did_something");

	cout << "lua 갔다옴 : " << *(lua_int) << endl;


	// 프로그램 종료
	lua_close(L);

	return 0;
}
