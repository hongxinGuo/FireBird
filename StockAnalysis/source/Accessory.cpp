#include"pch.h"
#include"globedef.h"
#include "Accessory.h"

#include<boost/property_tree/ptree.hpp>
#include<boost/property_tree/json_parser.hpp>
using namespace boost::property_tree;

CString ConvertDateToString(long lDate) {
	char buffer[30];
	long year = lDate / 10000;
	long month = lDate / 100 - year * 100;
	long day = lDate - year * 10000 - month * 100;

	sprintf_s(buffer, _T("%4d年%2d月%2d日"), year, month, day);
	CString str;
	str = buffer;
	return(str);
}

time_t ConvertBufferToTime(CString strFormat, const char* bufferMarketTime, time_t tTimeZoneOffset) {
	time_t tt{ 0 };
	tm tm_{ 0,0,0,0,0,0 };
	int year, month, day, hour, minute, second;

	sscanf_s(bufferMarketTime, strFormat.GetBuffer(), &year, &month, &day, &hour, &minute, &second);
	tm_.tm_year = year - 1900;
	tm_.tm_mon = month - 1;
	tm_.tm_mday = day;
	tm_.tm_hour = hour;
	tm_.tm_min = minute;
	tm_.tm_sec = second;
	tm_.tm_isdst = 0;
	tt = _mkgmtime(&tm_); // 先变成GMT时间
	if (tt > -1) {
		tt += tTimeZoneOffset; // 然后改成本市场UTC时间
	}
	return tt;
}

time_t ConvertStringToTime(CString strFormat, CString strMarketTime, time_t tTimeZoneOffset) {
	time_t tt{ 0 };
	tm tm_{ 0, 0, 0, 0, 0, 0 };
	int year, month, day, hour, minute, second;

	sscanf_s(strMarketTime.GetBuffer(), strFormat.GetBuffer(), &year, &month, &day, &hour, &minute, &second);
	tm_.tm_year = year - 1900;
	tm_.tm_mon = month - 1;
	tm_.tm_mday = day;
	tm_.tm_hour = hour;
	tm_.tm_min = minute;
	tm_.tm_sec = second;
	tm_.tm_isdst = 0;
	tt = _mkgmtime(&tm_);
	if (tt > -1) {
		tt += tTimeZoneOffset; //
	}
	return tt;
}

time_t TransferToTTime(long lDate, time_t tTimeZone, long lTime) {
	tm tmMarket{ 0, 0, 0, 0,0, 0 };

	ASSERT(lDate >= 19700101);
	tmMarket.tm_year = lDate / 10000 - 1900;
	tmMarket.tm_mon = lDate / 100 - (lDate / 10000) * 100 - 1;
	tmMarket.tm_mday = lDate - (lDate / 100) * 100;
	tmMarket.tm_hour = lTime / 10000;
	tmMarket.tm_min = lTime / 100 - (lTime / 10000) * 100;
	tmMarket.tm_sec = lTime - (lTime / 100) * 100;

	return _mkgmtime(&tmMarket) + tTimeZone;
}

long TransferToDate(time_t const tUTC, time_t tTimeZone) noexcept {
	tm tm_;
	GetMarketTimeStruct(&tm_, tUTC, tTimeZone);
	return((tm_.tm_year + 1900) * 10000 + (tm_.tm_mon + 1) * 100 + tm_.tm_mday);
}

long TransferToTime(time_t const tUTC, time_t tTimeZone) noexcept {
	tm tm_;
	GetMarketTimeStruct(&tm_, tUTC, tTimeZone);
	return(tm_.tm_hour * 10000 + tm_.tm_min * 100 + tm_.tm_sec);
}

INT64 TransferToDateTime(time_t const tUTC, time_t tTimeZone) noexcept {
	tm tm_;
	GetMarketTimeStruct(&tm_, tUTC, tTimeZone);
	return((static_cast<INT64>(tm_.tm_year) + 1900) * 10000000000 + (static_cast<INT64>(tm_.tm_mon) + 1) * 100000000 + static_cast<INT64>(tm_.tm_mday) * 1000000 + tm_.tm_hour * 10000 + tm_.tm_min * 100 + tm_.tm_sec);
}

long TransferToDate(const tm* ptm) noexcept {
	return((ptm->tm_year + 1900) * 10000 + (ptm->tm_mon + 1) * 100 + ptm->tm_mday);
}

long TransferToTime(const tm* ptm) noexcept {
	return(ptm->tm_hour * 10000 + ptm->tm_min * 100 + ptm->tm_sec);
}

