#pragma once

long long StrToDecimal(const string_view& svData, size_t power);
long long StrToDecimal2(const string_view& svData, size_t power);
long Str2Long(const std::string_view& svData, size_t power);
string_view GetNextField(const string_view& svData, size_t& lCurrentPos, char delimiter);

void ReportJSonErrorToSystemMessage(const string& strPrefix, const string& strWhat);
void ReportJSonErrorToSystemMessage(const string& strPrefix, const string& strWhat, const char* jsonData);

void ParseSinaRTData(const CWebDataPtr& pWebData);
void ParseTengxunRTData(const CWebDataPtr& pWebData);
CDayLineWebDataPtr ParseTengxunDayLine(const CWebDataPtr& pWebData);
CDayLinesPtr ParseTengxunDayLine(const string_view& svData, const string& strStockCode);
bool IsTengxunRTDataInvalid(const CWebDataPtr& pWebDataReceived);

//bool CreateJsonWithNlohmann(nlohmannJson& js, string& str, long lBeginPos = 0, long lEndPos = 0);
bool CreateJsonWithNlohmann(nlohmannJson& js, const std::string& s, long lBeginPos = 0, long lEndPos = 0);
