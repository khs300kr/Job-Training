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

class PassiveSkill
{
private:
	int sID;
	string sJob;
	double sCoolTime;
	int bullet[2];

public:
	PassiveSkill(int _sID, string _sJob, float _sCoolTime,int b)
		: sID(_sID), sJob(_sJob), sCoolTime(_sCoolTime)
	{
		for (int i = 0; i < 2; ++i)
			bullet[i] = b;
	}
	PassiveSkill () {}
	virtual ~PassiveSkill() {}

public:
	template<typename Writer>
	void Serialize(Writer& writer) const
	{
		writer.StartObject();
		// sID
		writer.String("sID");
		writer.Int(sID);
		// sJob
		writer.String("sJob");
		writer.String(sJob.c_str(), static_cast<SizeType>(sJob.length()));
		// sCoolTime
		writer.String("sCoolTime");
		writer.Double(sCoolTime);
		// Array
		writer.Key("a");
		writer.StartArray();
		for (unsigned i = 0; i < 4; i++)
			writer.Uint(i);
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

//class SkillEffect : public PassiveSkill
//{
//private:
//	int Att;
//	int Def;
//	int bullet[2];
//public:
//	SkillEffect(int _sID, string _sJob, float _sCoolTime, int _Att, int _Def)
//		: PassiveSkill(_sID, _sJob, _sCoolTime), Att(_Att), Def(_Def) {}
//	SkillEffect(int _sID, string _sJob, float _sCoolTime, int b)
//		: PassiveSkill(_sID, _sJob, _sCoolTime) 
//	{
//		for (int i = 0; i < 2; ++i)
//		{
//			bullet[i] = b;
//		}
//
//	}
//	virtual ~SkillEffect() {}
//
//public:
//	template<typename Writer>
//	void Serialize(Writer& writer)const
//	{
//		writer.StartObject();
//		SkillEffect::Serialize(writer);
//		// Att
//		writer.String("Att");
//		writer.Int(Att);
//		// Def
//		writer.String("Def");
//		writer.Int(Def);
//
//
//		//
//		writer.EndObject();
//	}
//	string serialize(void)
//	{
//		StringBuffer s;
//		PrettyWriter<StringBuffer> writer(s);
//		Serialize(writer);
//		return s.GetString();
//	}
//};


// Writer - ex) C++ 데이터를 writer를 통해 DOM-TREE에 등록
// 그것을 JSON화 시켜등록한다.



void main()
{
	// Writer
	
	PassiveSkill* Str = new PassiveSkill(1, "War", 3.3,3);//, 30, 0);

	cout << Str->serialize() <<endl;

	delete Str;


	// Lua 를 초기화 한다.
	//lua_State* L = lua_open();

	//luaL_openlibs(L);
	//lua_tinker::dofile(L, "TPassive.lua");

	////string json = lua_tinker::get<char *>(L, "recvTable");

	//string json = lua_tinker::call<char *>(L,"EncodeJson");

	//	
	//cout << json << endl;

	//lua_tinker::table dataTable = lua_tinker::call<lua_tinker::table>(L,"EncodeTable",json.c_str());
	//cout << dataTable.get<int>("cCoolTime") << endl;
	//
	//
	//// 프로그램 종료 
	//lua_close(L);
}


