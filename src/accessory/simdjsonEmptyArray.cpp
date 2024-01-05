#include "pch.h"
#include "simdjsonEmptyArray.h"

simdjson_result<fallback::ondemand::array> gl_simdjsonEmptyArray;

ondemand::parser s_parserEmptyArray;
auto s_jsonEmptyArray = "[]"_padded; // The _padded suffix creates a simdjson::padded_string instance
ondemand::document s_docEmptyArray;

void CreateSimdjsonEmptyArray() {
	s_docEmptyArray = s_parserEmptyArray.iterate(s_jsonEmptyArray);
	gl_simdjsonEmptyArray = s_docEmptyArray.get_array();
	int i = 0;
	i++;
}
