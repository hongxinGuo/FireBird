#pragma once

#include<time.h>

#include"WebData.h"

using namespace std;
#include<vector>

#include<boost/property_tree/ptree.hpp>
#include<boost/property_tree/json_parser.hpp>
using namespace boost::property_tree;

// ʱ��任����buffer�е��ַ�������strFormat����ʽ�任��time_t��ʽ������ʱ�䣬����UTC��GMT����׼ʱ��
CString ConvertDateToString(long lDate);
time_t ConvertBufferToTime(CString strFormat, const char* bufferMarketTime, time_t tTimeZoneOffset = -8 * 3600); // Ĭ�ϲ��ö�������׼ʱ��
time_t ConvertStringToTime(CString strFormat, CString strMarketTime, time_t tTimeZoneOffset = -8 * 3600); // Ĭ�ϲ��ö�������׼ʱ��

// ʱ��ת������������time_tʹ��UTC��GMT����׼��������ʽ��ʱ��Ϊ���г��ı�׼ʱ�䣬Ĭ�ϲ��ö�������׼ʱ�䡣
time_t TransferToTTime(long lDate, time_t tTimeZone, long lTime); // ������(YYYYMMDD)ת��Ϊtime_t��ʽ.
long TransferToDate(time_t tUTC, time_t tTimeZone = -8 * 3600) noexcept;// ��ʱ��ת��Ϊ����(YYYYMMDD)��ʽ, Ĭ�϶�������׼ʱ��
long TransferToTime(time_t tUTC, time_t tTimeZone = -8 * 3600) noexcept; // ��ʱ��ת��Ϊ������HHMMSS)��ʽ, Ĭ�϶�������׼ʱ��
INT64 TransferToDateTime(time_t tUTC, time_t tTimeZone = -8 * 3600) noexcept; // ��ʱ��ת��Ϊ������YYYYMMDDHHMMSS)��ʽ, Ĭ�϶�������׼ʱ��
long TransferToDate(const tm* ptm) noexcept; // ��ʱ��ת��Ϊ����(YYYYMMDD)��ʽ
long TransferToTime(const tm* ptm) noexcept; // ��ʱ��ת��Ϊ����(HHMMSS)��ʽ
INT64 TransferToDateTime(const tm* ptm) noexcept; // ��ʱ��ת��Ϊ����(YYYYMMDDHHMMSS)��ʽ
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

CString GetSchemaConnect(CString strSchema);// �������ݼ���GetDefaultConnect()���Ա���һ���ԡ�����strSchemaΪ���г�������

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

bool ConvertToJSON(ptree& pt, CWebDataPtr pWebData);
bool ConvertToJSON(ptree& pt, string s);
bool IsJsonReportingrror(ptree& pt, string& s); // ���json�Ƿ�Ϊ���󱨸�

CString FormatToMK(long long iNumber); // ������ת������M����KΪ��λ

// ʱ��֧�ֺ���
void GetUTCTimeStruct(tm* tm_, const time_t* tUTC);
void GetMarketTimeStruct(tm* tm_, time_t tUTC, const time_t tTimeZone);

void ReportJSonErrorToSystemMessage(CString strPrefix, ptree_error& e);
void ReportErrorToSystemMessage(CString strPrefix, exception& e);
