#include "include/scriptinclude.h"
#include "lua_tinker.h"
#include <string>
#include <iostream>

using namespace std;


// Sample1

// lua_tinker::def									 ------    C++함수를 Stack에 등록
// lua_tinker::call<datatype>(L,"함수이름",인자)		 ------    Lua함수를 C++에서 실행

//int cpp_func(int arg1, int arg2)
//{
//	return arg1 + arg2;
//}
//
//int main()
//{
//	// Lua 를 초기화 한다.
//	lua_State* L = lua_open();
//
//	// Lua 기본 함수들을 로드한다.- print() 사용
//	luaopen_base(L);
//
//	// LuaTinker 를 이용해서 함수를 등록한다.
//	lua_tinker::def(L, "cpp_func", cpp_func);
//
//	// sample1.lua 파일을 로드/실행한다.
//	lua_tinker::dofile(L, "LuaTinker-master/sample1.lua");
//
//	// sample1.lua 의 함수를 호출한다.
//	int result = lua_tinker::call<int>(L, "lua_func", 3, 4);
//
//	// lua_func(3,4) 의 결과물 출력
//	printf("lua_func(3,4) = %d\n", result);
//
//	// 프로그램 종료
//	lua_close(L);
//
//	return 0;
//}


//--------------------------------------------------------------------------------------------
// Sample2

// lua_tinker::set									------	  C++의 전역변수를 전역 스택에
// lua_tinker::get<datatype>						------	  Lua의 전역변수를 전역 스택에

static int cpp_int = 100;
int main()
{
	// Lua 를 초기화 한다.
	lua_State* L = lua_open();

	// Lua 기본 함수들을 로드한다.- print() 사용
	luaopen_base(L);
	luaopen_string(L);

	// LuaTinker 를 이용해서 cpp_int 를 Lua에 전달
	lua_tinker::set(L, "cpp_int", cpp_int);

	// sample1.lua 파일을 로드/실행한다.
	lua_tinker::dofile(L, "LuaTinker-master/sample2.lua");

	// sample1.lua 의 함수를 호출한다.
	string lua_int = lua_tinker::get<string>(L, "lua_int");

	// lua_int 값 출력
	printf("lua_int = %s\n", lua_int);

	// 프로그램 종료
	lua_close(L);

	return 0;
}


//--------------------------------------------------------------------------------------------
// Sample3

// lua_tinker::class함수

//struct A
//{
//	A(int v) : value(v) {}
//	int value;
//};
//
//struct base
//{
//	base() {}
//
//	const char* is_base() { return "this is base"; }
//};
//
//class test : public base
//{
//public:
//	test(int val) : _test(val) {}
//	~test() {}
//
//	const char* is_test() { return "this is test"; }
//
//	void ret_void() {}
//	int ret_int() { return _test; }
//	int ret_mul(int m) const { return _test * m; }
//	A get() { return A(_test); }
//	void set(A a) { _test = a.value; }
//
//	int _test;
//};
//
//test g_test(11);
//
//int main()
//{
//	// Lua 를 초기화 한다.
//	lua_State* L = lua_open();
//
//	// Lua 기본 함수들을 로드한다.- print() 사용
//	luaopen_base(L);
//	// Lua 문자열 함수들을 로드한다.- string 사용
//	luaopen_string(L);
//
//	// base 클래스를 Lua 에 추가한다.
//	lua_tinker::class_add<base>(L, "base");
//	// base 의 함수를 등록한다.
//	lua_tinker::class_def<base>(L, "is_base", &base::is_base);
//
//	// test 클래스를 Lua 에 추가한다.
//	lua_tinker::class_add<test>(L, "test");
//	// test 가 base 에 상속 받았음을 알려준다.
//	lua_tinker::class_inh<test, base>(L);
//	// test 클래스 생성자를 등록한다.
//	lua_tinker::class_con<test>(L, lua_tinker::constructor<test, int>);
//	// test 의 함수들을 등록한다.
//	lua_tinker::class_def<test>(L, "is_test", &test::is_test);
//	lua_tinker::class_def<test>(L, "ret_void", &test::ret_void);
//	lua_tinker::class_def<test>(L, "ret_int", &test::ret_int);
//	lua_tinker::class_def<test>(L, "ret_mul", &test::ret_mul);
//	lua_tinker::class_def<test>(L, "get", &test::get);
//	lua_tinker::class_def<test>(L, "set", &test::set);
//	lua_tinker::class_mem<test>(L, "_test", &test::_test);
//
//	// Lua 전역 변수호 g_test 의 포인터를 등록한다.
//	lua_tinker::set(L, "g_test", &g_test);
//
//	// sample3.lua 파일을 로드/실행한다.
//	lua_tinker::dofile(L, "LuaTinker-master/sample3.lua");
//
//	// 프로그램 종료
//	lua_close(L);
//
//	return 0;
//}
//



