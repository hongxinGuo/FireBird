#include"pch.h"

#include "ChinaStockCodeConverter.h"

string XferSinaToStandard(const CString& strSina) {
	const string strSymbol = strSina.Right(6).GetBuffer();
	if (strSina.GetAt(0) == 's') {
		switch (strSina.GetAt(1)) {
		case 'h':
			return strSymbol + _T(".") + _T("SS");
		case 'z':
			return strSymbol + _T(".") + _T("SZ");
		default: ;
		}
	}
	throw std::exception("XferSinaToStandard bad header");
}

string XferSinaToStandard(const string_view& svSina) {
	const string strSymbol(svSina.data() + 2, 6);
	if (svSina.at(0) == 's') {
		switch (svSina.at(1)) {
		case 'h':
			return strSymbol + _T(".") + _T("SS");
		case 'z':
			return strSymbol + _T(".") + _T("SZ");
		default: ;
		}
	}
	throw exception("XferSinaToStandard bad header");
}

CString XferSinaToNetease(const CString& strSina) {
	const CString strSymbol = strSina.Right(6);
	if (strSina.GetAt(0) == 's') {
		switch (strSina.GetAt(1)) {
		case 'h':
			return _T("0") + strSymbol;
		case 'z':
			return _T("1") + strSymbol;
		default: ;
		}
	}
	throw exception("XferSinaToNetease bad header");
}

CString XferNeteaseToStandard(CString strNetease) {
	if (strNetease.GetLength() != 7) throw exception("XferNeteaseToStandard bad format");
	const CString strSymbol = strNetease.Right(6);
	switch (strNetease.GetAt(0)) {
	case '0':
		return strSymbol + _T(".") + _T("SS");
	case '1':
		return strSymbol + _T(".") + _T("SZ");
	default: ;
	}
	throw exception("XferNeteaseToStandard bad header");
}

CString XferNeteaseToStandard(const string_view& svNetease) {
	if (svNetease.length() != 7) throw exception("XferNeteaseToStandard bad format");
	const CString strSymbol(svNetease.data() + 1, 6);
	switch (svNetease.at(0)) {
	case '0':
		return strSymbol + _T(".") + _T("SS");
	case '1':
		return strSymbol + _T(".") + _T("SZ");
	default: ;
	}
	throw exception("XferNeteaseToStandard bad header");
}

CString XferNeteaseToSina(const CString& strNetease) {
	const CString strSymbol = strNetease.Right(6);
	switch (strNetease.GetAt(0)) {
	case '0':
		return _T("sh") + strSymbol;
	case '1':
		return _T("sz") + strSymbol;
	default: ;
	}
	throw exception("XferNeteaseToSina bad header");
}

CString XferStandardToSina(const CString& strStandard) {
	const CString strSymbol = strStandard.Left(6);
	if (strStandard.GetAt(strStandard.GetLength() - 2) == 'S') {
		switch (strStandard.GetAt(strStandard.GetLength() - 1)) {
		case 'S':
			return _T("sh") + strSymbol;
		case 'Z':
			return _T("sz") + strSymbol;
		default: ;
		}
	}
	throw exception("XferSinaToStandard bad header");
}

CString XferStandardToNetease(const CString& strStandard) {
	const CString strSymbol = strStandard.Left(6);
	if (strStandard.GetAt(strStandard.GetLength() - 2) == 'S') {
		switch (strStandard.GetAt(strStandard.GetLength() - 1)) {
		case 'S':
			return _T("0") + strSymbol;
		case 'Z':
			return _T("1") + strSymbol;
		default: ;
		}
	}
	throw exception("XferSinaToStandard bad header");
}
