#include"pch.h"
module;
#include"nlohmann/json.hpp"
export module FireBird.Accessory.JsonGetValue;

export {
	[[nodiscard]]  nlohmann::ordered_json jsonGetChild( nlohmann::ordered_json* pjs, const char* szKey);
	[[nodiscard]] std::string jsonGetString( nlohmann::ordered_json* pjs, const char* szKey, const char* szDefault = _T(""));
	[[nodiscard]] double jsonGetDouble( nlohmann::ordered_json* pjs, const char* szKey, double dDefault = 0.0);
	[[nodiscard]] int jsonGetInt( nlohmann::ordered_json* pjs, const char* szKey, int iDefault = 0);
	[[nodiscard]] long jsonGetLong( nlohmann::ordered_json* pjs, const char* szKey, long lDefault = 0);
	[[nodiscard]] long long jsonGetLongLong( nlohmann::ordered_json* pjs, const char* szKey, long long llDefault = 0);
	[[nodiscard]]  nlohmann::ordered_json jsonGetChild( nlohmann::ordered_json& js, const char* szKey);
	[[nodiscard]] std::string jsonGetString( nlohmann::ordered_json& js, const char* szKey, const char* szDefault = _T(""));
	[[nodiscard]] double jsonGetDouble( nlohmann::ordered_json& js, const char* szKey, double dDefault = 0.0);
	[[nodiscard]] int jsonGetInt( nlohmann::ordered_json& js, const char* szKey, int iDefault = 0);
	[[nodiscard]] long jsonGetLong( nlohmann::ordered_json& js, const char* szKey, long lDefault = 0);
	[[nodiscard]] long long jsonGetLongLong( nlohmann::ordered_json& js, const char* szKey, long long llDefault = 0);

	[[nodiscard]]  nlohmann::ordered_json jsonGetChild(const  nlohmann::ordered_json::iterator& it, const char* szKey);
	[[nodiscard]] std::string jsonGetString(const  nlohmann::ordered_json::iterator& it, const char* szKey, const char* szDefault = _T(""));
	[[nodiscard]] double jsonGetDouble(const  nlohmann::ordered_json::iterator& it, const char* szKey, double dDefault = 0.0);
	[[nodiscard]] int jsonGetInt(const  nlohmann::ordered_json::iterator& it, const char* szKey, int iDefault = 0);
	[[nodiscard]] long jsonGetLong(const  nlohmann::ordered_json::iterator& it, const char* szKey, long lDefault = 0);
	[[nodiscard]] long long jsonGetLongLong(const  nlohmann::ordered_json::iterator& it, const char* szKey, long long llDefault = 0);
	[[nodiscard]] std::string jsonGetString(const  nlohmann::ordered_json::iterator& it);
	[[nodiscard]] double jsonGetDouble(const  nlohmann::ordered_json::iterator& it);
	[[nodiscard]] int jsonGetInt(const  nlohmann::ordered_json::iterator& it);
	[[nodiscard]] long jsonGetLong(const  nlohmann::ordered_json::iterator& it);
	[[nodiscard]] long long jsonGetLongLong(const  nlohmann::ordered_json::iterator& it);
}
