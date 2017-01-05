#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

#include "include/scriptinclude.h"
#include "lua_tinker.h"

#include <iostream>
#include <string>

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

// Reader - JSON 파일을 Stream 형태로 변환 시켜준다.
// Event 나열

class PassiveSkill
{
private:
	int sID;
	string sJob;
	double sCoolTime;

public:
	PassiveSkill(int _sID, string _sJob, float _sCoolTime)
		: sID(_sID), sJob(_sJob), sCoolTime(_sCoolTime) {}
	PassiveSkill() {}
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
		writer.EndObject();
	}
	string serialize(void)
	{
		StringBuffer s;
		PrettyWriter<StringBuffer> writer(s);
		Serialize(writer);
		return s.GetString();
	}
public:

};

void main() 
{
	//const char json[] = " { \"hello\" : \"world\", \"t\" : true , \"f\" : false, \"n\": null, \"i\":123, \"pi\": 3.1416, \"a\":[1, 2, 3, 4] } ";

	lua_State* L = lua_open();

	luaL_openlibs(L);
	lua_tinker::dofile(L, "TPassive.lua");

	char* json = lua_tinker::call<char *>(L, "EncodeJson");

	MyHandler handler;
	Reader reader;
	StringStream ss(json);
	reader.Parse(ss, handler);


	
}

