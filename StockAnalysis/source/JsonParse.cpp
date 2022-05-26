#include"pch.h"
#include"globedef.h"
#include"SystemMessage.h"

#include "JsonParse.h"

using namespace std;
#include<string>

#include<boost/property_tree/ptree.hpp>
#include<boost/property_tree/json_parser.hpp>
using namespace boost::property_tree;

#include"nlohmann/json.hpp"
using json = nlohmann::ordered_json;

bool ParseWithNlohmannJSon(json* pjs, std::string& s, long lBeginPos, long lEndPos) {
	try {
		*pjs = json::parse(s.begin() + lBeginPos, s.end() - lEndPos);
	}
	catch (json::parse_error& e) {
		char buffer[180]{}, buffer2[100];
		int i = 0;
		CString str = e.what();
		gl_systemMessage.PushErrorMessage(_T("Nlohmann JSon Reading Error ") + str);
		for (i = 0; i < 180; i++) buffer[i] = 0x000;
		for (i = 0; i < 180; i++) {
			if ((e.byte - 90 + i) < s.size()) {
				buffer[i] = s.at(e.byte - 90 + i);
			}
			else break;
		}
		sprintf_s(buffer2, _T("%d  "), (long)(s.size()));
		str = buffer2;
		sprintf_s(buffer2, _T("%d  "), (long)(e.byte));
		str += buffer2;
		sprintf_s(buffer2, _T("%d  "), i);
		str += buffer2;
		str += buffer;
		gl_systemMessage.PushErrorMessage(str);
		pjs = nullptr;
		return false;
	}
	return true;
}