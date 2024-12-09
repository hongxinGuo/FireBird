#include"pch.h"
module FireBird.Type.StockSection;

CStockSection::CStockSection() {
	m_fActive = false;
	m_wMarket = 0;
	m_strComment = _T("");
	m_lIndexNumber = 0;
	m_fBuildStockPtr = false;
}
