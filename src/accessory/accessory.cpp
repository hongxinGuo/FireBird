#include "pch.h"

#include "accessory.h"

CString GetTiingoDayLineInquiryParam(const CString& strSymbol, long lStartDate, long lCurrentDate) {
	CString strParam = _T("");
	char buffer[50];
	const long year = lCurrentDate / 10000;
	const long month = lCurrentDate / 100 - year * 100;
	const long date = lCurrentDate - year * 10000 - month * 100;

	const long year2 = lStartDate / 10000;
	const long month2 = lStartDate / 100 - year2 * 100;
	const long date2 = lStartDate - year2 * 10000 - month2 * 100;

	strParam += strSymbol;
	strParam += _T("/prices?&startDate=");
	sprintf_s(buffer, _T("%4d-%d-%d"), year2, month2, date2);
	CString strTemp = buffer;
	strParam += strTemp;
	strParam += _T("&endDate=");
	sprintf_s(buffer, _T("%4d-%d-%d"), year, month, date);
	strTemp = buffer;
	strParam += strTemp;

	return strParam;
}
