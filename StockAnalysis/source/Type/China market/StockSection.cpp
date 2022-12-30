#include"pch.h"
#include "StockSection.h"

CStockSection::CStockSection() {
	m_fActive = false;
	m_wMarket = 0;
	m_strComment = _T("");
	m_lIndexNumber = 0;
	m_fBuildStockPtr = false;
}
