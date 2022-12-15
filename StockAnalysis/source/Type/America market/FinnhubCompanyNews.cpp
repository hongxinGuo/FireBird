#include"pch.h"

#include"FinnhubCompanyNews.h"

CFinnhubCompanyNews::CFinnhubCompanyNews() : CObject() {
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

void CFinnhubCompanyNews::Append(CSetCompanyNews& setCompanyNews) {
	setCompanyNews.AddNew();
	Save(setCompanyNews);
	setCompanyNews.Update();
}

void CFinnhubCompanyNews::Update(CSetCompanyNews& setCompanyNews) {
	setCompanyNews.Update();
	Save(setCompanyNews);
	setCompanyNews.Update();
}

void CFinnhubCompanyNews::Save(CSetCompanyNews& setCompanyNews) {
	char buffer[30]{0};
	setCompanyNews.m_CompanySymbol = m_strCompanySymbol.Left(45);
	setCompanyNews.m_Category = m_strCategory.Left(100);
	sprintf_s(buffer, _T("%lld"), m_llDateTime);
	setCompanyNews.m_DateTime = buffer;
	setCompanyNews.m_HeadLine = m_strHeadLine.Left(1000);
	setCompanyNews.m_NewsID = m_iNewsID;
	setCompanyNews.m_Image = m_strImage.Left(1000);
	setCompanyNews.m_RelatedSymbol = m_strRelatedSymbol.Left(45);
	setCompanyNews.m_Source = m_strSource.Left(500);
	setCompanyNews.m_Summary = m_strSummary.Left(10000);
	setCompanyNews.m_URL = m_strURL.Left(1000);
}

void CFinnhubCompanyNews::Load(CSetCompanyNews& setCompanyNews) {
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
