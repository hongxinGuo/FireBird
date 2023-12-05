#include "pch.h"

#include"FinnhubInquiryType.h"

#include "FinnhubInaccessibleExchange.h"

using namespace std;
#include<string>
#include<fstream>
#include<memory>
using std::fstream;
using std::ios;
using std::make_shared;
using std::exception;

std::string Test_gl_sFinnhubInaccessibleExchange = R"(
{ "UpdateDate" : 20221205,
 "InaccessibleExchange" :
[
{
	"Function" : "StockFundamentalsCompanyProfileConcise",
	"Exchange" : ["SS", "SZ"]
},
{
	"Function" : "StockFundamentalsBasicFinancials",
	"Exchange" : ["SS", "SZ"]
}
]})";

CInaccessibleExchanges::CInaccessibleExchanges() {
	m_sFunction = _T("");
	m_iFunction = 0;
	m_vExchange.clear();
	m_setExchange.clear();
}

CInaccessibleExchanges::CInaccessibleExchanges(const CString& sFunction, const int iFunction, const vector<CString>& vExchange) {
	m_sFunction = sFunction;
	m_iFunction = iFunction;
	for (auto& s : vExchange) {
		m_vExchange.push_back(s);
		m_setExchange.insert(s);
	}
}

bool CInaccessibleExchanges::Assign(const CString& sFunction, const int iFunction, const vector<CString>& vExchange) {
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

bool CInaccessibleExchanges::AddExchange(const CString& sExchangeName) {
	m_vExchange.push_back(sExchangeName);
	m_setExchange.insert(sExchangeName);
	return true;
}

bool CInaccessibleExchanges::DeleteExchange(const CString& sExchangeName) {
	if (m_setExchange.contains(sExchangeName)) { // 集合中存在此元素？
		m_setExchange.erase(sExchangeName);
		for (int position = 0; position < m_vExchange.size(); position++) {
			if (m_vExchange.at(position).Compare(sExchangeName) == 0) {
				m_vExchange.erase(m_vExchange.begin() + position);
				break;
			}
		}
	}
	return true;
}

bool CInaccessibleExchanges::HaveExchange(const CString& sExchange) const {
	if (m_setExchange.contains(sExchange)) return true;
	return false;
}

bool CInaccessibleExchanges::HaveExchange() const {
	if (m_vExchange.empty()) return false;
	return true;
}

CFinnhubInaccessibleExchange::CFinnhubInaccessibleExchange() {
	if (static int siInstance = 0; ++siInstance > 1) {
		TRACE(_T("FinnhubINaccessibleExchange全局变量只允许存在一个实例\n"));
#ifdef _DEBUG
		ASSERT(FALSE);
#endif // _DEBUG
	}

	m_fUpdate = false; // update flag
	m_fInitialized = true;
	m_strFileName = _T("FinnhubInaccessibleExchange.json"); // json file name
	m_lUpdateDate = 19800101;

	ASSERT(m_strFileName.Compare(_T("FinnhubInaccessibleExchange.json")) == 0);
	if (LoadDB()) {
		Update();
	}
}

CFinnhubInaccessibleExchange::~CFinnhubInaccessibleExchange() {
	if (m_fUpdate) {
		UpdateDiscFile();
	}
}

void CFinnhubInaccessibleExchange::UpdateDiscFile() {
	const CString strOld = m_strFileName.Left(m_strFileName.GetLength() - 4) + _T("json");
	const CString strNew = m_strFileName.Left(m_strFileName.GetLength() - 4) + _T("bak");

	DeleteFile(gl_systemConfiguration.GetDefaultFileDirectory() + strNew);
	rename(gl_systemConfiguration.GetDefaultFileDirectory() + strOld, gl_systemConfiguration.GetDefaultFileDirectory() + strNew); // 保存备份
	UpdateJson();
	SaveDB();
	SetUpdate(false);
}

bool CFinnhubInaccessibleExchange::LoadDB() {
	fstream f(gl_systemConfiguration.GetDefaultFileDirectory() + m_strFileName, ios::in);
	if (f.is_open()) {
		f >> m_finnhubInaccessibleExchange;
		return true;
	}
	return false;
}

bool CFinnhubInaccessibleExchange::LoadDB(const CString& strFileDirectory) {
	fstream f(strFileDirectory + m_strFileName, ios::in);
	if (f.is_open()) {
		f >> m_finnhubInaccessibleExchange;
		return true;
	}
	return false;
}

void CFinnhubInaccessibleExchange::SaveDB() const {
	fstream f(gl_systemConfiguration.GetDefaultFileDirectory() + m_strFileName, ios::out);
	f << m_finnhubInaccessibleExchange;
	f.close();
}

void CFinnhubInaccessibleExchange::Update() {
	try {
		m_lUpdateDate = m_finnhubInaccessibleExchange.at("UpdateDate");
	}
	catch (json::exception&) { }
	try {
		for (int i = 0; i < m_finnhubInaccessibleExchange.at(_T("InaccessibleExchange")).size(); i++) {
			const int size = m_finnhubInaccessibleExchange.at(_T("InaccessibleExchange")).at(i).at(_T("Exchange")).size();
			if (size > 0) {
				// 有exchange数据的话才建立数据集
				const auto pExchange = make_shared<CInaccessibleExchanges>();
				string s2 = m_finnhubInaccessibleExchange[_T("InaccessibleExchange")].at(i).at(_T("Function")); // 从json解析出的字符串格式为std::string
				pExchange->SetFunctionString(s2.c_str());
				pExchange->SetFunction(gl_FinnhubInquiryType.GetInquiryType(pExchange->GetFunctionString()));
				for (int j = 0; j < size; j++) {
					string s = m_finnhubInaccessibleExchange.at(_T("InaccessibleExchange")).at(i).at(_T("Exchange")).at(j);
					pExchange->AddExchange(s.c_str());
				}
				gl_finnhubInaccessibleExchange.m_mapExchange[gl_FinnhubInquiryType.GetInquiryType(pExchange->GetFunctionString())] = pExchange;
			}
		}
	}
	catch (json::exception&) { }
}

void CFinnhubInaccessibleExchange::UpdateJson() {
	m_finnhubInaccessibleExchange.clear();

	m_finnhubInaccessibleExchange["UpdateDate"] = m_lUpdateDate;
	for (const auto& pExchange : m_mapExchange) {
		if (pExchange.second->HaveExchange()) {
			// 有exchange数据的话才建立数据集
			auto jsonExchange = json{{"Function", pExchange.second->GetFunctionString()}};
			for (int i = 0; i < pExchange.second->ExchangeSize(); i++) {
				auto s = pExchange.second->GetExchange(i);
				jsonExchange[_T("Exchange")].push_back(s);
			}

			m_finnhubInaccessibleExchange[_T("InaccessibleExchange")].push_back(jsonExchange);
		}
	}
}

void CFinnhubInaccessibleExchange::DeleteExchange(int iInquiryType, const CString& strExchange) {
	if (HaveExchange(iInquiryType, strExchange)) {
		const CInaccessibleExchangesPtr pExchange = GetExchange(iInquiryType);
		pExchange->DeleteExchange(strExchange);
	}
}

bool CFinnhubInaccessibleExchange::HaveExchange(int iInquiryType, const CString& strExchangeCode) const {
	try {
		if (m_mapExchange.at(iInquiryType)->HaveExchange(strExchangeCode)) {
			return true;
		}
		return false;
	}
	catch (exception&) {
		return false;
	}
}