INT64 TransferToDateTime(const tm* ptm) noexcept {
	return((static_cast<INT64>(ptm->tm_year) + 1900) * 10000000000 + (static_cast<INT64>(ptm->tm_mon) + 1) * 100000000 + static_cast<INT64>(ptm->tm_mday) * 1000000 + ptm->tm_hour * 10000 + ptm->tm_min * 100 + ptm->tm_sec);
}

bool IsEarlyThen(long lEarlyDate, long lLatelyDate, long lTimeSpawnOfDays) {
	ASSERT(lEarlyDate >= 19700101);
	ASSERT(lLatelyDate >= 19700101);
	CTimeSpan ts(lTimeSpawnOfDays, 0, 0, 0);
	const long year = lEarlyDate / 10000;
	const long month = lEarlyDate / 100 - year * 100;
	const long day = lEarlyDate - year * 10000 - month * 100;
	CTime ctEarly(year, month, day, 12, 0, 0);
	ctEarly += ts;
	long lNewDate = ctEarly.GetYear() * 10000 + ctEarly.GetMonth() * 100 + ctEarly.GetDay();
	return (lNewDate < lLatelyDate);
}

const static CTimeSpan s_1Day(1, 0, 0, 0);
const static CTimeSpan s_2Day(2, 0, 0, 0);
const static CTimeSpan s_3Day(3, 0, 0, 0);
const static CTimeSpan s_4Day(4, 0, 0, 0);
const static CTimeSpan s_5Day(5, 0, 0, 0);
const static CTimeSpan s_6Day(6, 0, 0, 0);
const static CTimeSpan s_7Day(7, 0, 0, 0);

long GetNextMonday(long lDate) {
	const long year = lDate / 10000;
	const long month = lDate / 100 - (lDate / 10000) * 100;
	const long mday = lDate - (lDate / 100) * 100;
	const CTime ctCurrent(year, month, mday, 12, 0, 0);
	CTime ctNext;

	ctNext = ctCurrent;
	switch (ctCurrent.GetDayOfWeek()) {
	case 2: // 星期一
		ctNext += s_7Day;
		break;
	case 3:
		ctNext += s_6Day;
		break;
	case 4:
		ctNext += s_5Day;
		break;
	case 5:
		ctNext += s_4Day;
		break;
	case 6:
		ctNext += s_3Day;
		break;
	case 7:
		ctNext += s_2Day;
		break;
	case 1:
		ctNext += s_1Day;
		break;
		break;
	default: // 不可能
		break;
	}
	const long lNextDay = ctNext.GetYear() * 10000 + ctNext.GetMonth() * 100 + ctNext.GetDay();

	return lNextDay;
}

long GetPrevMonday(long lDate) {
	const long year = lDate / 10000;
	const long month = lDate / 100 - (lDate / 10000) * 100;
	const long mday = lDate - (lDate / 100) * 100;
	const CTime ctCurrent(year, month, mday, 12, 0, 0);
	CTime ctNext;

	ctNext = ctCurrent;
	switch (ctCurrent.GetDayOfWeek()) {
	case 2: // 星期一
		ctNext -= s_7Day;
		break;
	case 3: // 星期二
		ctNext -= s_1Day;
		break;
	case 4: // 星期三
		ctNext -= s_2Day;
		break;
	case 5: // 星期四
		ctNext -= s_3Day;
		break;
	case 6:
		ctNext -= s_4Day;
		break;
	case 7: // 星期六
		ctNext -= s_5Day;
		break;
	case 1: // 星期日
		ctNext -= s_6Day;
		break;
		break;
	default: // 不可能
		break;
	}
	const long lPrevMonday = ctNext.GetYear() * 10000 + ctNext.GetMonth() * 100 + ctNext.GetDay();

	return lPrevMonday;
}

long GetCurrentMonday(long lDate) {
	const long year = lDate / 10000;
	const long month = lDate / 100 - (lDate / 10000) * 100;
	const long mday = lDate - (lDate / 100) * 100;
	const CTime ctCurrent(year, month, mday, 12, 0, 0);
	CTime ctNext;

	ctNext = ctCurrent;
	switch (ctCurrent.GetDayOfWeek()) {
	case 2: // 星期一
		break;
	case 3: // 星期二
		ctNext -= s_1Day;
		break;
	case 4: // 星期三
		ctNext -= s_2Day;
		break;
	case 5: // 星期四
		ctNext -= s_3Day;
		break;
	case 6:
		ctNext -= s_4Day;
		break;
	case 7: // 星期六
		ctNext -= s_5Day;
		break;
	case 1: // 星期日
		ctNext -= s_6Day;
		break;
		break;
	default: // 不可能
		break;
	}
	const long lCurrentMonday = ctNext.GetYear() * 10000 + ctNext.GetMonth() * 100 + ctNext.GetDay();

	return lCurrentMonday;
}

