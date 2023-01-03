#include"pch.h"

#include"SICIndustry.h"

CSICIndustry::CSICIndustry() {
	m_lCode = 0;
	m_strIndustry = _T(" ");
	m_strSector = _T(" ");

	m_fUpdated = false;
}

void CSICIndustry::Append(CSetSICIndustry& setSICIndustry) {
	setSICIndustry.AddNew();
	setSICIndustry.m_Code = m_lCode;
	setSICIndustry.m_Industry = m_strIndustry;
	setSICIndustry.m_Sector = m_strSector;
	setSICIndustry.Update();
}

void CSICIndustry::Load(CSetSICIndustry& setSICIndustry) {
	m_lCode = setSICIndustry.m_Code;
	m_strIndustry = setSICIndustry.m_Industry;
	m_strSector = setSICIndustry.m_Sector;
}
