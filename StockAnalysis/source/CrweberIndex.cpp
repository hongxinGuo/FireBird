#include"stdafx.h"

#include"Accessory.h"
#include "CrweberIndex.h"

CCrweberIndex::CCrweberIndex() {
  m_lID = 0;
  m_lDay = 0;
  m_dTD1 = m_dTD2 = m_dTD3C = m_dTD5 = m_dTD6 = m_dTD7 = m_dTD8 = m_dTD9 = m_dTD12 = m_dTD15 = m_dTD19 = m_dTD20
    = m_dTD21 = m_dVLCC_USGSPORE = m_dSUEZMAX_CBSUSG = 0;
  m_dTC1 = m_dTC2 = m_dTC3 = m_dTC4 = m_dTC5 = m_dTC14 = m_dCPP_USGCBS = 0;
  m_dVLCC_TC_1YEAR = m_dSUEZMAX_TC_1YEAR = m_dAFRAMAX_TC_1YEAR = m_dPANAMAX_TC_1YEAR = m_dMR_TC_1YEAR = m_dHANDY_TC_1YEAR = 0;
  m_dVLCC_TC_3YEAR = m_dSUEZMAX_TC_3YEAR = m_dAFRAMAX_TC_3YEAR = m_dPANAMAX_TC_3YEAR = m_dMR_TC_3YEAR = m_dHANDY_TC_3YEAR = 0;

  m_fTodayUpdated = false;
  m_lLastUpdateDay = 0;
}

CCrweberIndex::~CCrweberIndex() {
}

void CCrweberIndex::Reset(void) {
  m_fTodayUpdated = false;
}

void CCrweberIndex::LoadData(CSetCrweberIndex& setCrweberIndex) {
  m_lDay = setCrweberIndex.m_Day;
  m_dTD1 = atof(setCrweberIndex.m_TD1);
  m_dTD2 = atof(setCrweberIndex.m_TD2);
  m_dTD3C = atof(setCrweberIndex.m_TD3C);
  m_dTD5 = atof(setCrweberIndex.m_TD5);
  m_dTD6 = atof(setCrweberIndex.m_TD6);
  m_dTD7 = atof(setCrweberIndex.m_TD7);
  m_dTD8 = atof(setCrweberIndex.m_TD8);
  m_dTD9 = atof(setCrweberIndex.m_TD9);
  m_dTD12 = atof(setCrweberIndex.m_TD12);
  m_dTD15 = atof(setCrweberIndex.m_TD15);
  m_dTD19 = atof(setCrweberIndex.m_TD19);
  m_dTD20 = atof(setCrweberIndex.m_TD20);
  m_dTD21 = atof(setCrweberIndex.m_TD21);
  m_dVLCC_USGSPORE = atof(setCrweberIndex.m_VLCC_USGSPORE);
  m_dSUEZMAX_CBSUSG = atof(setCrweberIndex.m_SUEZMAX_CBSUSG);
  m_dTC1 = atof(setCrweberIndex.m_TC1);
  m_dTC2 = atof(setCrweberIndex.m_TC2);
  m_dTC3 = atof(setCrweberIndex.m_TC3);
  m_dTC4 = atof(setCrweberIndex.m_TC4);
  m_dTC5 = atof(setCrweberIndex.m_TC5);
  m_dTC14 = atof(setCrweberIndex.m_TC14);
  m_dVLCC_TC_1YEAR = atof(setCrweberIndex.m_VLCC_TC_1YEAR);
  m_dCPP_USGCBS = atof(setCrweberIndex.m_CPP_USGCBS);
  m_dSUEZMAX_TC_1YEAR = atof(setCrweberIndex.m_SUEZMAX_TC_1YEAR);
  m_dAFRAMAX_TC_1YEAR = atof(setCrweberIndex.m_AFRAMAX_TC_1YEAR);
  m_dPANAMAX_TC_1YEAR = atof(setCrweberIndex.m_PANAMAX_TC_1YEAR);
  m_dMR_TC_1YEAR = atof(setCrweberIndex.m_MR_TC_1YEAR);
  m_dHANDY_TC_1YEAR = atof(setCrweberIndex.m_HANDY_TC_1YEAR);
  m_dVLCC_TC_3YEAR = atof(setCrweberIndex.m_VLCC_TC_3YEAR);
  m_dSUEZMAX_TC_3YEAR = atof(setCrweberIndex.m_SUEZMAX_TC_3YEAR);
  m_dAFRAMAX_TC_3YEAR = atof(setCrweberIndex.m_AFRAMAX_TC_3YEAR);
  m_dPANAMAX_TC_3YEAR = atof(setCrweberIndex.m_PANAMAX_TC_3YEAR);
  m_dMR_TC_3YEAR = atof(setCrweberIndex.m_MR_TC_3YEAR);
  m_dHANDY_TC_3YEAR = atof(setCrweberIndex.m_HANDY_TC_3YEAR);
}

