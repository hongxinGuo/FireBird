#include "accessory.h"

time_t ConvertBufferToTime(CString strFormat, char* buffer) {
  time_t tt;
  tm tm_;
  int year, month, day, hour, minute, second;
  sscanf_s(buffer, strFormat.GetBuffer(), &year, &month, &day, &hour, &minute, &second);
  tm_.tm_year = year - 1900;
  tm_.tm_mon = month - 1;
  tm_.tm_mday = day;
  tm_.tm_hour = hour;
  tm_.tm_min = minute;
  tm_.tm_sec = second;
  tm_.tm_isdst = 0;
  tt = mktime(&tm_);

  return tt;
}

////////////////////////////////////////////////////////////////////////////////
//
// 所有数据集的GetDefaultConnect()函数皆调用此函数完成具体工作，以保证一致性。
//
// 测试模式时使用mysqlTest驱动，正常模式时使用mysql驱动，以保证使用不同的Schema。
//
/////////////////////////////////////////////////////////////////////////////////
CString GetDefaultSchemaConnect()
{
  if (!gl_fNormalMode) {
    gl_fTestMode = true;
    return _T("DSN=mysqlTest;UID=Test;PASSWORD=test;charset=utf8"); // mysqlTest操作的是TestStock Schema
  }
  else {
    gl_fTestMode = false;
    return _T("DSN=mysql;UID=guo;PASSWORD=guo1426hx;charset=utf8"); // mysql操作的是SmartStockData Schema
  }
}

CString ConvertValueToString(long lValue, int iDividend)
{
  char buffer[30];
  double d = ((double)lValue) / iDividend;
  CString str;

  sprintf_s(buffer, "%.3f", d);
  str = buffer;

  return str;
}

CString ConvertValueToString(int iValue, int iDividend)
{
  char buffer[30];
  double d = ((double)iValue) / iDividend;
  CString str;

  sprintf_s(buffer, "%.3f", d);
  str = buffer;

  return str;
}

CString ConvertValueToString(INT64 iValue, int iDividend)
{
  char buffer[30];
  double d = ((double)iValue) / iDividend;
  CString str;

  sprintf_s(buffer, "%.3f", d);
  str = buffer;

  return str;
}

CString ConvertValueToString(double dValue, int iDividend)
{
  char buffer[30];
  double d = dValue / iDividend;
  CString str;

  sprintf_s(buffer, "%.3f", d);
  str = buffer;

  return str;
}