#pragma once

using std::shared_ptr;
using std::string;
using std::string_view;

#include"nlohmann/json_fwd.hpp"
using nlohmannJson = nlohmann::ordered_json;

class CDayLine;
class CWebRTData;
class CDayLineWebData;

long long StrToDecimal(const string_view& svData, size_t power);
long long StrToDecimal2(const string_view& svData, size_t power);
long Str2Long(const std::string_view& svData, size_t power);
string_view GetNextField(const string_view& svData, size_t& lCurrentPos, char delimiter);

void ReportJSonErrorToSystemMessage(const string& strPrefix, const string& strWhat);
void ReportJSonErrorToSystemMessage(const string& strPrefix, const string& strWhat, const char* jsonData);

void ParseSinaRTData(const string& text);
void ParseTengxunRTData(const string& text);

shared_ptr<CWebRTData> ParseSinaOneData(const string_view& svData);
shared_ptr<CWebRTData> ParseOneTengxunData(const string_view& svData);

shared_ptr<CDayLineWebData> ParseTengxunDayLine(const string& text, const string& stockSymbol);
shared_ptr<std::vector<CDayLine>> ParseTengxunDayLineImp(const string& text, const string& strStockCode);
inline bool IsTengxunRTDataInvalid(const string& text) noexcept { return text == "v_pv_none_match=\"1\";\n"; }

//bool CreateJsonWithNlohmann(nlohmannJson& js, string& str, long lBeginPos = 0, long lEndPos = 0);
bool CreateJsonWithNlohmann(nlohmannJson& js, const std::string& s, size_t lBeginPos = 0, size_t lEndPos = 0);
bool CreateJsonWithNlohmann(nlohmannJson& js, const std::string_view s, size_t lBeginPos = 0, size_t lEndPos = 0);

inline bool IsVoidJson(const string_view sw) { return sw == "{}" || sw == "[]"; }
inline bool IsVoidJson(const string& s) { return s == "{}" || s == "[]"; }
