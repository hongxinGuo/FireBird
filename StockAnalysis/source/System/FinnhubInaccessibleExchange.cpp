#include "pch.h"
#include "FinnhubInaccessibleExchange.h"

CInaccessibleExchanges::CInaccessibleExchanges(void) {
	m_sFunction = _T("");
	m_iFunction = 0;
	m_vExchange.clear();
	m_setExchange.clear();
}

CInaccessibleExchanges::CInaccessibleExchanges(string sFunction, int iFunction, vector<string>& vExchange) {
	m_sFunction = sFunction;
	m_iFunction = iFunction;
	for (auto& s : vExchange) {
		m_vExchange.push_back(s);
		m_setExchange.insert(s);
	}
}

CInaccessibleExchanges::~CInaccessibleExchanges() {
}

bool CInaccessibleExchanges::Assign(string sFunction, int iFunction, vector<string>& vExchange) {
	m_sFunction = sFunction;
	m_iFunction = iFunction;
	m_vExchange.clear();
	m_setExchange.clear();
	for (auto& s : vExchange) {
		m_vExchange.push_back(s);
		m_setExchange.insert(s);
	}
	return true;
}

bool CInaccessibleExchanges::AddExchange(string sExchangeName) {
	m_vExchange.push_back(sExchangeName);
	m_setExchange.insert(sExchangeName);
	return true;
}

bool CInaccessibleExchanges::DeleteExchange(string sExchangeName) {
	int position = 0;
	if (m_setExchange.count(sExchangeName) > 0) { // 集合中存在此元素？
		m_setExchange.erase(sExchangeName);
		for (position = 0; position < m_vExchange.size(); position++) {
			if (m_vExchange.at(position).compare(sExchangeName) == 0) {
				m_vExchange.erase(m_vExchange.begin() + position);
				break;
			}
		}
	}
	return true;
}

bool CInaccessibleExchanges::HaveExchange(string sExchange) {
	if (m_setExchange.count(sExchange) > 0) return true;
	else return false;
}

CFinnhubInaccessibleExchange::CFinnhubInaccessibleExchange() {
	static int siInstance = 0;

	if (++siInstance > 1) {
		TRACE(_T("GlobeOption全局变量只允许存在一个实例\n"));
#ifdef _DEBUG
		ASSERT(FALSE);
#endif // _DEBUG
	}
}

CFinnhubInaccessibleExchange::~CFinnhubInaccessibleExchange() {
}

bool CFinnhubInaccessibleExchange::LoadDB(void) {
	return false;
}

void CFinnhubInaccessibleExchange::SaveDB(void) {
}

void CFinnhubInaccessibleExchange::CreateDefaultVector(void) {
}