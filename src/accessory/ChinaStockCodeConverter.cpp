#include"pch.h"

#include "ChinaStockCodeConverter.h"

string XferSinaToStandard(const string& strSina) {
	ASSERT(strSina.length() >= 6);
	const string strSymbol = strSina.substr(strSina.length() - 6, 6);
	if (strSina.at(0) == 's') {
		switch (strSina.at(1)) {
		case 'h':
			return strSymbol + "." + "SS";
		case 'z':
			return strSymbol + "." + "SZ";
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
			return strSymbol + "." + "SS";
		case 'z':
			return strSymbol + "." + "SZ";
		default: ;
		}
	}
	throw exception("XferSinaToStandard bad header");
}

string XferStandardToSina(const string& strStandard) {
	const string strSymbol = strStandard.substr(0, 6);
	if (strStandard.at(strStandard.length() - 2) == 'S') {
		switch (strStandard.at(strStandard.length() - 1)) {
		case 'S':
			return "sh" + strSymbol;
		case 'Z':
			return "sz" + strSymbol;
		default: ;
		}
	}
	throw exception("XferSinaToStandard bad header");
}

string XferStandardToEastmoney(const string& strStandardCode) {
	if (strStandardCode.length() != 9) throw exception("XferStandardToEastmoney bad format");
	const string strSymbol = strStandardCode.substr(0, 6);
	if (strStandardCode.at(strStandardCode.length() - 2) == 'S') {
		switch (strStandardCode.at(strStandardCode.length() - 1)) {
		case 'S':
			return "1." + strSymbol;
		case 'Z':
			return "0." + strSymbol;
		default: ;
		}
	}
	throw exception("XferStandardToEastmoney bad header");
}

string XferEastmoneyToStandard(const string& strEastmoneyCode) {
	if (strEastmoneyCode.length() != 8) throw exception("XferEastmoneyToStandard bad format");
	const string strSymbol = strEastmoneyCode.substr(2, 6);
	if (strEastmoneyCode.at(0) == '1') {
		return strSymbol + ".SS";
	}
	else if (strEastmoneyCode.at(0) == '0') {
		return strSymbol + ".SZ";
	}
	throw exception("XferEastmoneyToStandard bad format");
}
