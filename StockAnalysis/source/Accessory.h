#pragma once

#include"stdafx.h"

#include"WebData.h"

using namespace std;
#include<vector>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
using namespace boost::property_tree;

// ʱ��任����buffer�е��ַ�������strFormat����ʽ�任��time_t��ʽ������ʱ��
time_t ConvertBufferToTime(CString strFormat, const char* buffer);
time_t ConvertStringToTime(CString strFormat, CString strTime);

// ʱ��ת����������
time_t FormatToTTime(long lDate, long lTime = 150000); // ������(YYYYMMDD)ת��Ϊtime_t��ʽ�� Ĭ��ʱ��Ϊ15:00:00
long FormatToDate(time_t tt) noexcept;// ��ʱ��ת��Ϊ����(YYYYMMDD)��ʽ
long FormatToTime(time_t tt) noexcept; // ��ʱ��ת��Ϊ������HHMMSS)��ʽ
INT64 FormatToDateTime(time_t tt) noexcept; // ��ʱ��ת��Ϊ������YYYYMMDDHHMMSS)��ʽ
long FormatToDate(const tm* ptm) noexcept; // ��ʱ��ת��Ϊ����(YYYYMMDD)��ʽ
long FormatToTime(const tm* ptm) noexcept; // ��ʱ��ת��Ϊ����(YYYYMMDD)��ʽ
INT64 FormatToDateTime(const tm* ptm) noexcept; // ��ʱ��ת��Ϊ����(YYYYMMDD)��ʽ
bool IsEarlyThen(long lEarlyDate, long lLatelyDate, long lTimeSpawnOfDays);

long GetNextMonday(long lDate); // �ҵ�lDate����һ������һ����ֵ
long GetPrevMonday(long lDate); // �ҵ�lDate����һ������һ����ֵ
long GetCurrentMonday(long lDate); // �ҵ�����lDate���Ǹ����ڵ�����һ

CString GetChinaMarketSchemaVonnect();// �����й��г����ݼ���GetDefaultConnect()���Ա���һ���ԡ�����ģʽʱʹ��mysqlTest����������ģʽʱʹ��mysql����
CString GetAmericaMarketSchemaConnect();// �����й��г����ݼ���GetDefaultConnect()���Ա���һ���ԡ�����ģʽʱʹ��AmericaMarketTest����������ģʽʱʹ��AmericaMarket����

CString ConvertValueToString(long lValue, int iDividend = 1);
CString ConvertValueToString(INT64 iValue, int iDividend = 1);
CString ConvertValueToString(int ivalue, int iDividend = 1);
CString ConvertValueToString(double dvalue, int iDividend = 1);

// ��Ȼ����������ȡ����ʵʱ���ݵ���ȫһ������Ϊ�˷�ֹ�Ժ���ܸı��Ե�ʣ����Ƿֱ�ʵ�֡�
bool ReadOneValueOfNeteaseDayLine(char*& pCurrentPos, char* buffer, long& iReadNumber);
bool ReadOneValueOfNeteaseDayLine2(vector<char>& pBuffer, char* buffer, INT64& lCurrentPos);

void ZoomIn(vector<double>& vData, double dLevel, double dRate);

class CRSReference {
public:
  bool m_fActive{ false };
  long m_lDayLength[4]{ 0, 0, 0, 0 }; // ���ڳ���
  long m_lStrongDayLength[4]{ 0, 0, 0, 0 }; // ��������������
  double m_dRSStrong[4]{ 50.0, 50.0, 50.0, 50.0 }; // ��ֵ��˼Ϊ���ڴ�ֵ����ֵ��˼Ϊ���ڴ�ֵ
};

bool ConvertToJSon(ptree& pt, CWebDataPtr pWebData);
