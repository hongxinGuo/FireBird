#include"pch.h"
#include"TiingoIndustry.h"

CTiingoIndustry::CTiingoIndustry() : CObject() {
  m_strIndustry = _T(" ");
  m_strSector = _T(" ");

  m_fUpdated = false;
}

void CTiingoIndustry::Append(CSetTiingoIndustry& setTiingoIndustry) {
  setTiingoIndustry.AddNew();
  setTiingoIndustry.m_Industry = m_strIndustry;
  setTiingoIndustry.m_Sector = m_strSector;
  setTiingoIndustry.Update();
}

void CTiingoIndustry::Load(CSetTiingoIndustry& setTiingoIndustry) {
  m_strIndustry = setTiingoIndustry.m_Industry;
  m_strSector = setTiingoIndustry.m_Sector;
}