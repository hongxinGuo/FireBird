#include"globedef.h"
#include "Accessory.h"

time_t ConvertBufferToTime(CString strFormat, char* buffer) {
  time_t tt;
  tm tm_;
  int year, month, day, hour, minute, second;
  try {
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
  catch (exception&) {
    TRACE(_T("ConvertBufferToTime异常\n"));
    return 0;
  }
}

time_t ConvertStringToTime(CString strFormat, CString strTime) {
  time_t tt;
  tm tm_;
  int year, month, day, hour, minute, second;
  try {
    sscanf_s(strTime.GetBuffer(), strFormat.GetBuffer(), &year, &month, &day, &hour, &minute, &second);
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
  catch (exception&) {
    TRACE(_T("ConvertBufferToTime异常\n"));
    return 0;
  }
}

time_t FormatToTTime(long lDay, long lTime) {
  ASSERT(lDay > 19700000);
  const long lYear = lDay / 10000;
  const long lMonth = (lDay - lYear * 10000) / 100;
  const long lD = (lDay - lYear * 10000 - lMonth * 100);
  const long lHour = lTime / 10000;
  const long lMinute = (lTime - lHour * 10000) / 100;
  const long lSecond = lTime - lHour * 10000 - lMinute * 100;
  CTime ct(lYear, lMonth, lD, lHour, lMinute, lSecond);	// 北京时间15时即UTC7时
  return (ct.GetTime());
}

long FormatToDay(time_t const tt) {
  tm tm_;
  localtime_s(&tm_, &tt);
  return((tm_.tm_year + 1900) * 10000 + (tm_.tm_mon + 1) * 100 + tm_.tm_mday);
}

long FormatToDay(tm* ptm) {
  return((ptm->tm_year + 1900) * 10000 + (ptm->tm_mon + 1) * 100 + ptm->tm_mday);
}

////////////////////////////////////////////////////////////////////////////////
//
// 所有数据集的GetDefaultConnect()函数皆调用此函数完成具体工作，以保证一致性。
//
// 测试模式时使用mysqlTest驱动，正常模式时使用mysql驱动，以保证使用不同的Schema。
//
/////////////////////////////////////////////////////////////////////////////////
CString GetDefaultSchemaConnect() {
  if (!gl_fNormalMode) {
    gl_fTestMode = true;
    return _T("DSN=mysqlTest;UID=Test;PASSWORD=test;charset=utf8"); // mysqlTest操作的是TestStock Schema
  }
  else {
    gl_fTestMode = false;
    return _T("DSN=mysql;UID=hxguo;PASSWORD=hxguo;charset=utf8"); // mysql操作的是SmartStockData Schema
  }
}

CString ConvertValueToString(long lValue, int iDividend) {
  char buffer[50];
  double d = ((double)lValue) / iDividend;
  CString str;

  try {
    sprintf_s(buffer, "%.3f", d);
    str = buffer;
    return str;
  }
  catch (exception&) {
    TRACE(_T("ConvertValuetoString异常\n"));
    return _T("");
  }
}

CString ConvertValueToString(int iValue, int iDividend) {
  char buffer[50];
  double d = ((double)iValue) / iDividend;
  CString str;

  try {
    sprintf_s(buffer, "%.3f", d);
    str = buffer;

    return str;
  }
  catch (exception&) {
    TRACE(_T("ConvertValuetoString异常\n"));
    return _T("");
  }
}

CString ConvertValueToString(INT64 iValue, int iDividend) {
  char buffer[50];
  double d = ((double)iValue) / iDividend;
  CString str;

  try {
    sprintf_s(buffer, "%.3f", d);
    str = buffer;
    return str;
  }
  catch (exception&) {
    TRACE(_T("ConvertValuetoString异常\n"));
    return _T("");
  }
}

CString ConvertValueToString(double dValue, int iDividend) {
  char buffer[50];
  double d = dValue / iDividend;
  CString str;

  try {
    sprintf_s(buffer, "%.3f", d);
    str = buffer;
    return str;
  }
  catch (exception&) {
    TRACE(_T("ConvertValuetoString异常\n"));
    return _T("");
  }
}

double GetValue(char* buffer) {
  try {
    return atof(buffer);
  }
  catch (exception&) {
    TRACE(_T("CDayline::SetValue exception\n"));
    return(0.0);
  }
}

double GetValue(CString strBuffer) {
  try {
    return atof(strBuffer);
  }
  catch (exception&) {
    TRACE(_T("CDayline::SetValue exception\n"));
    return(0.0);
  }
}

bool ReadOneValueOfNeteaseDayLine(char*& pCurrentPos, char* buffer, long& iReadNumber) {
  int i = 0;

  while (*pCurrentPos != ',') { // 将下一个逗号前的字符存入缓冲区. 0x2c就是逗号。
    if ((*pCurrentPos == 0x0d) || (*pCurrentPos == 0x00a) || (*pCurrentPos == 0x000) || (i > 100)) { // 遇到回车、换行或者字符串结束符,或者超过了100个字符
      return false; // 数据出错，放弃载入
    }
    buffer[i++] = *pCurrentPos++;
  }
  buffer[i] = 0x000;
  pCurrentPos++;
  i++;
  iReadNumber += i;
  return true;
}