bool IsShanghaiExchange(CString strStockCode) {
	CString str = GetStockExchange(strStockCode);
	if (str.Compare(_T("SS")) == 0) return true;
	else return false;
}

bool IsShenzhenExchange(CString strStockCode) {
	CString str = GetStockExchange(strStockCode);
	if (str.Compare(_T("SZ")) == 0) return true;
	else return false;
}

/// <summary>
/// 从一个完整的股票代码中分离出符号部分（去除表示交易所的部分）
/// 目前使用的上海深圳格式为600000.SS, 000001.SZ
/// </summary>
/// <param name="strStockCode"></param>
/// <returns></returns>
CString GetStockSymbol(CString strStockCode) {
	long lLength = strStockCode.GetLength();
	ASSERT(lLength > 3);
	CString str = strStockCode.Left(lLength - 3);
	return str;
}

/// <summary>
/// 从一个完整的股票代码中分离出交易所部分（去除表示符号的部分）
/// 目前使用的上海深圳格式为600000.SS、000001.SZ
/// </summary>
/// <param name="strStockCode"></param>
/// <returns></returns>
CString GetStockExchange(CString strStockCode) {
	return(strStockCode.Right(2));
}

/// <summary>
/// 使用交易所和符号生成完整股票代码。
/// 方式为 符号.交易所。 SS + 600000 = 600000.SS
/// </summary>
/// <param name="strStockExchange"></param>
/// <param name="strStockSymbol"></param>
/// <returns></returns>
CString CreateStockCode(CString strStockExchange, CString strStockSymbol) {
	return strStockSymbol + _T(".") + strStockExchange;
}

CString XferSinaToStandred(CString strSina) {
	CString strSymbol = strSina.Right(6);
	if (strSina.Left(2).Compare(_T("sh")) == 0) {
		return strSymbol + _T(".") + _T("SS");
	}
	else {
		ASSERT(strSina.GetAt(1) == 'z');
		return strSymbol + _T(".") + _T("SZ");
	}
}

CString XferSinaToNetease(CString strSina) {
	CString strSymbol = strSina.Right(6);
	if (strSina.Left(2).Compare(_T("sh")) == 0) {
		return _T("0") + strSymbol;
	}
	else {
		ASSERT(strSina.GetAt(1) == 'z');
		return _T("1") + strSymbol;
	}
}

CString XferNeteaseToStandred(CString strNetease) {
	CString strSymbol = strNetease.Right(6);
	if (strNetease.GetAt(0) == '0') {
		return strSymbol + _T(".") + _T("SS");
	}
	else {
		ASSERT(strNetease.GetAt(0) == '1');
		return strSymbol + _T(".") + _T("SZ");
	}
}

CString XferNeteaseToSina(CString strNetease) {
	CString strSymbol = strNetease.Right(6);
	if (strNetease.GetAt(0) == '0') {
		return _T("sh") + strSymbol;
	}
	else {
		ASSERT(strNetease.GetAt(0) == '1');
		return _T("sz") + strSymbol;
	}
}

CString XferTengxunToStandred(CString strTengxun) {
	return XferSinaToStandred(strTengxun);
}

CString XferStandredToSina(CString strStandred) {
	CString strSymbol = strStandred.Left(6);
	if (IsShanghaiExchange(strStandred)) {
		return _T("sh") + strSymbol;
	}
	else {
		ASSERT(strStandred.GetAt(strStandred.GetLength() - 1) == 'Z');
		return _T("sz") + strSymbol;
	}
}

CString XferStandredToNetease(CString strStandred) {
	CString strSymbol = strStandred.Left(6);
	if (IsShanghaiExchange(strStandred)) {
		return _T("0") + strSymbol;
	}
	else {
		ASSERT(strStandred.GetAt(strStandred.GetLength() - 1) == 'Z');
		return _T("1") + strSymbol;
	}
}

CString XferStandredToTengxun(CString strStandred) {
	return XferStandredToSina(strStandred);
}

CString GetSchemaConnect(CString strSchema)
{
	if (!gl_fNormalMode) {
		gl_fTestMode = true;
		return _T("DSN=") + strSchema + _T("Test;UID=Test;PASSWORD=test;charset=utf8mb4"); // Test操作时DSN名称后要加上后缀Test
	}
	else {
		gl_fTestMode = false;
		return _T("DSN=") + strSchema + _T(";UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"); // 运行时的DSN使用原schema名称
	}
}

