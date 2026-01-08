#include "pch.h"

#include "Inaccessible.h"

namespace {
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
}

CInaccessible::CInaccessible() {
	m_sFunction = "";
	m_iFunction = 0;
	m_vSymbol.clear();
	m_setSymbol.clear();
}

CInaccessible::CInaccessible(const string& sFunction, const int iFunction, const vector<string>& vSymbol) {
	m_sFunction = sFunction;
	m_iFunction = iFunction;
	for (auto& s : vSymbol) {
		m_vSymbol.push_back(s);
		m_setSymbol.insert(s);
	}
}

bool CInaccessible::Assign(const string& sFunction, const int iFunction, const vector<string>& vSymbol) {
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

void CInaccessible::AddSymbol(const string& sSymbolName) {
	m_vSymbol.push_back(sSymbolName);
	m_setSymbol.insert(sSymbolName);
}

void CInaccessible::DeleteSymbol(const string& sSymbolName) {
	if (m_setSymbol.contains(sSymbolName)) { // 集合中存在此元素？
		m_setSymbol.erase(sSymbolName);
		auto it = ranges::find(m_vSymbol, sSymbolName);
		if (it != m_vSymbol.end()) {
			m_vSymbol.erase(it);
		}
	}
}
void CInaccessible::Clear() {
	m_vSymbol.clear();
	m_setSymbol.clear();
}

bool CInaccessible::HaveSymbol(const string& sSymbol) const {
	if (m_setSymbol.contains(sSymbol)) return true;
	return false;
}
