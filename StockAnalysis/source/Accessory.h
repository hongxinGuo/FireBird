#pragma once

#include"stdafx.h"

#include"globedef.h"

// 时间变换。将buffer中的字符串根据strFormat的制式变换成time_t制式的日期时间
time_t ConvertBufferToTime(CString strFormat, char* buffer);
time_t ConvertStringToTime(CString strFormat, CString strTime);

// 时间转换辅助函数
time_t FormatToTTime(long lDay, long lTime = 150000); // 将整型(YYYYMMDD)转变为time_t形式
long FormatToDay(time_t tt);// 将时间转变为整型(YYYYMMDD)形式
long FormatToDay(tm* ptm); // 将时间转变为整型(YYYYMMDD)形式

// 用于个数据集的GetDefaultConnect()，以保持一致性。测试模式时使用mysqlTest驱动，正常模式时使用mysql驱动。
CString GetDefaultSchemaConnect();

CString ConvertValueToString(long lValue, int iDividend = 1);
CString ConvertValueToString(INT64 iValue, int iDividend = 1);
CString ConvertValueToString(int ivalue, int iDividend = 1);
CString ConvertValueToString(double dvalue, int iDividend = 1);

double GetValue(char* buffer);
double GetValue(CString strBuffer);

// 虽然这个函数与读取新浪实时数据的完全一样，但为了防止以后可能改变的缘故，还是分别实现。
bool ReadOneValueOfNeteaseDayLine(vector<char>& buffer, char* buffer2, INT64& lCurrentPos);
