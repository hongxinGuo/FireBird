#include"pch.h"

#include"TiingoIndustry.h"

CTiingoIndustry::CTiingoIndustry() {
	m_strIndustry = _T(" ");
	m_strSector = _T(" ");

	m_fUpdated = false;
}

void CTiingoIndustry::Append(CSetTiingoIndustry& setTiingoIndustry) const {
	setTiingoIndustry.AddNew();
	setTiingoIndustry.m_Industry = m_strIndustry.c_str();
	setTiingoIndustry.m_Sector = m_strSector.c_str();
	setTiingoIndustry.Update();
}

void CTiingoIndustry::Load(const CSetTiingoIndustry& setTiingoIndustry) {
	m_strIndustry = setTiingoIndustry.m_Industry;
	m_strSector = setTiingoIndustry.m_Sector;
}
