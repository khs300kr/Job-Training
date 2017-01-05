#include<iostream>
#include "include/scriptinclude.h"
#include "lua_tinker.h"

using namespace std;

// member struct
typedef struct tINFO
{
	int		m_iX;
	int		m_iY;
	tINFO() {}
	tINFO(int x, int y)
		: m_iX(x), m_iY(y) {}
}INFO;


// CObject class
class CObject
{
protected:
	INFO	m_Info;
	int		m_iHP;
	int		m_iDamage;
public:
	CObject() {}
	virtual ~CObject() {}
public:
	INFO	get_INFO() { return m_Info; }
	int		get_HP() { return m_iHP; }
	int		get_Damage() { return m_iDamage; }
public:
	virtual void set_Damage(int _iDamage) = 0;
public:
	virtual void Initialize() = 0;
};

// CPlayer class
class CPlayer : public CObject
{
private:
	int m_iCount;
public:
	CPlayer(void) {}
	~CPlayer(void) {}
public:
	virtual void set_Damage(int _iDamage)
	{
		m_iHP -= _iDamage;
	}
public:
	virtual void Initialize()
	{
		m_Info = INFO(10, 10);
		m_iHP = 100;
		m_iDamage = 5;
	}
};

int main()
{
	// ��Ӱ�ü
	CObject* pPlayer = new CPlayer;
	pPlayer->Initialize();
	// Just ��ü
	CPlayer Player;
	Player.Initialize();


	// Lua �� �ʱ�ȭ �Ѵ�.
	lua_State* L = lua_open();
	luaopen_base(L);
	luaopen_string(L);

	
	// �θ����ڽ��� publish�ϸ� �ڽ��� ���ǥ�ø� �ϸ� �ȴ�.
	//=================== Parent ===================
	lua_tinker::class_add<CObject>(L, "CObject");
	// �Լ�
	lua_tinker::class_def<CObject>(L, "get_INFO", &CObject::get_HP);			// non-virtual
	lua_tinker::class_def<CObject>(L, "get_HP", &CObject::get_HP);				// non-virtual
	lua_tinker::class_def<CObject>(L, "get_Damage", &CObject::get_Damage);		// non-virtual
	lua_tinker::class_def<CObject>(L, "set_Damage", &CObject::set_Damage);		// virtual
	lua_tinker::class_def<CObject>(L, "Initialize", &CObject::Initialize);		// virtual

	lua_tinker::class_add<CObject>(L, "CObject");
	// �Լ�
	lua_tinker::class_def<CObject>(L, "get_INFO", &CObject::get_HP);			// non-virtual
	lua_tinker::class_def<CObject>(L, "get_HP", &CObject::get_HP);				// non-virtual
	lua_tinker::class_def<CObject>(L, "get_Damage", &CObject::get_Damage);		// non-virtual
	lua_tinker::class_def<CObject>(L, "set_Damage", &CObject::set_Damage);		// virtual
	lua_tinker::class_def<CObject>(L, "Initialize", &CObject::Initialize);		// virtual


	//=================== Child ===================
	lua_tinker::class_add<CPlayer>(L, "CPlayer");
	// ���
	lua_tinker::class_inh<CPlayer, CObject>(L);									// inherit


	// �����͸� &�� ������ ������, value

	// Variable -> Set
	lua_tinker::set(L, "pPlayer", pPlayer);		// Pointer
	lua_tinker::set(L, "Player", Player);		// Not_pointer

	lua_tinker::dofile(L, "luas/pointer_test.lua");

	lua_tinker::set(L, "pPlayer", pPlayer);		// Pointer

	// Varaible -> Get
	cout << "===Varaible -> Get====" << endl;
	cout << lua_tinker::get<int>(L, "a") << endl;
	cout << lua_tinker::get<int>(L, "b") << endl;

	// Just Funt Call
	cout << "===just Func Call===" << endl;
	cout << pPlayer->get_HP() << endl;
	cout << Player.get_HP() << endl;


	// ���α׷� ����
	lua_close(L);
	delete pPlayer;

	return 0;
}