//--------------------------------------------------------------------------------------------
// Sample4

// C++에서 Table 데이터 타입을 사용할 수 있으며 (get,set함수 호출가능)
// lua에서 함수를 정의하고, C++ 에서 call를 하여 Table을 조작 할 수 있다.


//int main()
//{
//	// Lua 를 초기화 한다.
//	lua_State* L = lua_open();
//
//	// Lua 기본 함수들을 로드한다.- print() 사용
//	luaopen_base(L);
//
//	// Lua 테이블을 생성하고 스택에 푸쉬한다.
//	lua_tinker::table haha(L, "haha");
//
//	// haha.value = 1 값을 넣는다.
//	haha.set("value", 1);
//
//	// table 내에 table을 만들어 넣는다.
//	haha.set("inside", lua_tinker::table(L));
//
//	// haha.inside 의 포인터를 스택위로 복사한다.
//	lua_tinker::table inside = haha.get<lua_tinker::table>("inside");
//
//	// inside.value = 2 값을 넣는다.
//	inside.set("value", 2);
//
//	// sample4.lua 파일을 로드/실행한다.
//	lua_tinker::dofile(L, "LuaTinker-master/sample4.lua");
//
//	// Lua 에서 생성한 haha.test 값을 읽는다.
//	const char* test = haha.get<const char*>("test");
//	printf("haha.test = %s\n", test);
//
//	// 전역에 등록하지 않고 Lua 스택에 빈 테이블을 생성한다.(지역변수)
//	lua_tinker::table temp(L);
//
//	// 빈 테이블.name 에 값을 넣는다.
//	temp.set("name", "local table !!");
//
//	// table을 의 인자로 사용하여 print_table 을 호출한다.
//	lua_tinker::call<void>(L, "print_table", temp);
//
//	// 함수가 리턴하는 table을 받는다.
//	lua_tinker::table ret = lua_tinker::call<lua_tinker::table>(L, "return_table", "give me a table !!");
//	printf("ret.name =\t%s\n", ret.get<const char*>("name"));
//
//	// 프로그램 종료
//	lua_close(L);
//
//	return 0;
//}
//



//--------------------------------------------------------------------------------------------
// Sample5

// Error와 Stack출력
//void show_error(const char* error)
//{
//	printf("_ALERT -> %s\n", error);
//}
//
//int main()
//{
//	// Lua 를 초기화 한다.
//	lua_State* L = lua_open();
//
//	// Lua 기본 함수들을 로드한다.- print() 사용
//	luaopen_base(L);
//
//
//	// lua_State* 가 갖고있는 현재의 스택을 보여준다.
//	printf("%s\n", "-------------------------- current stack");
//	lua_tinker::enum_stack(L);
//
//	// 스택에 1을 밀어넣는다.
//	lua_pushnumber(L, 1);
//
//	// 현재 스택의 내용을 다시 출력한다.
//	printf("%s\n", "-------------------------- stack after push '1'");
//	lua_tinker::enum_stack(L);
//
//
//	// sample5.lua 파일을 로드/실행한다.
//	lua_tinker::dofile(L, "LuaTinker-master/sample5.lua");
//
//	// test_error() 함수를 호출한다.
//	// test_error() 는 실행중 test_error_3() 호출을 시도하다 에러를 발생시킨다.
//	// 함수 호출중 발생한 에러는 printf()를 통해서 출력된다.
//	printf("%s\n", "-------------------------- calling test_error()");
//	lua_tinker::call<void>(L, "test_error");
//
//	// test_error_3()는 존재하지 않는 함수이다. 호출 자체가 실패한다.
//	printf("%s\n", "-------------------------- calling test_error_3()");
//	lua_tinker::call<void>(L, "test_error_3");
//
//	// printf() 대신 유저가 제공하는 에러 출력 루틴을 사용할 수 있다.
//	// 이 에러처리 함수는1개의 루아 문자열로 발생한 에러를 전달하게 된다.
//	// C++ 에서 등록할 경우 void function(const char*) 형태가 적합하다.
//	lua_tinker::def(L, "_ALERT", show_error);
//
//	lua_tinker::call<void>(L, "_ALERT", "test !!!");
//
//	// test_error() 함수를 호출한다.
//	// 함수 호출중 발생한 에러는 Lua에 등록된 _ALERT()를 통해서 출력된다.
//	printf("%s\n", "-------------------------- calling test_error()");
//	lua_tinker::call<void>(L, "test_error");
//
//
//	// 프로그램 종료
//	lua_close(L);
//
//	return 0;
//}




