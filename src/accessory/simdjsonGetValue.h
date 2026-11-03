#pragma once

#include"simdjson.h"

using std::string_view;

inline simdjson::ondemand::array gl_simdjsonEmptyArray;

void CreateSimdjsonEmptyArray();

[[nodiscard]] double simdjsonGetDouble(simdjson::ondemand::value& value, const string_view& key, double defaultValue = 0.0);
[[nodiscard]] double simdjsonGetDouble(simdjson::ondemand::object& object, const string_view& key, double defaultValue = 0.0);
[[nodiscard]] double simdjsonGetDouble(simdjson::ondemand::document& document, const string_view& key, double defaultValue = 0.0);
[[nodiscard]] double simdjsonGetDouble(simdjson::ondemand::value& value, double defaultValue = 0.0);

[[nodiscard]] int64_t simdjsonGetInt64(simdjson::ondemand::value& value, const string_view& key, int64_t defaultValue = 0);
[[nodiscard]] int64_t simdjsonGetInt64(simdjson::ondemand::object& object, const string_view& key, int64_t defaultValue = 0);
[[nodiscard]] int64_t simdjsonGetInt64(simdjson::ondemand::document& document, const string_view& key, int64_t defaultValue = 0);
[[nodiscard]] int64_t simdjsonGetInt64(simdjson::ondemand::value& value, int64_t defaultValue = 0);

[[nodiscard]] bool simdjsonGetBool(simdjson::ondemand::value& value, const string_view& key, bool defaultValue = false);
[[nodiscard]] bool simdjsonGetBool(simdjson::ondemand::object& object, const string_view& key, bool defaultValue = false);
[[nodiscard]] bool simdjsonGetBool(simdjson::ondemand::document& document, const string_view& key, bool defaultValue = false);
[[nodiscard]] bool simdjsonGetBool(simdjson::ondemand::value& value);

[[nodiscard]] string_view simdjsonGetStringView(simdjson::ondemand::value& value, const string_view& key, string_view defaultValue = "");
[[nodiscard]] string_view simdjsonGetStringView(simdjson::ondemand::object& object, const string_view& key, string_view defaultValue = "");
[[nodiscard]] string_view simdjsonGetStringView(simdjson::ondemand::document& document, const string_view& key, string_view defaultValue = "");
[[nodiscard]] string_view simdjsonGetStringView(simdjson::ondemand::value& value);

[[nodiscard]] simdjson::ondemand::array simdjsonGetArray(simdjson::ondemand::value& value, const string_view& key, const simdjson::ondemand::array& defaultValue = gl_simdjsonEmptyArray);
[[nodiscard]] simdjson::ondemand::array simdjsonGetArray(simdjson::ondemand::object& object, const string_view& key, const simdjson::ondemand::array& defaultValue = gl_simdjsonEmptyArray);
[[nodiscard]] simdjson::ondemand::array simdjsonGetArray(simdjson::ondemand::document& document, const string_view& key, const simdjson::ondemand::array& defaultValue = gl_simdjsonEmptyArray);
[[nodiscard]] simdjson::ondemand::array simdjsonGetArray(simdjson::ondemand::value& value);

[[nodiscard]] string_view simdjsonGetRawJsonToken(simdjson::ondemand::value& value, const string_view& key);
[[nodiscard]] string_view simdjsonGetRawJsonToken(simdjson::ondemand::object& object, const string_view& key);
[[nodiscard]] string_view simdjsonGetRawJsonToken(simdjson::ondemand::document& doc, const string_view& key);
[[nodiscard]] string_view simdjsonGetRawJsonToken(simdjson::ondemand::value& value);
