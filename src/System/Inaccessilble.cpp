#include "pch.h"

import FireBird.FinnhubInquiryType;

import FireBird.System.Inaccessible;

std::string Test_gl_sFinnhubInaccessibleSymbol = R"(
{ "UpdateDate" : 20221205,
 "InaccessibleSymbol" :
[
{
	"Function" : "StockFundamentalsCompanyProfileConcise",
	"Symbol" : ["SS", "SZ"]
},
{
	"Function" : "StockFundamentalsBasicFinancials",
	"Symbol" : ["SS", "SZ"]
}
]})";

CInaccessible::CInaccessible() {
	m_sFunction = _T("");
	m_iFunction = 0;
	m_vSymbol.clear();
	m_setSymbol.clear();
}

CInaccessible::CInaccessible(const CString& sFunction, const int iFunction, const vector<CString>& vSymbol) {
	m_sFunction = sFunction;
	m_iFunction = iFunction;
	for (auto& s : vSymbol) {
		m_vSymbol.push_back(s);
		m_setSymbol.insert(s);
	}
}

bool CInaccessible::Assign(const CString& sFunction, const int iFunction, const vector<CString>& vSymbol) {
	m_sFunction = sFunction;
	m_iFunction = iFunction;
	m_vSymbol.clear();
	m_setSymbol.clear();
	for (auto& s : vSymbol) {
		m_vSymbol.push_back(s);
		m_setSymbol.insert(s);
	}
	return true;
}

bool CInaccessible::AddSymbol(const CString& sSymbolName) {
	m_vSymbol.push_back(sSymbolName);
	m_setSymbol.insert(sSymbolName);
	return true;
}

bool CInaccessible::DeleteSymbol(const CString& sSymbolName) {
	if (m_setSymbol.contains(sSymbolName)) { // 集合中存在此元素？
		m_setSymbol.erase(sSymbolName);
		for (int position = 0; position < m_vSymbol.size(); position++) {
			if (m_vSymbol.at(position).Compare(sSymbolName) == 0) {
				m_vSymbol.erase(m_vSymbol.begin() + position);
				break;
			}
		}
	}
	return true;
}

bool CInaccessible::HaveSymbol(const CString& sSymbol) const {
	if (m_setSymbol.contains(sSymbol)) return true;
	return false;
}

bool CInaccessible::HaveSymbol() const {
	if (m_vSymbol.empty()) return false;
	return true;
}
