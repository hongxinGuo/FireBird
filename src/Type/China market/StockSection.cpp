module;

module FireBirdLib.StockSection;

CStockSection::CStockSection() {
	m_fActive = false;
	m_wMarket = 0;
	m_strComment = " ";
	m_lIndexNumber = 0;
	m_fBuildStockPtr = false;
}
