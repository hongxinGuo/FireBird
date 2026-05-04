#include"pch.h"

#include"FinnhubCompanyNews.h"

#include "CharSetTransfer.h"

CFinnhubCompanyNews::CFinnhubCompanyNews() {
	Reset();
}

void CFinnhubCompanyNews::Reset() {
	m_llDateTime = 0;
	m_iNewsID = 0;
	m_strCategory.clear();
	m_strCompanySymbol.clear();
	m_strHeadLine.clear();
	m_strImage.clear();
	m_strRelatedSymbol.clear();
	m_strSource.clear();
	m_strSummary.clear();
	m_strURL.clear();
}
void CFinnhubCompanyNews::Append(CSetCompanyNews& setCompanyNews) const {
	setCompanyNews.AddNew();
	Save(setCompanyNews);
	setCompanyNews.Update();
}

void CFinnhubCompanyNews::Update(CSetCompanyNews& setCompanyNews) const {
	setCompanyNews.Update();
	Save(setCompanyNews);
	setCompanyNews.Update();
}

void CFinnhubCompanyNews::Save(CSetCompanyNews& setCompanyNews) const {
	setCompanyNews.m_CompanySymbol = m_strCompanySymbol.substr(0, 45).c_str();
	setCompanyNews.m_Category = m_strCategory.substr(0, 100).c_str();
	setCompanyNews.m_DateTime = fmt::format("{:Ld}", m_llDateTime).c_str();
	setCompanyNews.m_HeadLine = m_strHeadLine.substr(0, 1000).c_str();
	setCompanyNews.m_NewsID = m_iNewsID;
	setCompanyNews.m_Image = m_strImage.substr(0, 1000).c_str();;
	setCompanyNews.m_RelatedSymbol = m_strRelatedSymbol.substr(0, 45).c_str();
	setCompanyNews.m_Source = m_strSource.substr(0, 500).c_str();
	setCompanyNews.m_Summary = m_strSummary.substr(0, 10000).c_str();
	setCompanyNews.m_URL = m_strURL.substr(0, 1000).c_str();
}

void CFinnhubCompanyNews::Load(const CSetCompanyNews& setCompanyNews) {
	m_strCompanySymbol = T2Utf8(setCompanyNews.m_CompanySymbol);
	m_strCategory = T2Utf8(setCompanyNews.m_Category);
	m_llDateTime = _tstoll(setCompanyNews.m_DateTime);
	m_strHeadLine = T2Utf8(setCompanyNews.m_HeadLine);
	m_iNewsID = setCompanyNews.m_NewsID;
	m_strImage = T2Utf8(setCompanyNews.m_Image);
	m_strRelatedSymbol = T2Utf8(setCompanyNews.m_RelatedSymbol);
	m_strSource = T2Utf8(setCompanyNews.m_Source);
	m_strSummary = T2Utf8(setCompanyNews.m_Summary);
	m_strURL = T2Utf8(setCompanyNews.m_URL);
}
