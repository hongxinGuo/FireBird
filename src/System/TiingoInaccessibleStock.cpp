#include "pch.h"

#include"FinnhubInquiryType.h"

#include "TiingoInaccessibleStock.h"

#include <fstream>

std::string Test_gl_sTiingoInaccessibleStock = R"(
{ "UpdateDate" : 20221205,
 "InaccessibleStock" :
[
{
	"Function" : "StockDayLine",
	"Stock" : ["A", "AA"]
},
{
	"Function" : "StockCompanyProfile",
	"Stock" : ["AAPL", "IBM"]
}
]})";

CTiingoInaccessibleStock::CTiingoInaccessibleStock() {
	if (static int siInstance = 0; ++siInstance > 1) {
		TRACE(_T("TiingoInaccessibleStock全局变量只允许存在一个实例\n"));
		ASSERT(FALSE);
	}

	ASSERT(m_strFileName == "TiingoInaccessibleStock.json");
	if (LoadDB()) {
		Update();
		m_tiingoInaccessibleStock.clear();
	}
}

CTiingoInaccessibleStock::~CTiingoInaccessibleStock() {
	if (IsUpdateDB()) {
		UpdateDB();
	}
}

void CTiingoInaccessibleStock::UpdateDB() {
	const string strOld = m_strFileName.substr(0, m_strFileName.length() - 4) + "json";
	const string strNew = m_strFileName.substr(0, m_strFileName.length() - 4) + "bak";

	std::filesystem::remove(gl_systemConfiguration.GetConfigurationFileDirectory() + strNew);
	rename((gl_systemConfiguration.GetConfigurationFileDirectory() + strOld).c_str(), (gl_systemConfiguration.GetConfigurationFileDirectory() + strNew).c_str()); // 保存备份
	UpdateJson();
	SaveDB();
	SetUpdateDB(false);
}

bool CTiingoInaccessibleStock::LoadDB() {
	string str = gl_systemConfiguration.GetConfigurationFileDirectory() + m_strFileName;
	fstream f(gl_systemConfiguration.GetConfigurationFileDirectory() + m_strFileName, ios::in);
	if (f.is_open()) {
		f >> m_tiingoInaccessibleStock;
		return true;
	}
	return false;
}

bool CTiingoInaccessibleStock::LoadDB(const string& strFileDirectory) {
	fstream f(strFileDirectory + m_strFileName, ios::in);
	if (f.is_open()) {
		f >> m_tiingoInaccessibleStock;
		return true;
	}
	return false;
}

void CTiingoInaccessibleStock::SaveDB() const {
	fstream f(gl_systemConfiguration.GetConfigurationFileDirectory() + m_strFileName, ios::out);
	f << m_tiingoInaccessibleStock;
	f.close();
}

void CTiingoInaccessibleStock::Update() {
	try {
		m_lUpdateDate = m_tiingoInaccessibleStock.at("UpdateDate");
	} catch (nlohmannJson::exception&) {}
	try {
		for (size_t i = 0; i < m_tiingoInaccessibleStock.at("InaccessibleStock").size(); i++) {
			const auto size = m_tiingoInaccessibleStock.at("InaccessibleStock").at(i).at("Stock").size();
			if (size > 0) {
				// 有stock数据的话才建立数据集
				const auto pInaccessible = make_shared<CInaccessible>();
				string s2 = m_tiingoInaccessibleStock["InaccessibleStock"].at(i).at("Function"); // 从json解析出的字符串格式为std::string
				pInaccessible->SetFunctionString(s2);
				pInaccessible->SetFunction(gl_FinnhubInquiryType.GetInquiryType(pInaccessible->GetFunctionString()));
				for (size_t j = 0; j < size; j++) {
					string s = m_tiingoInaccessibleStock.at("InaccessibleStock").at(i).at("Stock").at(j);
					pInaccessible->AddSymbol(s);
				}
				m_mapStock[gl_FinnhubInquiryType.GetInquiryType(pInaccessible->GetFunctionString())] = pInaccessible;
				//gl_tiingoInaccessibleStock.m_mapStock[gl_FinnhubInquiryType.GetInquiryType(pInaccessible->GetFunctionString())] = pInaccessible;
			}
		}
	} catch (nlohmannJson::exception&) {}
}

void CTiingoInaccessibleStock::UpdateJson() {
	m_tiingoInaccessibleStock.clear();

	m_tiingoInaccessibleStock["UpdateDate"] = m_lUpdateDate;
	for (const auto& val : m_mapStock | std::views::values) {
		if (val->HaveSymbol()) {
			// 有exchange数据的话才建立数据集
			auto jsonStock = nlohmannJson{ { "Function", val->GetFunctionString() } };
			for (size_t i = 0; i < val->Size(); i++) {
				auto s = val->GetSymbol(i);
				jsonStock["Stock"].push_back(s);
			}

			m_tiingoInaccessibleStock["InaccessibleStock"].push_back(jsonStock);
		}
	}
}

void CTiingoInaccessibleStock::DeleteStock(int iInquireType, const string& strStock) {
	if (HaveStock(iInquireType, strStock)) {
		const CInaccessibleStocksPtr pStock = GetStock(iInquireType);
		pStock->DeleteSymbol(strStock);
	}
}

bool CTiingoInaccessibleStock::HaveStock(int iInquireType, const string& strStockCode) const {
	try {
		if (m_mapStock.at(iInquireType)->HaveSymbol(strStockCode)) {
			return true;
		}
		return false;
	} catch (exception&) {
		return false;
	}
}
