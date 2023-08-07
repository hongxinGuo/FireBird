#include"pch.h"

#include"ConvertToString.h"

CString ConvertValueToString(const long lValue, const int iDividend) {
	char buffer[50]{0};
	const double d = (static_cast<double>(lValue)) / iDividend;

	sprintf_s(buffer, _T("%.3f"), d);
	CString str = buffer;
	return str;
}

CString ConvertValueToString(const int iValue, const int iDividend) {
	char buffer[50]{0};
	const double d = (static_cast<double>(iValue)) / iDividend;

	sprintf_s(buffer, _T("%.3f"), d);
	CString str = buffer;

	return str;
}

CString ConvertValueToString(const INT64 iValue, const int iDividend) {
	char buffer[50]{0};
	const double d = (static_cast<double>(iValue)) / iDividend;

	sprintf_s(buffer, _T("%.3f"), d);
	CString str = buffer;
	return str;
}

CString ConvertValueToString(const double dValue, const int iDividend) {
	char buffer[50]{0};
	const double d = dValue / iDividend;

	sprintf_s(buffer, _T("%.3f"), d);
	CString str = buffer;
	return str;
}

wstring to_wide_string(const std::string& input) {
	const int iLength = static_cast<int>(input.size());
	const auto pBuffer = new char[iLength + 1];

	for (int i = 0; i < input.size(); i++) { pBuffer[i] = input.at(i); }
	pBuffer[iLength] = 0x000;
	const auto pBufferW = new WCHAR[iLength * 2];

	const long lReturnSize = MultiByteToWideChar(CP_UTF8, 0, pBuffer, iLength, pBufferW, iLength * 2);
	pBufferW[lReturnSize] = 0x000;
	wstring ws = pBufferW;

	delete[]pBuffer;
	delete[]pBufferW;

	return ws;
}

// ��utf-8�ַ���ת��ΪCString
CString XferToCString(const std::string& s) {
	CString strName3;
	wstring wsName;
	CStringW strWName;

	wsName = to_wide_string(s); // ������utf8ת�ɿ��ֽ��ַ���
	strWName = wsName.c_str(); // ����׼��Ŀ��ֽ��ַ���ת����CStringW��ʽ��
	strName3 = strWName; // ��CStringW��ʽת����CString
	return strName3;
}

CString FormatToMK(long long iNumber) {
	char buffer[100];
	if (iNumber > 1024 * 1024) { // 1M���ϵ�������
		sprintf_s(buffer, _T("%4lldM"), iNumber / (1024 * 1024));
	}
	else if (iNumber > 1024) { // 1K���ϵ�������
		sprintf_s(buffer, _T("%4lldK"), iNumber / 1024);
	}
	else { sprintf_s(buffer, _T("%4lld"), iNumber); }
	CString str = buffer;

	return str;
}
