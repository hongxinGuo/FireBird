#include"pch.h"

#include "StockCodeConverter.h"
#include"JsonParse.h"

using namespace std;
//#include<codecvt>

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