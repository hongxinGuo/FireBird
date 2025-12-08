#include"pch.h"

#include"TiingoIndustry.h"

#include "CharSetTransfer.h"

CTiingoIndustry::CTiingoIndustry() {
	m_strIndustry = " ";
	m_strSector = " ";

	m_fUpdated = false;
}

void CTiingoIndustry::Append(CSetTiingoIndustry& setTiingoIndustry) const {
	setTiingoIndustry.AddNew();
	setTiingoIndustry.m_Industry = m_strIndustry.c_str();
	setTiingoIndustry.m_Sector = m_strSector.c_str();
	setTiingoIndustry.Update();
}

void CTiingoIndustry::Load(const CSetTiingoIndustry& setTiingoIndustry) {
	m_strIndustry = T2Utf8(setTiingoIndustry.m_Industry);
	m_strSector = T2Utf8(setTiingoIndustry.m_Sector);
}
