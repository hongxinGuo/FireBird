#include"pch.h"

#include"FinnhubCompanyNews.h"

CFinnhubCompanyNews::CFinnhubCompanyNews() {
	m_llDateTime = 0;
	m_iNewsID = 0;
	m_strCategory = _T("");
	m_strCompanySymbol = _T("");
	m_strHeadLine = _T("");
	m_strImage = _T("");
	m_strRelatedSymbol = _T("");
	m_strSource = _T("");
	m_strSummary = _T("");
	m_strURL = _T("");
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
	m_strCompanySymbol = setCompanyNews.m_CompanySymbol;
	m_strCategory = setCompanyNews.m_Category;
	m_llDateTime = atoll(setCompanyNews.m_DateTime);
	m_strHeadLine = setCompanyNews.m_HeadLine;
	m_iNewsID = setCompanyNews.m_NewsID;
	m_strImage = setCompanyNews.m_Image;
	m_strRelatedSymbol = setCompanyNews.m_RelatedSymbol;
	m_strSource = setCompanyNews.m_Source;
	m_strSummary = setCompanyNews.m_Summary;
	m_strURL = setCompanyNews.m_URL;
}
