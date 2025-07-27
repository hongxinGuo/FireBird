#pragma once

long long StrToDecimal(const string_view& svData, int power);
string_view GetNextField(const string_view& svData, size_t& lCurrentPos, char delimiter);

void ReportJSonErrorToSystemMessage(const string& strPrefix, const string& strWhat);
void ReportJSonErrorToSystemMessage(const string& strPrefix, const string& strWhat, const char* jsonData);

void ParseSinaRTData(const CWebDataPtr& pWebData);
void ParseTengxunRTData(const CWebDataPtr& pWebData);
CDayLineWebDataPtr ParseNeteaseDayLine(const CWebDataPtr& pWebData);
CDayLineWebDataPtr ParseTengxunDayLine(const CWebDataPtr& pWebData);
shared_ptr<vector<CDayLinePtr>> ParseTengxunDayLine(const string_view& svData, const string& strStockCode);
bool IsTengxunRTDataInvalid(const CWebDataPtr& pWebDataReceived);

//bool CreateJsonWithNlohmann(json& js, string& str, long lBeginPos = 0, long lEndPos = 0);
bool CreateJsonWithNlohmann(json& js, const std::string& s, long lBeginPos = 0, long lEndPos = 0);
void ParseOneNeteaseRTData(const json::iterator& it, const CWebRTDataPtr& pWebRTData);
shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTData(json* pjs);
shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTDataWithSimdjson(string_view svJsonData);
shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTDataWithSimdjson(const CWebDataPtr& pData); // ½âÎöÒ»¸öWebData
