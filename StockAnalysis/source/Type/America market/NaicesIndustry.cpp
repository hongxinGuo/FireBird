#include"pch.h"

#include"NaicsIndustry.h"

CNaicsIndustry::CNaicsIndustry() : CObject() {
	m_strNaics = _T(" ");
	m_strNationalIndustry = _T(" ");
	m_strSector = _T(" ");
	m_strSubSector = _T(" ");

	m_fUpdated = false;
}

void CNaicsIndustry::Append(CSetNaicsIndustry& setNaicsIndustry) {
	setNaicsIndustry.AddNew();
	setNaicsIndustry.m_Naics = m_strNaics;
	setNaicsIndustry.m_NationalIndustry = m_strNationalIndustry;
	setNaicsIndustry.m_Sector = m_strSector;
	setNaicsIndustry.m_SubSector = m_strSubSector;
	setNaicsIndustry.Update();
}

void CNaicsIndustry::Load(CSetNaicsIndustry& setNaicsIndustry) {
	m_strNaics = setNaicsIndustry.m_Naics;
	m_strNationalIndustry = setNaicsIndustry.m_NationalIndustry;
	m_strSector = setNaicsIndustry.m_Sector;
	m_strSubSector = setNaicsIndustry.m_SubSector;
}
