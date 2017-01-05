#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include <iostream>
#include <string>
#include <fstream>

using namespace rapidjson;
using namespace std;
int main()
{
	// C++ I/O - 
	string json = "{\"project\":\"rapidjson\",\"stars\":10}";

	// 1. Parse a JSON string into DOM.
	Document d;
	d.Parse(json.c_str());

	// 3. Stringify the DOM
	StringBuffer buffer; 
	Writer<StringBuffer> writer(buffer);
	d.Accept(writer);

	cout << buffer.GetString() << endl;
	return 0;
}