#include"pch.h"

#include "ChinaStockCodeConverter.h"

string XferSinaToStandard(const string& strSina) {
	ASSERT(strSina.length() >= 6);
	const string strSymbol = strSina.substr(strSina.length() - 6, 6);
	if (strSina.at(0) == 's') {
		switch (strSina.at(1)) {
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

string XferSinaToNetease(const string& strSina) {
	ASSERT(strSina.length() >= 6);
	const string strSymbol = strSina.substr(strSina.length() - 6, 6);
	if (strSina.at(0) == 's') {
		switch (strSina.at(1)) {
		case 'h':
			return _T("0") + strSymbol;
		case 'z':
			return _T("1") + strSymbol;
		default: ;
		}
	}
	throw exception("XferSinaToNetease bad header");
}

string XferNeteaseToStandard(const string& strNetease) {
	if (strNetease.length() != 7) throw exception("XferNeteaseToStandard bad format");
	const string strSymbol = strNetease.substr(strNetease.length() - 6, 6);
	switch (strNetease.at(0)) {
	case '0':
		return strSymbol + _T(".") + _T("SS");
	case '1':
		return strSymbol + _T(".") + _T("SZ");
	default: ;
	}
	throw exception("XferNeteaseToStandard bad header");
}

string XferNeteaseToStandard(const string_view& svNetease) {
	if (svNetease.length() != 7) throw exception("XferNeteaseToStandard bad format");
	const string strSymbol(svNetease.data() + 1, 6);
	switch (svNetease.at(0)) {
	case '0':
		return strSymbol + _T(".") + _T("SS");
	case '1':
		return strSymbol + _T(".") + _T("SZ");
	default: ;
	}
	throw exception("XferNeteaseToStandard bad header");
}

string XferNeteaseToSina(const string& strNetease) {
	const string strSymbol = strNetease.substr(strNetease.length() - 6, 6);
	switch (strNetease.at(0)) {
	case '0':
		return _T("sh") + strSymbol;
	case '1':
		return _T("sz") + strSymbol;
	default: ;
	}
	throw exception("XferNeteaseToSina bad header");
}

string XferStandardToSina(const string& strStandard) {
	const string strSymbol = strStandard.substr(0, 6);
	if (strStandard.at(strStandard.length() - 2) == 'S') {
		switch (strStandard.at(strStandard.length() - 1)) {
		case 'S':
			return _T("sh") + strSymbol;
		case 'Z':
			return _T("sz") + strSymbol;
		default: ;
		}
	}
	throw exception("XferSinaToStandard bad header");
}

string XferStandardToNetease(const string& strStandard) {
	const string strSymbol = strStandard.substr(0, 6);
	if (strStandard.at(strStandard.length() - 2) == 'S') {
		switch (strStandard.at(strStandard.length() - 1)) {
		case 'S':
			return _T("0") + strSymbol;
		case 'Z':
			return _T("1") + strSymbol;
		default: ;
		}
	}
	throw exception("XferSinaToStandard bad header");
}
