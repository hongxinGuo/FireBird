#pragma once

#include"stdafx.h"

#include"globedef.h"

// 时间变换。将buffer中的字符串根据strFormat的制式变换成time_t制式的日期时间
time_t ConvertBufferToTime(CString strFormat, char* buffer);
time_t ConvertStringToTime(CString strFormat, CString strTime);

// 用于个数据集的GetDefaultConnect()，以保持一致性。测试模式时使用mysqlTest驱动，正常模式时使用mysql驱动。
CString GetDefaultSchemaConnect();

CString ConvertValueToString(long lValue, int iDividend = 1);
CString ConvertValueToString(INT64 iValue, int iDividend = 1);
CString ConvertValueToString(int ivalue, int iDividend = 1);
CString ConvertValueToString(double dvalue, int iDividend = 1);

double GetValue(char* buffer);
double GetValue(CString strBuffer);

// 虽然这个函数与读取新浪实时数据的完全一样，但为了防止以后可能改变的缘故，还是分别实现。
bool ReadOneValueOfNeteaseDayLine(char*& pCurrentPos, char* buffer, long& iReadNumber);
bool ReadOneValueExceptPeriodOfNeteaseDayLine(char*& pCurrentPos, char* buffer, long& lCounter);
