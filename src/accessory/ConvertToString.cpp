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

// 将utf-8字符串转化为CString
CString XferToCString(const std::string& s) {
	// ReSharper disable once CppJoinDeclarationAndAssignment
	CString strName3; // 将声明和赋值合并时出现运行结果错误，原因不明
	wstring wsName;
	// ReSharper disable once CppJoinDeclarationAndAssignment
	CStringW strWName; // 将声明和赋值合并时出现运行结果错误，原因不明

	wsName = to_wide_string(s); // 将中文utf8转成宽字节字符串
	strWName = wsName.c_str(); // 将标准库的宽字节字符串转换成CStringW制式，
	strName3 = strWName; // 将CStringW制式转换成CString
	return strName3;
}

CString FormatToMK(long iNumber) {
	string s;
	if (iNumber > 1000000) { // 1M以上的流量？
		s = fmt::format("{:#4L}M", iNumber / 1000000);
	}
	else if (iNumber > 1024) { // 1K以上的流量？
		s = fmt::format("{:4L}K", iNumber / 1024);
	}
	else {
		s = fmt::format("{:#4L}", iNumber);
	}

	return s.c_str();
}
