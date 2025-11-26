#include"pch.h"

#include"SICIndustry.h"

CSICIndustry::CSICIndustry() {
	m_lCode = 0;
	m_strIndustry = " ";
	m_strSector = " ";

	m_fUpdated = false;
}

void CSICIndustry::Append(CSetSICIndustry& setSICIndustry) const {
	setSICIndustry.AddNew();
	setSICIndustry.m_Code = m_lCode;
	setSICIndustry.m_Industry = m_strIndustry.c_str();
	setSICIndustry.m_Sector = m_strSector.c_str();
	setSICIndustry.Update();
}

void CSICIndustry::Load(const CSetSICIndustry& setSICIndustry) {
	m_lCode = setSICIndustry.m_Code;
	m_strIndustry = ToUTF8(setSICIndustry.m_Industry);
	m_strSector = ToUTF8(setSICIndustry.m_Sector);
}
