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
    TRACE(_T("ConvertBufferToTime�쳣\n"));
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
    TRACE(_T("ConvertBufferToTime�쳣\n"));
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
  CTime ct(lYear, lMonth, lD, lHour, lMinute, lSecond);	// ����ʱ��15ʱ��UTC7ʱ
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
// �������ݼ���GetDefaultConnect()�����Ե��ô˺�����ɾ��幤�����Ա�֤һ���ԡ�
//
// ����ģʽʱʹ��mysqlTest����������ģʽʱʹ��mysql�������Ա�֤ʹ�ò�ͬ��Schema��
//
/////////////////////////////////////////////////////////////////////////////////
CString GetDefaultSchemaConnect() {
  if (!gl_fNormalMode) {
    gl_fTestMode = true;
    return _T("DSN=mysqlTest;UID=Test;PASSWORD=test;charset=utf8"); // mysqlTest��������TestStock Schema
  }
  else {
    gl_fTestMode = false;
    return _T("DSN=mysql;UID=hxguo;PASSWORD=hxguo;charset=utf8"); // mysql��������SmartStockData Schema
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
    TRACE(_T("ConvertValuetoString�쳣\n"));
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
    TRACE(_T("ConvertValuetoString�쳣\n"));
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
    TRACE(_T("ConvertValuetoString�쳣\n"));
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
    TRACE(_T("ConvertValuetoString�쳣\n"));
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

  while (*pCurrentPos != ',') { // ����һ������ǰ���ַ����뻺����. 0x2c���Ƕ��š�
    if ((*pCurrentPos == 0x0d) || (*pCurrentPos == 0x00a) || (*pCurrentPos == 0x000) || (i > 100)) { // �����س������л����ַ���������,���߳�����100���ַ�
      return false; // ���ݳ�����������
    }
    buffer[i++] = *pCurrentPos++;
  }
  buffer[i] = 0x000;
  pCurrentPos++;
  i++;
  iReadNumber += i;
  return true;
}