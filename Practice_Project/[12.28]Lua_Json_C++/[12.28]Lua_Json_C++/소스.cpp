#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

#include "include/scriptinclude.h"
#include "lua_tinker.h"

#include <iostream>
#include <string>
#include <list>

using namespace std;
using namespace rapidjson;
struct MyHandler : public BaseReaderHandler<UTF8<>, MyHandler>
{
	bool Null() { cout << "Null()" << endl; return true; }
	bool Bool(bool b) { cout << "Bool(" << boolalpha << b << ")" << endl; return true; }
	bool Int(int i) { cout << "Int(" << i << ")" << endl; return true; }
	bool Uint(unsigned u) { cout << "Uint(" << u << ")" << endl; return true; }
	bool Int64(int64_t i) { cout << "Int64(" << i << ")" << endl; return true; }
	bool Uint64(uint64_t u) { cout << "Uint64(" << u << ")" << endl; return true; }
	bool Double(double d) { cout << "Double(" << d << ")" << endl; return true; }
	bool String(const char* str, SizeType length, bool copy) {
		cout << "String(" << str << ", " << length << ", " << boolalpha << copy << ")" << endl;
		return true;
	}
	bool StartObject() { cout << "StartObject()" << endl; return true; }
	bool Key(const char* str, SizeType length, bool copy) {
		cout << "Key(" << str << ", " << length << ", " << boolalpha << copy << ")" << endl;
		return true;
	}
	bool EndObject(SizeType memberCount) { cout << "EndObject(" << memberCount << ")" << endl; return true; }
	bool StartArray() { cout << "StartArray()" << endl; return true; }
	bool EndArray(SizeType elementCount) { cout << "EndArray(" << elementCount << ")" << endl; return true; }
};



class Bullet
{
public:
	int		m_ID;
	string	m_Job;
public:
	Bullet(int _m_ID, string _m_Job)
		: m_ID(_m_ID), m_Job(_m_Job) {}
	Bullet() {}
public:

};


class Player
{
private:
	int			m_ID;
	Bullet		b[10];
public:
	Player(int _m_ID, Bullet _b)
		: m_ID(_m_ID)
	{
		for (int i = 0; i < 10; ++i)
		{
			b[i] = Bullet(i,_b.m_Job);
		}
	}
public:
	template<typename Writer>
	void Serialize(Writer& writer)const
	{
		// Player_Info
		writer.StartObject();
		writer.String("Player_m_ID");
		writer.Int(m_ID);
		// Bullet_Info
		writer.String("B");
		writer.StartArray();
		for (int i = 0; i < 10; ++i)
		{
			writer.StartObject();

			writer.String("Bullet_m_ID");
			writer.Int(b[i].m_ID);
			writer.String("Bullet_m_");
			writer.String(b[i].m_Job.c_str(), static_cast<SizeType>(b[i].m_Job.length()));

			writer.EndObject();
		}

		writer.EndArray();

		writer.EndObject();
	}
	string serialize(void)
	{
		StringBuffer s;
		PrettyWriter<StringBuffer> writer(s);
		Serialize(writer);
		return s.GetString();
	}

};


// Writer - ex) C++ 데이터를 writer를 통해 DOM-TREE에 등록
// 그것을 JSON화 시켜등록한다.



void main()
{
	lua_State* L = lua_open();
	// Lua 기본 함수들을 로드한다.- print() 사용

	luaL_openlibs(L);


	Player *p = new Player(1, Bullet(1, "War"));

	string str_JSON = p->serialize();
	cout << "------------------------C++클래스->JSON---------------------------------" << endl;
	cout << str_JSON << endl;
	
	
	lua_tinker::dofile(L, "TPassive.lua");

	lua_tinker::table dataTable = lua_tinker::call<lua_tinker::table>(L,"EncodeTable", str_JSON.c_str());


	// Table -> Json Logic
	//string return_JSON = lua_tinker::call<char *>(L, "EncodeJson", dataTable);
	//cout << "------------------------Table->JSON---------------------------------" << endl;
	//cout << return_JSON << endl;

	delete p;
	// 프로그램 종료
	lua_close(L);
}


