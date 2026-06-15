#include "pch.h"

#include"FinnhubInquiryType.h"

#include "InaccessibleSymbol.h"

#include <fstream>

CInaccessibleSymbol::CInaccessibleSymbol(const string& strFileName) {
	m_strFileName = strFileName;
	if (LoadDB()) {
		Update();
	}
	m_jsonInaccessible.clear();
}

CInaccessibleSymbol::~CInaccessibleSymbol() {
	if (IsUpdateDB()) {
		UpdateDB();
	}
}

void CInaccessibleSymbol::UpdateDB() {
	const string strOld = m_strFileName.substr(0, m_strFileName.length() - 4) + "json";
	const string strNew = m_strFileName.substr(0, m_strFileName.length() - 4) + "bak";

	std::filesystem::remove(gl_systemConfiguration.GetConfigurationFileDirectory() + strNew);
	rename((gl_systemConfiguration.GetConfigurationFileDirectory() + strOld).c_str(), (gl_systemConfiguration.GetConfigurationFileDirectory() + strNew).c_str()); // 保存备份
	UpdateJson();
	SaveDB();
	SetUpdateDB(false);
}

bool CInaccessibleSymbol::LoadDB() {
	string str = gl_systemConfiguration.GetConfigurationFileDirectory() + m_strFileName;
	fstream f(gl_systemConfiguration.GetConfigurationFileDirectory() + m_strFileName, ios::in);
	if (f.is_open()) {
		f >> m_jsonInaccessible;
		return true;
	}
	return false;
}

bool CInaccessibleSymbol::LoadDB(const string& strFileDirectory) {
	fstream f(strFileDirectory + m_strFileName, ios::in);
	if (f.is_open()) {
		f >> m_jsonInaccessible;
		return true;
	}
	return false;
}

void CInaccessibleSymbol::SaveDB() const {
	fstream f(gl_systemConfiguration.GetConfigurationFileDirectory() + m_strFileName, ios::out);
	f << m_jsonInaccessible;
	f.close();
}

void CInaccessibleSymbol::Update() {
	try {
		m_lUpdateDate = toLocalDays(m_jsonInaccessible.at("UpdateDate"));
	} catch (nlohmannJson::exception&) {}
	try {
		for (size_t i = 0; i < m_jsonInaccessible.at("InaccessibleSymbol").size(); i++) {
			const auto size = m_jsonInaccessible.at("InaccessibleSymbol").at(i).at("Symbol").size();
			if (size > 0) {
				// 有exchange数据的话才建立数据集
				const auto pInaccessible = make_shared<CInaccessible>();
				string s2 = m_jsonInaccessible["InaccessibleSymbol"].at(i).at("Function"); // 从json解析出的字符串格式为std::string
				pInaccessible->SetFunctionString(s2);
				pInaccessible->SetFunction(gl_FinnhubInquiryType.GetInquiryType(pInaccessible->GetFunctionString()));
				for (size_t j = 0; j < size; j++) {
					string s = m_jsonInaccessible.at("InaccessibleSymbol").at(i).at("Symbol").at(j);
					pInaccessible->AddSymbol(s);
				}
				m_mapInaccessible[gl_FinnhubInquiryType.GetInquiryType(pInaccessible->GetFunctionString())] = pInaccessible;
			}
		}
	} catch (nlohmannJson::exception&) {}
}

void CInaccessibleSymbol::UpdateJson() {
	m_jsonInaccessible.clear();

	m_jsonInaccessible["UpdateDate"] = toFormattedDate(m_lUpdateDate);
	for (const auto& val : m_mapInaccessible | std::views::values) {
		if (val->HaveSymbol()) {
			// 有exchange数据的话才建立数据集
			auto jsonSymbol = nlohmannJson{ { "Function", val->GetFunctionString() } };
			jsonSymbol["Symbol"] = nlohmannJson::array();
			for (size_t i = 0; i < val->Size(); i++) {
				auto s = val->GetSymbol(i);
				jsonSymbol["Symbol"].push_back(s);
			}

			m_jsonInaccessible["InaccessibleSymbol"].push_back(jsonSymbol);
		}
	}
}
void CInaccessibleSymbol::Clear() {
	for (const auto& val : m_mapInaccessible | views::values) {
		val->Clear();
	}
}

void CInaccessibleSymbol::AddInaccessible(int iInquireType, const string& strSymbol) {
	if (!IsInaccessible(iInquireType, strSymbol)) {
		const CInaccessiblePtr pInaccessible = GetInaccessible(iInquireType);
		pInaccessible->AddSymbol(strSymbol);
	}
}

void CInaccessibleSymbol::DeleteInaccessible(int iInquireType, const string& strSymbol) {
	if (IsInaccessible(iInquireType, strSymbol)) {
		const CInaccessiblePtr pInaccessible = GetInaccessible(iInquireType);
		pInaccessible->DeleteSymbol(strSymbol);
	}
}

bool CInaccessibleSymbol::IsInaccessible(int iInquireType, const string& strSymbol) const {
	try {
		if (m_mapInaccessible.at(iInquireType)->HaveSymbol(strSymbol)) {
			return true;
		}
		return false;
	} catch (exception&) {
		return false;
	}
}
