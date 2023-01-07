#pragma once

#ifdef _DEBUG
//#define JSON_DIAGNOSTICS 1
#endif

#include<memory>
using std::shared_ptr;

#include"nlohmann/json.hpp"
using json = nlohmann::ordered_json;

using jsonPtr = shared_ptr<json>;
