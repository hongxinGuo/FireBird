#pragma once

#include<time.h>

#include"WebData.h"

using namespace std;
#include<vector>

#include<boost/property_tree/ptree.hpp>
#include<boost/property_tree/json_parser.hpp>
using namespace boost::property_tree;

// 时间变换。将buffer中的字符串根据strFormat的制式变换成time_t制式的日期时间，采用UTC（GMT）标准时间
CString ConvertDateToString(long lDate);
time_t ConvertBufferToTime(CString strFormat, const char* bufferMarketTime, time_t tTimeZoneOffset = -8 * 3600); // 默认采用东八区标准时间
time_t ConvertStringToTime(CString strFormat, CString strMarketTime, time_t tTimeZoneOffset = -8 * 3600); // 默认采用东八区标准时间

// 时间转换辅助函数。time_t使用UTC（GMT）标准，其他形式的时间为该市场的标准时间，默认采用东八区标准时间。
time_t TransferToTTime(long lDate, time_t tTimeZone, long lTime); // 将整型(YYYYMMDD)转变为time_t形式.
long TransferToDate(time_t tUTC, time_t tTimeZone = -8 * 3600) noexcept;// 将时间转变为整型(YYYYMMDD)形式, 默认东八区标准时间
long TransferToTime(time_t tUTC, time_t tTimeZone = -8 * 3600) noexcept; // 将时间转变为整数（HHMMSS)形式, 默认东八区标准时间
INT64 TransferToDateTime(time_t tUTC, time_t tTimeZone = -8 * 3600) noexcept; // 将时间转变为整数（YYYYMMDDHHMMSS)形式, 默认东八区标准时间
long TransferToDate(const tm* ptm) noexcept; // 将时间转变为整型(YYYYMMDD)形式
long TransferToTime(const tm* ptm) noexcept; // 将时间转变为整型(HHMMSS)形式
INT64 TransferToDateTime(const tm* ptm) noexcept; // 将时间转变为整型(YYYYMMDDHHMMSS)形式
bool IsEarlyThen(long lEarlyDate, long lLatelyDate, long lTimeSpawnOfDays);

long GetNextMonday(long lDate); // 找到lDate的下一个星期一的数值
long GetPrevMonday(long lDate); // 找到lDate的上一个星期一的数值
long GetCurrentMonday(long lDate); // 找到包含lDate的那个星期的星期一

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

// 虽然这个函数与读取新浪实时数据的完全一样，但为了防止以后可能改变的缘故，还是分别实现。
bool ReadOneValueOfNeteaseDayLine(vector<char>& pBuffer, char* buffer, INT64& lCurrentPos);

void ZoomIn(vector<double>& vData, double dLevel, double dRate);

class CRSReference {
public:
	bool m_fActive{ false };
	long m_lDayLength[4]{ 0, 0, 0, 0 }; // 日期长度
	long m_lStrongDayLength[4]{ 0, 0, 0, 0 }; // 满足条件的天数
	double m_dRSStrong[4]{ 50.0, 50.0, 50.0, 50.0 }; // 正值意思为高于此值，负值意思为低于此值
};

bool ConvertToJSON(ptree& pt, CWebDataPtr pWebData);
bool ConvertToJSON(ptree& pt, string s);
bool IsJsonReportingrror(ptree& pt, string& s); // 检测json是否为错误报告

CString FormatToMK(long long iNumber); // 将数字转换成以M或者K为单位

// 时间支持函数
void GetUTCTimeStruct(tm* tm_, const time_t* tUTC);
void GetMarketTimeStruct(tm* tm_, time_t tUTC, const time_t tTimeZone);

void ReportJSonErrorToSystemMessage(CString strPrefix, ptree_error& e);
void ReportErrorToSystemMessage(CString strPrefix, exception& e);
