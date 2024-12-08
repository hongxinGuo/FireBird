#include "pch.h"

#include"globedef.h"

import System.Configuration;
#include"nlohmannJsonDeclaration.h" // ����˳�����json������ʹ�ô�ordered_json,�Ա�֤����������������ǰ��˳��һ�¡�
#include"nlohmann/json.hpp"

#include"FinnhubInquiryType.h"

#include "FinnhubInaccessibleExchange.h"

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

CFinnhubInaccessibleExchange::CFinnhubInaccessibleExchange() {
	if (static int siInstance = 0; ++siInstance > 1) {
		TRACE(_T("FinnhubInaccessibleExchangeȫ�ֱ���ֻ�������һ��ʵ��\n"));
		ASSERT(FALSE);
	}

	ASSERT(m_strFileName.Compare(_T("FinnhubInaccessibleExchange.json")) == 0);
	if (LoadDB()) {
		Update();
	}
}

CFinnhubInaccessibleExchange::~CFinnhubInaccessibleExchange() {
	if (IsUpdateDB()) {
		UpdateDB();
	}
}

void CFinnhubInaccessibleExchange::UpdateDB() {
	const CString strOld = m_strFileName.Left(m_strFileName.GetLength() - 4) + _T("json");
	const CString strNew = m_strFileName.Left(m_strFileName.GetLength() - 4) + _T("bak");

	DeleteFile(gl_systemConfiguration.GetConfigurationFileDirectory() + strNew);
	rename(gl_systemConfiguration.GetConfigurationFileDirectory() + strOld, gl_systemConfiguration.GetConfigurationFileDirectory() + strNew); // ���汸��
	UpdateJson();
	SaveDB();
	SetUpdateDB(false);
}

bool CFinnhubInaccessibleExchange::LoadDB() {
	CString str = gl_systemConfiguration.GetConfigurationFileDirectory() + m_strFileName;
	fstream f(gl_systemConfiguration.GetConfigurationFileDirectory() + m_strFileName, ios::in);
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
	fstream f(gl_systemConfiguration.GetConfigurationFileDirectory() + m_strFileName, ios::out);
	f << m_finnhubInaccessibleExchange;
	f.close();
}

void CFinnhubInaccessibleExchange::Update() {
	try {
		m_lUpdateDate = m_finnhubInaccessibleExchange.at("UpdateDate");
	} catch (json::exception&) {}
	try {
		for (size_t i = 0; i < m_finnhubInaccessibleExchange.at(_T("InaccessibleExchange")).size(); i++) {
			const auto size = m_finnhubInaccessibleExchange.at(_T("InaccessibleExchange")).at(i).at(_T("Exchange")).size();
			if (size > 0) {
				// ��exchange���ݵĻ��Ž������ݼ�
				const auto pInaccessible = make_shared<CInaccessible>();
				string s2 = m_finnhubInaccessibleExchange[_T("InaccessibleExchange")].at(i).at(_T("Function")); // ��json���������ַ�����ʽΪstd::string
				pInaccessible->SetFunctionString(s2.c_str());
				pInaccessible->SetFunction(gl_FinnhubInquiryType.GetInquiryType(pInaccessible->GetFunctionString()));
				for (size_t j = 0; j < size; j++) {
					string s = m_finnhubInaccessibleExchange.at(_T("InaccessibleExchange")).at(i).at(_T("Exchange")).at(j);
					pInaccessible->AddSymbol(s.c_str());
				}
				m_mapExchange[gl_FinnhubInquiryType.GetInquiryType(pInaccessible->GetFunctionString())] = pInaccessible;
				//gl_finnhubInaccessibleExchange.m_mapExchange[gl_FinnhubInquiryType.GetInquiryType(pInaccessible->GetFunctionString())] = pInaccessible;
			}
		}
	} catch (json::exception&) {}
}

void CFinnhubInaccessibleExchange::UpdateJson() {
	m_finnhubInaccessibleExchange.clear();

	m_finnhubInaccessibleExchange["UpdateDate"] = m_lUpdateDate;
	for (const auto& val : m_mapExchange | std::views::values) {
		if (val->HaveSymbol()) {
			// ��exchange���ݵĻ��Ž������ݼ�
			auto jsonExchange = json{ { "Function", val->GetFunctionString() } };
			for (int i = 0; i < val->SymbolSize(); i++) {
				auto s = val->GetSymbol(i);
				jsonExchange[_T("Exchange")].push_back(s);
			}

			m_finnhubInaccessibleExchange[_T("InaccessibleExchange")].push_back(jsonExchange);
		}
	}
}

void CFinnhubInaccessibleExchange::DeleteExchange(int iInquireType, const CString& strExchange) {
	if (HaveExchange(iInquireType, strExchange)) {
		const CInaccessibleExchangesPtr pExchange = GetExchange(iInquireType);
		pExchange->DeleteSymbol(strExchange);
	}
}

bool CFinnhubInaccessibleExchange::HaveExchange(int iInquireType, const CString& strExchangeCode) const {
	try {
		if (m_mapExchange.at(iInquireType)->HaveSymbol(strExchangeCode)) {
			return true;
		}
		return false;
	} catch (exception&) {
		return false;
	}
}
