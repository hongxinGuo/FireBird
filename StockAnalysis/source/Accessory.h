#pragma once

#include"stdafx.h"

using namespace std;
#include<vector>

// ʱ��任����buffer�е��ַ�������strFormat����ʽ�任��time_t��ʽ������ʱ��
time_t ConvertBufferToTime(CString strFormat, char* buffer);
time_t ConvertStringToTime(CString strFormat, CString strTime);

// ʱ��ת����������
time_t FormatToTTime(long lDay, long lTime = 150000); // ������(YYYYMMDD)ת��Ϊtime_t��ʽ
long FormatToDay(time_t tt);// ��ʱ��ת��Ϊ����(YYYYMMDD)��ʽ
long FormatToDay(tm* ptm); // ��ʱ��ת��Ϊ����(YYYYMMDD)��ʽ

// ���ڸ����ݼ���GetDefaultConnect()���Ա���һ���ԡ�����ģʽʱʹ��mysqlTest����������ģʽʱʹ��mysql������
CString GetDefaultSchemaConnect();

CString ConvertValueToString(long lValue, int iDividend = 1);
CString ConvertValueToString(INT64 iValue, int iDividend = 1);
CString ConvertValueToString(int ivalue, int iDividend = 1);
CString ConvertValueToString(double dvalue, int iDividend = 1);

// ��Ȼ����������ȡ����ʵʱ���ݵ���ȫһ������Ϊ�˷�ֹ�Ժ���ܸı��Ե�ʣ����Ƿֱ�ʵ�֡�
bool ReadOneValueOfNeteaseDayLine(char*& pCurrentPos, char* buffer, long& iReadNumber);

void ZoomIn(vector<double>& vData, double dLevel, double dRate);

class CRSReference {
public:
  CRSReference(void) {
    m_fActive = false;
    for (int i = 0; i < 3; i++) {
      m_lDayLength[i] = 0;
      m_dRSStrong[i] = 50.0;
    }
  }
public:
  bool m_fActive;
  long m_lDayLength[3]; // ���ڳ���
  double m_dRSStrong[3]; // ��ֵ��˼Ϊ���ڴ�ֵ����ֵ��˼Ϊ���ڴ�ֵ
};
