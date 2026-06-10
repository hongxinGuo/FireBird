#include"pch.h"

#include"FinnhubCompanyNews.h"

CFinnhubCompanyNews::CFinnhubCompanyNews() {
	Reset();
}

void CFinnhubCompanyNews::Reset() {
	m_DateTime = toSysTime(0);
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
