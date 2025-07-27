#pragma once

extern ondemand::array gl_simdjsonEmptyArray;

template <typename T, typename Getter>
T jsonGetHelper(Getter getter, ondemand::value& value, const string_view& key, const T& defaultValue) {
	auto valueInner = value[key].value();
	try {
		return getter(valueInner);
	} catch ([[maybe_unused]] simdjson_error& error) {
		if (valueInner.is_null()) return defaultValue;
		throw simdjson_error(error);
	}
}

void CreateSimdjsonEmptyArray();

[[nodiscard]] double simdjsonGetDouble(ondemand::value& value, const string_view& key, double defaultValue = 0.0);
[[nodiscard]] double simdjsonGetDouble(ondemand::object& object, const string_view& key, double defaultValue = 0.0);
[[nodiscard]] double simdjsonGetDouble(ondemand::document& document, const string_view& key, double defaultValue = 0.0);
[[nodiscard]] double simdjsonGetDouble(ondemand::value& value, double defaultValue = 0.0);

[[nodiscard]] INT64 simdjsonGetInt64(ondemand::value& value, const string_view& key, INT64 defaultValue = 0);
[[nodiscard]] INT64 simdjsonGetInt64(ondemand::object& object, const string_view& key, INT64 defaultValue = 0);
[[nodiscard]] INT64 simdjsonGetInt64(ondemand::document& document, const string_view& key, INT64 defaultValue = 0);
[[nodiscard]] INT64 simdjsonGetInt64(ondemand::value& value, INT64 defaultValue = 0);

[[nodiscard]] bool simdjsonGetBool(ondemand::value& value, const string_view& key, bool defaultValue = false);
[[nodiscard]] bool simdjsonGetBool(ondemand::object& object, const string_view& key, bool defaultValue = false);
[[nodiscard]] bool simdjsonGetBool(ondemand::document& document, const string_view& key, bool defaultValue = false);
[[nodiscard]] bool simdjsonGetBool(ondemand::value& value);

[[nodiscard]] string_view simdjsonGetStringView(ondemand::value& value, const string_view& key, string_view defaultValue = _T(""));
[[nodiscard]] string_view simdjsonGetStringView(ondemand::object& object, const string_view& key, string_view defaultValue = _T(""));
[[nodiscard]] string_view simdjsonGetStringView(ondemand::document& document, const string_view& key, string_view defaultValue = _T(""));
[[nodiscard]] string_view simdjsonGetStringView(ondemand::value& value);

[[nodiscard]] ondemand::array simdjsonGetArray(ondemand::value& value, const string_view& key, const ondemand::array& defaultValue = gl_simdjsonEmptyArray);
[[nodiscard]] ondemand::array simdjsonGetArray(ondemand::object& object, const string_view& key, const ondemand::array& defaultValue = gl_simdjsonEmptyArray);
[[nodiscard]] ondemand::array simdjsonGetArray(ondemand::document& document, const string_view& key, const ondemand::array& defaultValue = gl_simdjsonEmptyArray);
[[nodiscard]] ondemand::array simdjsonGetArray(ondemand::value& value);

[[nodiscard]] string_view simdjsonGetRawJsonToken(ondemand::value& value, const string_view& key);
[[nodiscard]] string_view simdjsonGetRawJsonToken(ondemand::object& object, const string_view& key);
[[nodiscard]] string_view simdjsonGetRawJsonToken(ondemand::document& doc, const string_view& key);
[[nodiscard]] string_view simdjsonGetRawJsonToken(ondemand::value& value);
