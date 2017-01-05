#pragma once
#include "TGameObject.h"
class TUser :
	public TGameObject
{
public:
	TUser();
	~TUser();
private:
	int m_Age;

public:
	static void published()
	{
		lua_tinker::class_add<TUser>(L, "TUser");
		lua_tinker::class_mem<TUser>(L, "Age", &TUser::m_Age);
		return;
	}
};

TUser::published();


