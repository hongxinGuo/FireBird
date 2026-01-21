#include "pch.h"

#include"FinnhubInquiryType.h"

#include "FinnhubInaccessibleExchange.h"

#include <fstream>

void DeleteAllFinnhubInaccessibleUSExchange() {
	for (int i = 0; i < END_OF_ALL_INQUIRY_TYPE_; i++) {
		try {
			auto pInaccessible = gl_finnhubInaccessibleExchange.GetInaccessible(i);
			if (pInaccessible->HaveSymbol("US")) {
				pInaccessible->DeleteSymbol("US");
				gl_finnhubInaccessibleExchange.SetUpdateDB(true);
			}
		} catch (...) {
			// do nothing
		}
	}
}

CFinnhubInaccessibleExchange::CFinnhubInaccessibleExchange(const string& strFileName) {
	m_strFileName = strFileName;
	if (LoadDB()) {
		Update();
	}
	m_jsonInaccessible.clear();

	// 每月第一天删除对US交易所的禁止访问
	gl_tpNow = chrono::time_point_cast<chrono::seconds>(chrono::system_clock::now());
	tm tmLocal;
	const auto time = gl_tpNow.time_since_epoch().count();
	localtime_s(&tmLocal, &time);
	if (tmLocal.tm_mday == 1) {
		DeleteAllFinnhubInaccessibleUSExchange();
	}
}

CFinnhubInaccessibleExchange::~CFinnhubInaccessibleExchange() {
	if (IsUpdateDB()) {
		UpdateDB();
	}
}

void CFinnhubInaccessibleExchange::UpdateDB() {
	const string strOld = m_strFileName.substr(0, m_strFileName.length() - 4) + "json";
	const string strNew = m_strFileName.substr(0, m_strFileName.length() - 4) + "bak";

	std::filesystem::remove(gl_systemConfiguration.GetConfigurationFileDirectory() + strNew);
	rename((gl_systemConfiguration.GetConfigurationFileDirectory() + strOld).c_str(), (gl_systemConfiguration.GetConfigurationFileDirectory() + strNew).c_str()); // 保存备份
	UpdateJson();
	SaveDB();
	SetUpdateDB(false);
}

bool CFinnhubInaccessibleExchange::LoadDB() {
	string str = gl_systemConfiguration.GetConfigurationFileDirectory() + m_strFileName;
	fstream f(gl_systemConfiguration.GetConfigurationFileDirectory() + m_strFileName, ios::in);
	if (f.is_open()) {
		f >> m_jsonInaccessible;
		return true;
	}
	return false;
}

bool CFinnhubInaccessibleExchange::LoadDB(const string& strFileDirectory) {
	fstream f(strFileDirectory + m_strFileName, ios::in);
	if (f.is_open()) {
		f >> m_jsonInaccessible;
		return true;
	}
	return false;
}

void CFinnhubInaccessibleExchange::SaveDB() const {
	fstream f(gl_systemConfiguration.GetConfigurationFileDirectory() + m_strFileName, ios::out);
	f << m_jsonInaccessible;
	f.close();
}

void CFinnhubInaccessibleExchange::Update() {
	try {
		m_lUpdateDate = m_jsonInaccessible.at("UpdateDate");
	} catch (nlohmannJson::exception&) {}
	try {
		for (size_t i = 0; i < m_jsonInaccessible.at("InaccessibleExchange").size(); i++) {
			const auto size = m_jsonInaccessible.at("InaccessibleExchange").at(i).at("Exchange").size();
			if (size > 0) {
				// 有exchange数据的话才建立数据集
				const auto pInaccessible = make_shared<CInaccessible>();
				string s2 = m_jsonInaccessible["InaccessibleExchange"].at(i).at("Function"); // 从json解析出的字符串格式为std::string
				pInaccessible->SetFunctionString(s2);
				pInaccessible->SetFunction(gl_FinnhubInquiryType.GetInquiryType(pInaccessible->GetFunctionString()));
				for (size_t j = 0; j < size; j++) {
					string s = m_jsonInaccessible.at("InaccessibleExchange").at(i).at("Exchange").at(j);
					pInaccessible->AddSymbol(s);
				}
				m_mapInaccessible[gl_FinnhubInquiryType.GetInquiryType(pInaccessible->GetFunctionString())] = pInaccessible;
			}
		}
	} catch (nlohmannJson::exception&) {}
}

void CFinnhubInaccessibleExchange::UpdateJson() {
	m_jsonInaccessible.clear();

	m_jsonInaccessible["UpdateDate"] = m_lUpdateDate;
	for (const auto& val : m_mapInaccessible | std::views::values) {
		if (val->HaveSymbol()) {
			// 有exchange数据的话才建立数据集
			auto jsonExchange = nlohmannJson{ { "Function", val->GetFunctionString() } };
			jsonExchange["Exchange"] = nlohmannJson::array();
			for (size_t i = 0; i < val->Size(); i++) {
				auto s = val->GetSymbol(i);
				jsonExchange["Exchange"].push_back(s);
			}

			m_jsonInaccessible["InaccessibleExchange"].push_back(jsonExchange);
		}
	}
}
void CFinnhubInaccessibleExchange::Clear() {
	for (const auto& val : m_mapInaccessible | views::values) {
		val->Clear();
	}
}

void CFinnhubInaccessibleExchange::AddInaccessible(int iInquireType, const string& strSymbol) {
	if (!IsInaccessible(iInquireType, strSymbol)) {
		const CInaccessiblePtr pExchange = GetInaccessible(iInquireType);
		pExchange->AddSymbol(strSymbol);
	}
}

void CFinnhubInaccessibleExchange::DeleteInaccessible(int iInquireType, const string& strSymbol) {
	if (IsInaccessible(iInquireType, strSymbol)) {
		const CInaccessiblePtr pExchange = GetInaccessible(iInquireType);
		pExchange->DeleteSymbol(strSymbol);
	}
}

bool CFinnhubInaccessibleExchange::IsInaccessible(int iInquireType, const string& strSymbol) const {
	try {
		if (m_mapInaccessible.at(iInquireType)->HaveSymbol(strSymbol)) {
			return true;
		}
		return false;
	} catch (exception&) {
		return false;
	}
}
