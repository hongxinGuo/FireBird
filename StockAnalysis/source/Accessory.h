#pragma once

#include"stdafx.h"

#include"WebData.h"

using namespace std;
#include<vector>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
using namespace boost::property_tree;

// 时间变换。将buffer中的字符串根据strFormat的制式变换成time_t制式的日期时间
time_t ConvertBufferToTime(CString strFormat, const char* buffer);
time_t ConvertStringToTime(CString strFormat, CString strTime);

// 时间转换辅助函数
time_t FormatToTTime(long lDate, long lTime = 150000); // 将整型(YYYYMMDD)转变为time_t形式， 默认时间为15:00:00
long FormatToDate(time_t tt) noexcept;// 将时间转变为整型(YYYYMMDD)形式
long FormatToTime(time_t tt) noexcept; // 将时间转变为整数（HHMMSS)形式
INT64 FormatToDateTime(time_t tt) noexcept; // 将时间转变为整数（YYYYMMDDHHMMSS)形式
long FormatToDate(const tm* ptm) noexcept; // 将时间转变为整型(YYYYMMDD)形式
long FormatToTime(const tm* ptm) noexcept; // 将时间转变为整型(YYYYMMDD)形式
INT64 FormatToDateTime(const tm* ptm) noexcept; // 将时间转变为整型(YYYYMMDD)形式
bool IsEarlyThen(long lEarlyDate, long lLatelyDate, long lTimeSpawnOfDays);

long GetNextMonday(long lDate); // 找到lDate的下一个星期一的数值
long GetPrevMonday(long lDate); // 找到lDate的上一个星期一的数值
long GetCurrentMonday(long lDate); // 找到包含lDate的那个星期的星期一

CString GetChinaMarketSchemaVonnect();// 用于中国市场数据集的GetDefaultConnect()，以保持一致性。测试模式时使用mysqlTest驱动，正常模式时使用mysql驱动
CString GetAmericaMarketSchemaConnect();// 用于中国市场数据集的GetDefaultConnect()，以保持一致性。测试模式时使用AmericaMarketTest驱动，正常模式时使用AmericaMarket驱动

CString ConvertValueToString(long lValue, int iDividend = 1);
CString ConvertValueToString(INT64 iValue, int iDividend = 1);
CString ConvertValueToString(int ivalue, int iDividend = 1);
CString ConvertValueToString(double dvalue, int iDividend = 1);

// 虽然这个函数与读取新浪实时数据的完全一样，但为了防止以后可能改变的缘故，还是分别实现。
bool ReadOneValueOfNeteaseDayLine(char*& pCurrentPos, char* buffer, long& iReadNumber);
bool ReadOneValueOfNeteaseDayLine2(vector<char>& pBuffer, char* buffer, INT64& lCurrentPos);

void ZoomIn(vector<double>& vData, double dLevel, double dRate);

class CRSReference {
public:
  bool m_fActive{ false };
  long m_lDayLength[4]{ 0, 0, 0, 0 }; // 日期长度
  long m_lStrongDayLength[4]{ 0, 0, 0, 0 }; // 满足条件的天数
  double m_dRSStrong[4]{ 50.0, 50.0, 50.0, 50.0 }; // 正值意思为高于此值，负值意思为低于此值
};

bool ConvertToJSon(ptree& pt, CWebDataPtr pWebData);
