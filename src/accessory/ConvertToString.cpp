#include"pch.h"

#include"ConvertToString.h"

CString ConvertValueToString(const long lValue, const int iDividend) {
	const double d = (static_cast<double>(lValue)) / iDividend;

	string s = fmt::format("{:.6f}", d);
	return s.c_str();
}

CString ConvertValueToString(const int iValue, const int iDividend) {
	const double d = (static_cast<double>(iValue)) / iDividend;

	string s = fmt::format("{:.6f}", d);
	return s.c_str();
}

CString ConvertValueToString(const INT64 iValue, const int iDividend) {
	const double d = (static_cast<double>(iValue)) / iDividend;

	string s = fmt::format("{:.6f}", d);
	return s.c_str();
}

CString ConvertValueToString(const double dValue, const int iDividend) {
	const double d = dValue / iDividend;

	string s = fmt::format("{:.6f}", d);
	return s.c_str();
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
	// ReSharper disable once CppJoinDeclarationAndAssignment
	CString strName3; // �������͸�ֵ�ϲ�ʱ�������н������ԭ����
	wstring wsName;
	// ReSharper disable once CppJoinDeclarationAndAssignment
	CStringW strWName; // �������͸�ֵ�ϲ�ʱ�������н������ԭ����

	wsName = to_wide_string(s); // ������utf8ת�ɿ��ֽ��ַ���
	strWName = wsName.c_str(); // ����׼��Ŀ��ֽ��ַ���ת����CStringW��ʽ��
	strName3 = strWName; // ��CStringW��ʽת����CString
	return strName3;
}

CString FormatToMK(long iNumber) {
	string s;
	if (iNumber > 1000000) { // 1M���ϵ�������
		s = fmt::format("{:#4L}M", iNumber / 1000000);
	}
	else if (iNumber > 1024) { // 1K���ϵ�������
		s = fmt::format("{:4L}K", iNumber / 1024);
	}
	else {
		s = fmt::format("{:#4L}", iNumber);
	}

	return s.c_str();
}
