#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace rapidjson;
using namespace std;

// Person
class Person
{
private:
	string name;
	unsigned age;
public:
	Person(const string& _name, unsigned _age)
		:name(_name), age(_age) {}
	Person(const Person& rhs)
		: name(rhs.name), age(rhs.age) {}
	virtual ~Person() {};

	Person& operator=(const Person& rhs)
	{
		name = rhs.name;
		age = rhs.age;
		return *this;
	}
protected:
	template<typename Writer>
	void Serialize(Writer& writer)const
	{
		writer.String("name");
#if RAPIDJSON_HAS_STDSTRING
		writer.String(name);
#else
		writer.String(name.c_str(), static_cast<SizeType>(name.length()));
#endif
		writer.String("age");
		writer.Uint(age);
	}
};


// Education
class Education
{
private:
	string school;
	double GPA;

public:
	Education(const string& _school, double _GPA)
		: school(_school), GPA(_GPA) {}
	Education(const Education& rhs)
		: school(rhs.school), GPA(rhs.GPA) {}

	template <typename Writer>
	void Serialize(Writer& writer) const
	{
		writer.StartObject();
		writer.String("school");
#if RAPIDJSON_HAS_STDSTRING
		writer.String(school);
#else
		writer.String(school.c_str(), static_cast<SizeType>(school.length()));
#endif
		writer.String("GPA");
		writer.Double(GPA);
		writer.EndObject();
	}
};


// Dependent
class Dependent : public Person
{
private:
	Education *education;
public:
	Dependent(const string& name, unsigned age, Education* _education = 0) 
		: Person(name, age), education(_education) {}
	Dependent(const Dependent& rhs) 
		: Person(rhs), education(0) 
	{ 
		// 
		education = (rhs.education == 0) ? 0 : new Education(*rhs.education); 
	}
	virtual ~Dependent() {};

	Dependent& operator=(const Dependent& rhs) 
	{
		if (this == &rhs)
			return *this;
		delete education;
		education = (rhs.education == 0) ? 0 : new Education(*rhs.education);
		return *this;
	}

	template <typename Writer>
	void Serialize(Writer& writer) const 
	{
		writer.StartObject();
		Person::Serialize(writer);
		writer.String("education");
		if (education)
			education->Serialize(writer);
		else
			writer.Null();
		writer.EndObject();
	}
};

// Employee
class Employee :public Person
{
private:
	vector<Dependent> dependents_;
	bool married_;

public:
	Employee(const std::string& name, unsigned age, bool married)
		: Person(name, age), dependents_(), married_(married) {}
	Employee(const Employee& rhs)
		: Person(rhs), dependents_(rhs.dependents_), married_(rhs.married_) {}
	virtual ~Employee() {};

	Employee& operator=(const Employee& rhs) {
		static_cast<Person&>(*this) = rhs;
		dependents_ = rhs.dependents_;
		married_ = rhs.married_;
		return *this;
	}

	void AddDependent(const Dependent& dependent) 
	{
		dependents_.push_back(dependent);
	}

	template <typename Writer>
	void Serialize(Writer& writer) const
	{
		writer.StartObject();
		Person::Serialize(writer);
		writer.String("married");
		writer.Bool(married_);
		writer.String(("dependents"));
		writer.StartArray();
		for (std::vector<Dependent>::const_iterator dependentItr = dependents_.begin(); dependentItr != dependents_.end(); ++dependentItr)
			dependentItr->Serialize(writer);
		writer.EndArray();

		writer.EndObject();
	}

};

class AnotherPerson
{
private:
	string name;
	int id; 
public:
	AnotherPerson(string _name, int _id)
		:name(_name), id(_id) {}
	AnotherPerson() {}

	template <typename Writer>
	void Serialize(Writer& writer) const
	{
		writer.StartObject();
		writer.String("name");
		writer.String(name.c_str(), static_cast<SizeType>(name.length()));
		writer.String(("id"));
		writer.Uint(id);
		writer.EndObject();
	}
	string serialize() 
	{
		StringBuffer s;
		Writer<StringBuffer> writer(s);
		Serialize(writer);
		return  s.GetString();
	}
};


void main()
{
	vector<AnotherPerson> v;
	v.push_back(AnotherPerson("kim", 25));
	v.push_back(AnotherPerson("lee", 55));
	v.push_back(AnotherPerson("goo", 75));


	vector<AnotherPerson>::iterator iter = v.begin();
	for (; iter != v.end(); ++iter)
	{
		cout << iter->serialize() << endl;
	}

}

//int main(int, char*[]) {
//	std::vector<Employee> employees;
//
//	employees.push_back(Employee("Milo YIP", 34, true));
//	employees.back().AddDependent(Dependent("Lua YIP", 3, new Education("Happy Kindergarten", 3.5)));
//	employees.back().AddDependent(Dependent("Mio YIP", 1));
//
//	employees.push_back(Employee("Percy TSE", 30, false));
//
//	StringBuffer sb;
//	PrettyWriter<StringBuffer> writer(sb);
//
//	writer.StartArray();
//	for (std::vector<Employee>::const_iterator employeeItr = employees.begin(); employeeItr != employees.end(); ++employeeItr)
//		employeeItr->Serialize(writer);
//	writer.EndArray();
//
//	puts(sb.GetString());
//
//	return 0;
//}