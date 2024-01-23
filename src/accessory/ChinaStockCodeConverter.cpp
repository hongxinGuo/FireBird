#include"pch.h"

#include "ChinaStockCodeConverter.h"

bool IsShanghaiExchange(const CString& strStockCode) {
	const CString str = GetStockExchange(strStockCode);
	if (str.Compare(_T("SS")) == 0) return true;
	else return false;
}

bool IsShenzhenExchange(const CString& strStockCode) {
	const CString str = GetStockExchange(strStockCode);
	if (str.Compare(_T("SZ")) == 0) return true;
	else return false;
}

/// <summary>
/// ��һ�������Ĺ�Ʊ�����з�������Ų��֣�ȥ����ʾ�������Ĳ��֣�
/// Ŀǰʹ�õ��Ϻ����ڸ�ʽΪ600000.SS, 000001.SZ
/// </summary>
/// <param name="strStockCode"></param>
/// <returns></returns>
CString GetStockSymbol(const CString& strStockCode) {
	const long lLength = strStockCode.GetLength();
	ASSERT(lLength > 3);
	CString str = strStockCode.Left(lLength - 3);
	return str;
}

/// <summary>
/// ��һ�������Ĺ�Ʊ�����з�������������֣�ȥ����ʾ���ŵĲ��֣�
/// Ŀǰʹ�õ��Ϻ����ڸ�ʽΪ600000.SS��000001.SZ
/// </summary>
/// <param name="strStockCode"></param>
/// <returns></returns>
CString GetStockExchange(const CString& strStockCode) {
	return (strStockCode.Right(2));
}

/// <summary>
/// ʹ�ý������ͷ�������������Ʊ���롣
/// ��ʽΪ ����.�������� SS + 600000 = 600000.SS
/// </summary>
/// <param name="strStockExchange"></param>
/// <param name="strStockSymbol"></param>
/// <returns></returns>
CString CreateStockCode(const CString& strStockExchange, const CString& strStockSymbol) {
	return strStockSymbol + _T(".") + strStockExchange;
}

CString XferSinaToStandard(const CString& strSina) {
	const CString strSymbol = strSina.Right(6);
	if (strSina.Left(2).Compare(_T("sh")) == 0) {
		return strSymbol + _T(".") + _T("SS");
	}
	else {
		ASSERT(strSina.GetAt(1) == 'z');
		return strSymbol + _T(".") + _T("SZ");
	}
}

CString XferSinaToStandard(string_view strSina) {
	const CString strSymbol = CString(strSina.data() + 2, 6);
	if (strSina.at(1) == 'h') {
		return strSymbol + _T(".") + _T("SS");
	}
	else {
		return strSymbol + _T(".") + _T("SZ");
	}
}

CString XferSinaToNetease(const CString& strSina) {
	const CString strSymbol = strSina.Right(6);
	if (strSina.Left(2).Compare(_T("sh")) == 0) {
		return _T("0") + strSymbol;
	}
	else {
		ASSERT(strSina.GetAt(1) == 'z');
		return _T("1") + strSymbol;
	}
}

CString XferNeteaseToStandard(const CString& strNetease) {
	const CString strSymbol = strNetease.Right(6);
	if (strNetease.GetAt(0) == '0') {
		return strSymbol + _T(".") + _T("SS");
	}
	else {
		ASSERT(strNetease.GetAt(0) == '1');
		return strSymbol + _T(".") + _T("SZ");
	}
}

CString XferNeteaseToSina(const CString& strNetease) {
	const CString strSymbol = strNetease.Right(6);
	if (strNetease.GetAt(0) == '0') {
		return _T("sh") + strSymbol;
	}
	else {
		ASSERT(strNetease.GetAt(0) == '1');
		return _T("sz") + strSymbol;
	}
}

CString XferTengxunToStandard(const CString& strTengxun) {
	return XferSinaToStandard(strTengxun);
}

CString XferStandardToSina(const CString& strStandard) {
	const CString strSymbol = strStandard.Left(6);
	if (IsShanghaiExchange(strStandard)) {
		return _T("sh") + strSymbol;
	}
	else {
		ASSERT(strStandard.GetAt(strStandard.GetLength() - 1) == 'Z');
		return _T("sz") + strSymbol;
	}
}

CString XferStandardToNetease(const CString& strStandard) {
	const CString strSymbol = strStandard.Left(6);
	if (IsShanghaiExchange(strStandard)) {
		return _T("0") + strSymbol;
	}
	else {
		ASSERT(strStandard.GetAt(strStandard.GetLength() - 1) == 'Z');
		return _T("1") + strSymbol;
	}
}

CString XferStandardToTengxun(const CString& strStandard) {
	return XferStandardToSina(strStandard);
}
