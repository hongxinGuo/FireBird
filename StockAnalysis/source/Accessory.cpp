#include"pch.h"
#include"globedef.h"
#include "Accessory.h"

long GetFormatedDate(void) {
	time_t tLocal;
	tm tmLocal;
	long lLocalDate;
	time(&tLocal);
	gmtime_s(&tmLocal, &tLocal);
	lLocalDate = (tmLocal.tm_year + 1900) * 10000 + (tmLocal.tm_mon + 1) * 100 + tmLocal.tm_mday;

	return lLocalDate;
}

time_t ConvertBufferToTime(CString strFormat, const char* buffer) {
	time_t tt{ 0 };
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

time_t ConvertStringToTime(CString strFormat, CString strTime) {
	time_t tt{ 0 };
	tm tm_;
	int year, month, day, hour, minute, second;

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

time_t FormatToTTime(long lDate, long lTime) {
	ASSERT(lDate > 19700000);
	const long lYear = lDate / 10000;
	const long lMonth = (lDate - lYear * 10000) / 100;
	const long lD = (lDate - lYear * 10000 - lMonth * 100);
	const long lHour = lTime / 10000;
	const long lMinute = (lTime - lHour * 10000) / 100;
	const long lSecond = lTime - lHour * 10000 - lMinute * 100;
	const CTime ct(lYear, lMonth, lD, lHour, lMinute, lSecond);	// 北京时间15时即UTC7时
	return (ct.GetTime());
}

long FormatToDate(time_t const tt) noexcept {
	tm tm_;
	localtime_s(&tm_, &tt);
	return((tm_.tm_year + 1900) * 10000 + (tm_.tm_mon + 1) * 100 + tm_.tm_mday);
}

long FormatToTime(time_t const tt) noexcept {
	tm tm_;
	localtime_s(&tm_, &tt);
	return(tm_.tm_hour * 10000 + tm_.tm_min * 100 + tm_.tm_sec);
}

INT64 FormatToDateTime(time_t const tt) noexcept {
	tm tm_;
	localtime_s(&tm_, &tt);
	return((static_cast<INT64>(tm_.tm_year) + 1900) * 10000000000 + (static_cast<INT64>(tm_.tm_mon) + 1) * 100000000 + static_cast<INT64>(tm_.tm_mday) * 1000000 + tm_.tm_hour * 10000 + tm_.tm_min * 100 + tm_.tm_sec);
}

long FormatToDate(const tm* ptm) noexcept {
	return((ptm->tm_year + 1900) * 10000 + (ptm->tm_mon + 1) * 100 + ptm->tm_mday);
}

long FormatToTime(const tm* ptm) noexcept {
	return(ptm->tm_hour * 10000 + ptm->tm_min * 100 + ptm->tm_sec);
}

INT64 FormatToDateTime(const tm* ptm) noexcept {
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
/// 目前使用的上海深圳格式为sh600000、sz000001，今后要改为600000.SS, 000001.SZ
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

////////////////////////////////////////////////////////////////////////////////
//
// 所有数据集的GetDefaultConnect()函数皆调用此函数完成具体工作，以保证一致性。
//
// 测试模式时使用ChinaMarketTest驱动，正常模式时使用ChinaMarket驱动，以保证使用不同的Schema。
//
/////////////////////////////////////////////////////////////////////////////////
CString GetChinaMarketSchemaConnect() {
	if (!gl_fNormalMode) {
		gl_fTestMode = true;
		return _T("DSN=ChinaMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"); // ChinaMarketTest操作的是TestData Schema
	}
	else {
		gl_fTestMode = false;
		return _T("DSN=ChinaMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"); // ChinaMarket操作的是Chinamarket Schema
	}
}

////////////////////////////////////////////////////////////////////////////////
//
// 所有数据集的GetDefaultConnect()函数皆调用此函数完成具体工作，以保证一致性。
//
// 测试模式时使用ChinaMarketTest驱动，正常模式时使用ChinaMarket驱动，以保证使用不同的Schema。
//
/////////////////////////////////////////////////////////////////////////////////
CString GetWorldMarketSchemaConnect() {
	if (!gl_fNormalMode) {
		gl_fTestMode = true;
		return _T("DSN=WorldMarketTest;UID=Test;PASSWORD=test;charset=utf8mb4"); // Test操作的是world_market_test Schema
	}
	else {
		gl_fTestMode = false;
		return _T("DSN=WorldMarket;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"); // WorldMarket操作的是world_market Schema
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
	catch (ptree_error&) {
		return false;
	}
	return true;
}

bool IsJsonReportingrror(ptree& pt, string& s) {
	try {
		s = pt.get<string>(_T("error"));
	}
	catch (ptree_error&) {
		return false;
	}
	return true;
}

CString FormatToMK(long long iNumber) {
	char buffer[100];
	CString str;
	if (iNumber > 1024 * 1024) { // 1M以上的流量？
		sprintf_s(buffer, _T("%4dM"), iNumber / (1024 * 1024));
	}
	else if (iNumber > 1024) { // 1K以上的流量？
		sprintf_s(buffer, _T("%4dK"), iNumber / 1024);
	}
	else {
		sprintf_s(buffer, _T("%4d"), iNumber);
	}
	str = buffer;

	return str;
}