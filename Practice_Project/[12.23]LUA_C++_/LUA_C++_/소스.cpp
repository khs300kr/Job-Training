#include <iostream>
#include <string>
#include "include\ScriptInclude.h"
#include "lua_tinker.h"

using namespace std;


class Parent 
{
public:
	//virtual void func() = 0;
	virtual void func() { cout << "�θ� func �Լ� ȣ��!"; }
	//virtual const char* func() { return "this is parent"; }

};

class Child : public Parent 
{
public:
	virtual void func() { cout << "�ڽ� func �Լ� ȣ��!";; }
	//virtual const char* func() { return "this is child"; }
};

int main()
{
	lua_State* L = lua_open();
	luaopen_base(L);
	luaopen_string(L);

	// parent
	lua_tinker::class_add<Parent>(L, "Parent");
	lua_tinker::class_def<Parent>(L, "func", &Parent::func);

	// child
	lua_tinker::class_add<Child>(L, "Child");
	lua_tinker::class_inh<Child, Parent>(L);
	//lua_tinker::class_def<Child>(L, "func", &Child::func); // <-- �����.

	
	// variable
	Child chi;
	lua_tinker::set(L, "chi",&chi);		//child 

	// pParent
	//Parent* pParent = new Child;
	//lua_tinker::set(L, "par", pParent);	//pParent
	//delete pParent;
	
	// pParent get child point
	//Child chi;
	//Parent* pParent = &chi;
	//lua_tinker::set(L, "par", pParent);


	// sample3.lua ������ �ε�/�����Ѵ�.
	lua_tinker::dofile(L, "luas/virtual_test.lua");
	

	// ���α׷� ����
	lua_close(L);
	return 0;
}