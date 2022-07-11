#pragma once

using namespace std;
#include<vector>
#include<string>

// 股票代码采用国家标准制式
bool IsShanghaiExchange(CString strStockCode);
bool IsShenzhenExchange(CString strStockCode);
CString GetStockSymbol(CString strStockCode);
CString GetStockExchange(CString strStockCode);
CString CreateStockCode(CString strStockExchange, CString strStockSymbol);
// 股票代码转换
CString XferSinaToStandred(CString strSina);
CString XferSinaToNetease(CString strSina);
CString XferNeteaseToStandred(CString strNetease);
CString XferNeteaseToSina(CString strNetease);
CString XferTengxunToStandred(CString strTengxun);
CString XferStandredToSina(CString strStandred);
CString XferStandredToNetease(CString strStandred);
CString XferStandredToTengxun(CString strStandred);

CString GetSchemaConnect(CString strSchema);// 用于数据集的GetDefaultConnect()，以保持一致性。参数strSchema为该市场的名称

CString ConvertValueToString(long lValue, int iDividend = 1);
CString ConvertValueToString(INT64 iValue, int iDividend = 1);
CString ConvertValueToString(int ivalue, int iDividend = 1);
CString ConvertValueToString(double dvalue, int iDividend = 1);

// 时间变换。将buffer中的字符串根据strFormat的制式变换成time_t制式的日期时间，采用UTC（GMT）标准时间
CString ConvertDateToString(long lDate);
time_t ConvertBufferToTime(CString strFormat, const char* bufferMarketTime, time_t tTimeZoneOffset = -8 * 3600); // 默认采用东八区标准时间
time_t ConvertStringToTime(CString strFormat, CString strMarketTime, time_t tTimeZoneOffset = -8 * 3600); // 默认采用东八区标准时间

// 虽然这个函数与读取新浪实时数据的完全一样，但为了防止以后可能改变的缘故，还是分别实现。
bool ReadOneValueOfNeteaseDayLine(string& pBuffer, char* buffer, INT64& lCurrentPos);

void ZoomIn(vector<double>& vData, double dLevel, double dRate);

CString FormatToMK(long long iNumber); // 将数字转换成以M或者K为单位

void ReportErrorToSystemMessage(CString strPrefix, exception& e);

string to_byte_string(const wstring& input); // 将utf-16制式的宽字符串转换成多字节的utf-8字符串。

//bool ParseWithNlohmannJSon(json* pjs, std::string& s);
