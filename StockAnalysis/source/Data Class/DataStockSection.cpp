#include "pch.h"

#include"globedef.h"
#include "DataStockSection.h"

CDataStockSection::CDataStockSection() {
	Reset();
}

CDataStockSection::~CDataStockSection()
{
}

void CDataStockSection::Reset(void) {
	m_vStockSection.resize(2000); // »¦Éî¸÷1000¸ö¶Î¡£
	CStockSectionPtr pStockSection;
	for (int i = 0; i < 2000; i++) {
		pStockSection = make_shared<CStockSection>();
		pStockSection->SetIndexNumber(i);
		if (i < 1000) pStockSection->SetMarket(__SHANGHAI_MARKET__);
		else pStockSection->SetMarket(__SHENZHEN_MARKET__);
		m_vStockSection.at(i) = pStockSection;
	}
}

void CDataStockSection::LoadDB(void)
{
}

void CDataStockSection::UpdateDB(void)
{
}