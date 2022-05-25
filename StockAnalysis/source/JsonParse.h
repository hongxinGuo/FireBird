#pragma once

using namespace std;
#include<string>

#include<boost/property_tree/ptree.hpp>
#include<boost/property_tree/json_parser.hpp>
using namespace boost::property_tree;

#include"nlohmann/json.hpp"
using json = nlohmann::ordered_json;

bool ParseWithNlohmannJSon(json* pjs, std::string& s, long lBeginPos = 0, long lEndPos = 0);
