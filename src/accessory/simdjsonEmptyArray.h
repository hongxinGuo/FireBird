#pragma once

#include"simdjson.h"

using namespace simdjson;

extern simdjson_result<fallback::ondemand::array> gl_simdjsonEmptyArray;

void CreateSimdjsonEmptyArray();
