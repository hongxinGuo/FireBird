#pragma once

#ifdef _DEBUG
//#define JSON_DIAGNOSTICS 1
#endif

#include"nlohmann/json.hpp"
using json = nlohmann::ordered_json;