CString ConvertValueToString(long lValue, int iDividend) {
	char buffer[50]{ 0 };
	const double d = (static_cast<double>(lValue)) / iDividend;
	CString str;

	sprintf_s(buffer, _T("%.3f"), d);
	str = buffer;
	return str;
}

CString ConvertValueToString(int iValue, int iDividend) {
	char buffer[50]{ 0 };
	const double d = (static_cast<double>(iValue)) / iDividend;
	CString str;

	sprintf_s(buffer, _T("%.3f"), d);
	str = buffer;

	return str;
}

CString ConvertValueToString(INT64 iValue, int iDividend) {
	char buffer[50]{ 0 };
	const double d = (static_cast<double>(iValue)) / iDividend;
	CString str;

	sprintf_s(buffer, _T("%.3f"), d);
	str = buffer;
	return str;
}

CString ConvertValueToString(double dValue, int iDividend) {
	char buffer[50]{ 0 };
	const double d = dValue / iDividend;
	CString str;

	sprintf_s(buffer, _T("%.3f"), d);
	str = buffer;
	return str;
}

bool ReadOneValueOfNeteaseDayLine(vector<char>& pBuffer, char* buffer, INT64& lCurrentPos) {
	int i = 0;

	while (pBuffer.at(lCurrentPos) != ',') { // 将下一个逗号前的字符存入缓冲区. 0x2c就是逗号。
		if ((pBuffer.at(lCurrentPos) == 0x0d) || (pBuffer.at(lCurrentPos) == 0x00a) || (pBuffer.at(lCurrentPos) == 0x000) || (i > 100)) { // 遇到回车、换行或者字符串结束符,或者超过了100个字符
			return false; // 数据出错，放弃载入
		}
		buffer[i++] = pBuffer.at(lCurrentPos++);
	}
	buffer[i] = 0x000;
	lCurrentPos++;
	return true;
}

void ZoomIn(vector<double>& vData, double dLevel, double dRate) {
	double d = 0;

	for (int i = 0; i < vData.size(); i++) {
		d = dLevel + (vData.at(i) - dLevel) * dRate;
		if (d < 0) vData.at(i) = 0;
		else if (d > 100) vData.at(i) = 100;
		else vData.at(i) = d;
	}
}

bool ConvertToJSON(ptree& pt, CWebDataPtr pWebData) {
	char* pbuffer = nullptr;

	pbuffer = new char[pWebData->GetBufferLength() + 1];
	for (int i = 0; i < pWebData->GetBufferLength(); i++) {
		pbuffer[i] = pWebData->GetData(i);
	}
	pbuffer[pWebData->GetBufferLength()] = 0x000;
	string strTemp = pbuffer;

	delete pbuffer;

	return (ConvertToJSON(pt, strTemp));
}

bool ConvertToJSON(ptree& pt, string s) {
	stringstream ss(s);

	try {
		read_json(ss, pt);
	}
	catch (ptree_error& e) {
		//ReportJSonErrorToSystemMessage(_T("JSon Reading Error "), e);
		return false;
	}
	return true;
}

bool IsJsonReportingrror(ptree& pt, string& s) {
	try {
		s = pt.get<string>(_T("error"));
	}
	catch (ptree_error& e) {
		//ReportJSonErrorToSystemMessage(_T("JSon get 'error'string Error "), e);
		return false;
	}
	return true;
}

CString FormatToMK(long long iNumber) {
	char buffer[100];
	CString str;
	if (iNumber > 1024 * 1024) { // 1M以上的流量？
		sprintf_s(buffer, _T("%4lldM"), iNumber / (1024 * 1024));
	}
	else if (iNumber > 1024) { // 1K以上的流量？
		sprintf_s(buffer, _T("%4lldK"), iNumber / 1024);
	}
	else {
		sprintf_s(buffer, _T("%4lld"), iNumber);
	}
	str = buffer;

	return str;
}

void GetUTCTimeStruct(tm* tm_, const time_t* tUTC) {
	gmtime_s(tm_, tUTC);
}

void GetMarketTimeStruct(tm* tm_, time_t tUTC, const time_t tTimeZone) {
	time_t tMarket;
	tMarket = tUTC - tTimeZone;
	gmtime_s(tm_, &tMarket);
}

void ReportJSonErrorToSystemMessage(CString strPrefix, ptree_error& e) {
	CString strError = strPrefix;
	strError += e.what();
	gl_systemMessage.PushErrorMessage(strError);
}

void ReportErrorToSystemMessage(CString strPrefix, exception& e) {
	CString strError = strPrefix;
	strError += e.what();
	gl_systemMessage.PushErrorMessage(strError);
}