#pragma once

#include"WebData.h"

using namespace std;
#include<vector>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
using namespace boost::property_tree;

long GetFormatedDate(void);

// ʱ��任����buffer�е��ַ�������strFormat����ʽ�任��time_t��ʽ������ʱ��
time_t ConvertBufferToTime(CString strFormat, const char* buffer);
time_t ConvertStringToTime(CString strFormat, CString strTime);

// ʱ��ת����������
time_t FormatToTTime(long lDate, long lTime = 150000); // ������(YYYYMMDD)ת��Ϊtime_t��ʽ�� Ĭ��ʱ��Ϊ15:00:00
long FormatToDate(time_t tt) noexcept;// ��ʱ��ת��Ϊ����(YYYYMMDD)��ʽ
long FormatToTime(time_t tt) noexcept; // ��ʱ��ת��Ϊ������HHMMSS)��ʽ
INT64 FormatToDateTime(time_t tt) noexcept; // ��ʱ��ת��Ϊ������YYYYMMDDHHMMSS)��ʽ
long FormatToDate(const tm* ptm) noexcept; // ��ʱ��ת��Ϊ����(YYYYMMDD)��ʽ
long FormatToTime(const tm* ptm) noexcept; // ��ʱ��ת��Ϊ����(HHMMSS)��ʽ
INT64 FormatToDateTime(const tm* ptm) noexcept; // ��ʱ��ת��Ϊ����(YYYYMMDD)��ʽ
bool IsEarlyThen(long lEarlyDate, long lLatelyDate, long lTimeSpawnOfDays);

long GetNextMonday(long lDate); // �ҵ�lDate����һ������һ����ֵ
long GetPrevMonday(long lDate); // �ҵ�lDate����һ������һ����ֵ
long GetCurrentMonday(long lDate); // �ҵ�����lDate���Ǹ����ڵ�����һ

// ��Ʊ������ù��ұ�׼��ʽ
bool IsShanghaiExchange(CString strStockCode);
bool IsShenzhenExchange(CString strStockCode);
CString GetStockSymbol(CString strStockCode);
CString GetStockExchange(CString strStockCode);
CString CreateStockCode(CString strStockExchange, CString strStockSymbol);
// ��Ʊ����ת��
CString XferSinaToStandred(CString strSina);
CString XferSinaToNetease(CString strSina);
CString XferNeteaseToStandred(CString strNetease);
CString XferNeteaseToSina(CString strNetease);
CString XferTengxunToStandred(CString strTengxun);
CString XferStandredToSina(CString strStandred);
CString XferStandredToNetease(CString strStandred);
CString XferStandredToTengxun(CString strStandred);

CString GetChinaMarketSchemaConnect();// �����й��г����ݼ���GetDefaultConnect()���Ա���һ���ԡ�����ģʽʱʹ��ChinaMarketTest����������ģʽʱʹ��ChinaMarket����
CString GetWorldMarketSchemaConnect();// ���������г����ݼ���GetDefaultConnect()���Ա���һ���ԡ�����ģʽʱʹ��world_market_test����������ģʽʱʹ��world_market����

CString ConvertValueToString(long lValue, int iDividend = 1);
CString ConvertValueToString(INT64 iValue, int iDividend = 1);
CString ConvertValueToString(int ivalue, int iDividend = 1);
CString ConvertValueToString(double dvalue, int iDividend = 1);

// ��Ȼ����������ȡ����ʵʱ���ݵ���ȫһ������Ϊ�˷�ֹ�Ժ���ܸı��Ե�ʣ����Ƿֱ�ʵ�֡�
bool ReadOneValueOfNeteaseDayLine(vector<char>& pBuffer, char* buffer, INT64& lCurrentPos);

void ZoomIn(vector<double>& vData, double dLevel, double dRate);

class CRSReference {
public:
	bool m_fActive{ false };
	long m_lDayLength[4]{ 0, 0, 0, 0 }; // ���ڳ���
	long m_lStrongDayLength[4]{ 0, 0, 0, 0 }; // ��������������
	double m_dRSStrong[4]{ 50.0, 50.0, 50.0, 50.0 }; // ��ֵ��˼Ϊ���ڴ�ֵ����ֵ��˼Ϊ���ڴ�ֵ
};

bool ConvertToJSon(ptree& pt, CWebDataPtr pWebData);
bool IsJsonReportingrror(ptree& pt, string& s); // ���json�Ƿ�Ϊ���󱨸�
