#include "accessory.h"

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
  catch (exception & e) {
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
  catch (exception & e) {
    TRACE(_T("ConvertBufferToTime异常\n"));
    return 0;
  }
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
    return _T("DSN=mysql;UID=guo;PASSWORD=guo1426hx;charset=utf8"); // mysql操作的是SmartStockData Schema
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
  catch (exception & e) {
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
  catch (exception & e) {
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
  catch (exception & e) {
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
  catch (exception & e) {
    TRACE(_T("ConvertValuetoString异常\n"));
    return _T("");
  }
}

double GetValue(char* buffer) {
  try {
    return atof(buffer);
  }
  catch (exception & e) {
    TRACE(_T("CDayline::SetValue exception\n"));
    return(0.0);
  }
}

double GetValue(CString strBuffer) {
  try {
    return atof(strBuffer);
  }
  catch (exception & e) {
    TRACE(_T("CDayline::SetValue exception\n"));
    return(0.0);
  }
}

bool ReadOneValueOfNeteaseDayLine(char*& pCurrentPos, char* buffer, long& iReadNumber) {
  int i = 0;

  while (*pCurrentPos != 0x2c) { // 将下一个逗号前的字符存入缓冲区. 0x2c就是逗号。
    if ((*pCurrentPos == 0x0d) || (*pCurrentPos == 0x00a) || (*pCurrentPos == 0x000)) {
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

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 读入浮点数，小数点后保留三位，不足就加上0.，多于三位就抛弃。读入的数值放大一千倍。
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ReadOneValueExceptPeriodOfNeteaseDayLine(char*& pCurrentPos, char* buffer, long& lCounter) {
  int i = 0;
  bool fFoundPoint = false;
  int iCount = 0;
  while ((*pCurrentPos != ',') && (iCount < 3)) {
    if (fFoundPoint) iCount++;
    if ((*pCurrentPos == 0x00a) || (*pCurrentPos == 0x00d) || (*pCurrentPos == 0x000)) return false;
    if (*pCurrentPos == '.') {
      fFoundPoint = true;
      pCurrentPos++;
    }
    else buffer[i++] = *pCurrentPos++;
  }

  if (fFoundPoint && (iCount < 3)) {
    int jCount = i;
    for (int j = iCount; j < 3; j++) {
      buffer[jCount++] = '0';
    }
    buffer[jCount] = 0x000;
  }
  else {
    buffer[i] = 0x000;
  }

  while (*pCurrentPos != ',') {
    if ((*pCurrentPos == 0x00a) || (*pCurrentPos == 0x00d) || (*pCurrentPos == 0x000)) return false;
    i++;
    pCurrentPos++;
  }
  pCurrentPos++;
  i++;
  if (fFoundPoint) i++;
  lCounter += i; // 多加1，是需要加上少算的逗号

  return true;
}