//--------------------------------------------------------------------------------------------
// Sample6 (Thread)

// 함수 형태가 int(*)(lua_State*) 또는 int(*)(lua_State*,T1) 일 경우만 lua_yield() 를 쓸 수 있다.
// 함수 인자가 더 필요할 경우 lua_tinker.h 의 "functor (non-managed)" 코멘트 부분을 참고해서 직접 추가할 것
//int TestFunc(lua_State* L)
//{
//	printf("# TestFunc 실행중\n");
//	return lua_yield(L, 0);
//}
//
//int TestFunc2(lua_State* L, float a)
//{
//	printf("# TestFunc2(L,%f) 실행중\n", a);
//	return lua_yield(L, 0);
//}
//
//class TestClass
//{
//public:
//
//	// 함수 형태가 int(T::*)(lua_State*) 또는 int(T::*)(lua_State*,T1) 일 경우만 lua_yield() 를 쓸 수 있다.
//	// 함수 인자가 더 필요할 경우 lua_tinker.h 의 "class member functor (non-managed)" 코멘트 부분을 참고해서 직접 추가할 것
//	int TestFunc(lua_State* L)
//	{
//		printf("# TestClass::TestFunc 실행중\n");
//		return lua_yield(L, 0);
//	}
//
//	int TestFunc2(lua_State* L, float a)
//	{
//		printf("# TestClass::TestFunc2(L,%f) 실행중\n", a);
//		return lua_yield(L, 0);
//	}
//};
//
//int main()
//{
//	// Lua 를 초기화 한다.
//	lua_State* L = lua_open();
//
//	// Lua 기본 함수들을 로드한다.- print() 사용
//	luaopen_base(L);
//	// Lua 문자열 함수들을 로드한다.- string 사용
//	luaopen_string(L);
//
//	// TestFunc 함수를 Lua 에 등록한다.
//	lua_tinker::def(L, "TestFunc", &TestFunc);
//	lua_tinker::def(L, "TestFunc2", &TestFunc2);
//
//	// TestClass 클래스를 Lua 에 추가한다.
//	lua_tinker::class_add<TestClass>(L, "TestClass");
//	// TestClass 의 함수를 등록한다.
//	lua_tinker::class_def<TestClass>(L, "TestFunc", &TestClass::TestFunc);
//	lua_tinker::class_def<TestClass>(L, "TestFunc2", &TestClass::TestFunc2);
//
//	// TestClass 를 전역 변수로 선언한다.
//	TestClass g_test;
//	lua_tinker::set(L, "g_test", &g_test);
//
//	// sample3.lua 파일을 로드한다.
//	lua_tinker::dofile(L, "sample6.lua");
//
//	// Thread 를 시작한다.
//	lua_newthread(L);
//	lua_pushstring(L, "ThreadTest");
//	lua_gettable(L, LUA_GLOBALSINDEX);
//
//	// Thread 를 시작한다.
//	printf("* lua_resume() 호출\n");
//	lua_resume(L, 0);
//
//	// Thread 를 다시 시작한다.
//	printf("* lua_resume() 호출\n");
//	lua_resume(L, 0);
//
//	// Thread 를 다시 시작한다.
//	printf("* lua_resume() 호출\n");
//	lua_resume(L, 0);
//
//	// Thread 를 다시 시작한다.
//	printf("* lua_resume() 호출\n");
//	lua_resume(L, 0);
//
//	// Thread 를 다시 시작한다.
//	printf("* lua_resume() 호출\n");
//	lua_resume(L, 0);
//
//	// 프로그램 종료
//	lua_close(L);
//
//	return 0;
//}
//
//
//
