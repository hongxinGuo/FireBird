#include "pch.h"

#include"FinnhubInquiryType.h"

#include "TiingoInaccessibleStock.h"

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

	m_fUpdateDB = false; // update flag
	m_fInitialized = true;
	m_strFileName = _T("TiingoInaccessibleStock.json"); // json file name
	m_lUpdateDate = 19800101;

	ASSERT(m_strFileName.Compare(_T("TiingoInaccessibleStock.json")) == 0);
	if (LoadDB()) {
		Update();
	}
}

CTiingoInaccessibleStock::~CTiingoInaccessibleStock() {
	if (IsUpdateDB()) {
		UpdateDB();
	}
}

void CTiingoInaccessibleStock::UpdateDB() {
	const CString strOld = m_strFileName.Left(m_strFileName.GetLength() - 4) + _T("json");
	const CString strNew = m_strFileName.Left(m_strFileName.GetLength() - 4) + _T("bak");

	DeleteFile(gl_systemConfiguration.GetConfigurationFileDirectory() + strNew);
	rename(gl_systemConfiguration.GetConfigurationFileDirectory() + strOld, gl_systemConfiguration.GetConfigurationFileDirectory() + strNew); // 保存备份
	UpdateJson();
	SaveDB();
	SetUpdateDB(false);
}

bool CTiingoInaccessibleStock::LoadDB() {
	CString str = gl_systemConfiguration.GetConfigurationFileDirectory() + m_strFileName;
	fstream f(gl_systemConfiguration.GetConfigurationFileDirectory() + m_strFileName, ios::in);
	if (f.is_open()) {
		f >> m_finnhubInaccessibleStock;
		return true;
	}
	return false;
}

bool CTiingoInaccessibleStock::LoadDB(const CString& strFileDirectory) {
	fstream f(strFileDirectory + m_strFileName, ios::in);
	if (f.is_open()) {
		f >> m_finnhubInaccessibleStock;
		return true;
	}
	return false;
}

void CTiingoInaccessibleStock::SaveDB() const {
	fstream f(gl_systemConfiguration.GetConfigurationFileDirectory() + m_strFileName, ios::out);
	f << m_finnhubInaccessibleStock;
	f.close();
}

void CTiingoInaccessibleStock::Update() {
	try {
		m_lUpdateDate = m_finnhubInaccessibleStock.at("UpdateDate");
	} catch (json::exception&) {}
	try {
		for (int i = 0; i < m_finnhubInaccessibleStock.at(_T("InaccessibleStock")).size(); i++) {
			const int size = m_finnhubInaccessibleStock.at(_T("InaccessibleStock")).at(i).at(_T("Stock")).size();
			if (size > 0) {
				// 有exchange数据的话才建立数据集
				const auto pStock = make_shared<CInaccessible>();
				string s2 = m_finnhubInaccessibleStock[_T("InaccessibleStock")].at(i).at(_T("Function")); // 从json解析出的字符串格式为std::string
				pStock->SetFunctionString(s2.c_str());
				pStock->SetFunction(gl_FinnhubInquiryType.GetInquiryType(pStock->GetFunctionString()));
				for (int j = 0; j < size; j++) {
					string s = m_finnhubInaccessibleStock.at(_T("InaccessibleStock")).at(i).at(_T("Stock")).at(j);
					pStock->AddSymbol(s.c_str());
				}
				gl_finnhubInaccessibleStock.m_mapStock[gl_FinnhubInquiryType.GetInquiryType(pStock->GetFunctionString())] = pStock;
			}
		}
	} catch (json::exception&) {}
}

void CTiingoInaccessibleStock::UpdateJson() {
	m_finnhubInaccessibleStock.clear();

	m_finnhubInaccessibleStock["UpdateDate"] = m_lUpdateDate;
	for (const auto& val : m_mapStock | std::views::values) {
		if (val->HaveSymbol()) {
			// 有exchange数据的话才建立数据集
			auto jsonStock = json{ { "Function", val->GetFunctionString() } };
			for (int i = 0; i < val->SymbolSize(); i++) {
				auto s = val->GetSymbol(i);
				jsonStock[_T("Stock")].push_back(s);
			}

			m_finnhubInaccessibleStock[_T("InaccessibleStock")].push_back(jsonStock);
		}
	}
}

void CTiingoInaccessibleStock::DeleteStock(int iInquireType, const CString& strStock) {
	if (HaveStock(iInquireType, strStock)) {
		const CInaccessibleStocksPtr pStock = GetStock(iInquireType);
		pStock->DeleteSymbol(strStock);
	}
}

bool CTiingoInaccessibleStock::HaveStock(int iInquireType, const CString& strStockCode) const {
	try {
		if (m_mapStock.at(iInquireType)->HaveSymbol(strStockCode)) {
			return true;
		}
		return false;
	} catch (exception&) {
		return false;
	}
}
