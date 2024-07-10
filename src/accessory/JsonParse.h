#pragma once

long StrToDecimal(const string_view& svData, int power);
string_view GetNextField(const string_view& svData, long& lCurrentPos, char delimiter);

void ReportJSonErrorToSystemMessage(const CString& strPrefix, const CString& strWhat);
void ReportJSonErrorToSystemMessage(const CString& strPrefix, const CString& strWhat, const char* jsonData);

void ParseSinaRTData(const CWebDataPtr& pWebData);
void ParseTengxunRTData(const CWebDataPtr& pWebData);
CDayLineWebDataPtr ParseNeteaseDayLine(const CWebDataPtr& pWebData);
CDayLineWebDataPtr ParseTengxunDayLine(const CWebDataPtr& pWebData);
shared_ptr<vector<CDayLinePtr>> ParseTengxunDayLine(const string_view& svData, const CString& strStockCode);
bool IsTengxunRTDataInvalid(const CWebDataPtr& pWebDataReceived);

bool CreateJsonWithNlohmann(json& js, const std::string& s, long lBeginPos = 0, long lEndPos = 0);
bool CreateJsonWithNlohmann(json& js, CString& str, long lBeginPos = 0, long lEndPos = 0);
void ParseOneNeteaseRTData(const json::iterator& it, const CWebRTDataPtr& pWebRTData);
shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTData(json* pjs);
shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTDataWithSimdjson(string_view svJsonData);
shared_ptr<vector<CWebRTDataPtr>> ParseNeteaseRTDataWithSimdjson(const CWebDataPtr& pData); // ½âÎöÒ»¸öWebData