void CCrweberIndex::SaveData(CSetCrweberIndex& setCrweberIndex) {
  ASSERT(setCrweberIndex.IsOpen());

  setCrweberIndex.m_Day = m_lDay;
  setCrweberIndex.m_TD1 = ConvertValueToString(m_dTD1);
  setCrweberIndex.m_TD2 = ConvertValueToString(m_dTD2);
  setCrweberIndex.m_TD3C = ConvertValueToString(m_dTD3C);
  setCrweberIndex.m_TD5 = ConvertValueToString(m_dTD5);
  setCrweberIndex.m_TD6 = ConvertValueToString(m_dTD6);
  setCrweberIndex.m_TD7 = ConvertValueToString(m_dTD7);
  setCrweberIndex.m_TD8 = ConvertValueToString(m_dTD8);
  setCrweberIndex.m_TD9 = ConvertValueToString(m_dTD9);
  setCrweberIndex.m_TD12 = ConvertValueToString(m_dTD12);
  setCrweberIndex.m_TD15 = ConvertValueToString(m_dTD15);
  setCrweberIndex.m_TD19 = ConvertValueToString(m_dTD19);
  setCrweberIndex.m_TD20 = ConvertValueToString(m_dTD20);
  setCrweberIndex.m_TD21 = ConvertValueToString(m_dTD21);
  setCrweberIndex.m_VLCC_USGSPORE = ConvertValueToString(m_dVLCC_USGSPORE * 100);
  setCrweberIndex.m_SUEZMAX_CBSUSG = ConvertValueToString(m_dSUEZMAX_CBSUSG);
  setCrweberIndex.m_TC1 = ConvertValueToString(m_dTC1);
  setCrweberIndex.m_TC2 = ConvertValueToString(m_dTC2);
  setCrweberIndex.m_TC3 = ConvertValueToString(m_dTC3);
  setCrweberIndex.m_TC4 = ConvertValueToString(m_dTC4);
  setCrweberIndex.m_TC5 = ConvertValueToString(m_dTC5);
  setCrweberIndex.m_TC14 = ConvertValueToString(m_dTC14);
  setCrweberIndex.m_CPP_USGCBS = ConvertValueToString(m_dCPP_USGCBS * 1000);

  setCrweberIndex.m_VLCC_TC_1YEAR = ConvertValueToString(m_dVLCC_TC_1YEAR);
  setCrweberIndex.m_SUEZMAX_TC_1YEAR = ConvertValueToString(m_dSUEZMAX_TC_1YEAR);
  setCrweberIndex.m_AFRAMAX_TC_1YEAR = ConvertValueToString(m_dAFRAMAX_TC_1YEAR);
  setCrweberIndex.m_PANAMAX_TC_1YEAR = ConvertValueToString(m_dPANAMAX_TC_1YEAR);
  setCrweberIndex.m_MR_TC_1YEAR = ConvertValueToString(m_dMR_TC_1YEAR);
  setCrweberIndex.m_HANDY_TC_1YEAR = ConvertValueToString(m_dHANDY_TC_1YEAR);
  setCrweberIndex.m_VLCC_TC_3YEAR = ConvertValueToString(m_dVLCC_TC_3YEAR);
  setCrweberIndex.m_SUEZMAX_TC_3YEAR = ConvertValueToString(m_dSUEZMAX_TC_3YEAR);
  setCrweberIndex.m_AFRAMAX_TC_3YEAR = ConvertValueToString(m_dAFRAMAX_TC_3YEAR);
  setCrweberIndex.m_PANAMAX_TC_3YEAR = ConvertValueToString(m_dPANAMAX_TC_3YEAR);
  setCrweberIndex.m_MR_TC_3YEAR = ConvertValueToString(m_dMR_TC_3YEAR);
  setCrweberIndex.m_HANDY_TC_3YEAR = ConvertValueToString(m_dHANDY_TC_3YEAR);
}

void CCrweberIndex::AppendData(CSetCrweberIndex& setCrweberIndex) {
  ASSERT(setCrweberIndex.IsOpen());
  setCrweberIndex.AddNew();
  SaveData(setCrweberIndex);
  setCrweberIndex.Update();
}