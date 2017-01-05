#include <iostream>
#include <string>
// 
#include "cpp_redis/cpp_redis"
//
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

using namespace std;
using namespace cpp_redis;
using namespace rapidjson;

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
			b[i] = Bullet(i, _b.m_Job);
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

void main()
{
	// CLASS -> JSON
	Player* pPlayer = new Player(1, Bullet(1, "War"));
	string Player_JSON = pPlayer->serialize();
	
	// Redis_Client Init
	redis_client client;
	client.connect();
	client.set("Player:Val", Player_JSON);
	client.get("Player:Val", [](reply& reply) {
		cout << reply << endl;
	});
	
	client.hset("user:1", "hp","5");
	client.hget("user:1","username", [](reply& reply) {
		cout << reply << endl;
	});
	client.commit();
	while (true) {}
}	



