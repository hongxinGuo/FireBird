#include"stdafx.h"

#include "CrweberIndex.h"

#include"Accessory.h"

CCrweberIndex::CCrweberIndex() {
  m_lID = 0;
  m_lDay = 0;
  m_dTD1 = m_dTD2 = m_dTD3C = m_dTD5 = m_dTD6 = m_dTD7 = m_dTD8 = m_dTD9 = m_dTD12 = m_dTD15 = m_dTD19 = m_dTD20
    = m_dTD21 = m_dVLCC_USGSPORE = m_dSUEZMAX_CBSUSG = 0;
  m_dTC1 = m_dTC2 = m_dTC3 = m_dTC4 = m_dTC5 = m_dTC14 = m_dCPP_USGCBS = 0;
  m_dVLCC_TC_1YEAR = m_dSUEZMAX_TC_1YEAR = m_dAFRAMAX_TC_1YEAR = m_dPANAMAX_TC_1YEAR = m_dMR_TC_1YEAR = m_dHANDY_TC_1YEAR = 0;
  m_dVLCC_TC_3YEAR = m_dSUEZMAX_TC_3YEAR = m_dAFRAMAX_TC_3YEAR = m_dPANAMAX_TC_3YEAR = m_dMR_TC_3YEAR = m_dHANDY_TC_3YEAR = 0;

  m_fTodayUpdated = false;
}

CCrweberIndex::~CCrweberIndex() {
}

void CCrweberIndex::Reset(void) {
  m_fTodayUpdated = false;
}

void CCrweberIndex::SetData(CSetCrweberIndex* pSetCrweberIndex) {
  m_lDay = pSetCrweberIndex->m_Day;
  m_dTD1 = atof(pSetCrweberIndex->m_TD1);
  m_dTD2 = atof(pSetCrweberIndex->m_TD2);
  m_dTD3C = atof(pSetCrweberIndex->m_TD3C);
  m_dTD5 = atof(pSetCrweberIndex->m_TD5);
  m_dTD6 = atof(pSetCrweberIndex->m_TD6);
  m_dTD7 = atof(pSetCrweberIndex->m_TD7);
  m_dTD8 = atof(pSetCrweberIndex->m_TD8);
  m_dTD9 = atof(pSetCrweberIndex->m_TD9);
  m_dTD12 = atof(pSetCrweberIndex->m_TD12);
  m_dTD15 = atof(pSetCrweberIndex->m_TD15);
  m_dTD19 = atof(pSetCrweberIndex->m_TD19);
  m_dTD20 = atof(pSetCrweberIndex->m_TD20);
  m_dTD21 = atof(pSetCrweberIndex->m_TD21);
  m_dVLCC_USGSPORE = atof(pSetCrweberIndex->m_VLCC_USGSPORE);
  m_dSUEZMAX_CBSUSG = atof(pSetCrweberIndex->m_SUEZMAX_CBSUSG);
  m_dTC1 = atof(pSetCrweberIndex->m_TC1);
  m_dTC2 = atof(pSetCrweberIndex->m_TC2);
  m_dTC3 = atof(pSetCrweberIndex->m_TC3);
  m_dTC4 = atof(pSetCrweberIndex->m_TC4);
  m_dTC5 = atof(pSetCrweberIndex->m_TC5);
  m_dTC14 = atof(pSetCrweberIndex->m_TC14);
  m_dCPP_USGCBS = atof(pSetCrweberIndex->m_CPP_USGCBS);
  m_dVLCC_TC_1YEAR == atof(pSetCrweberIndex->m_VLCC_TC_1YEAR);
  m_dSUEZMAX_TC_1YEAR == atof(pSetCrweberIndex->m_SUEZMAX_TC_1YEAR);
  m_dAFRAMAX_TC_1YEAR == atof(pSetCrweberIndex->m_AFRAMAX_TC_1YEAR);
  m_dPANAMAX_TC_1YEAR == atof(pSetCrweberIndex->m_PANAMAX_TC_1YEAR);
  m_dMR_TC_1YEAR == atof(pSetCrweberIndex->m_MR_TC_1YEAR);
  m_dHANDY_TC_1YEAR == atof(pSetCrweberIndex->m_HANDY_TC_1YEAR);
  m_dVLCC_TC_3YEAR == atof(pSetCrweberIndex->m_VLCC_TC_3YEAR);
  m_dSUEZMAX_TC_3YEAR == atof(pSetCrweberIndex->m_SUEZMAX_TC_3YEAR);
  m_dAFRAMAX_TC_3YEAR == atof(pSetCrweberIndex->m_AFRAMAX_TC_3YEAR);
  m_dPANAMAX_TC_3YEAR == atof(pSetCrweberIndex->m_PANAMAX_TC_3YEAR);
  m_dMR_TC_3YEAR == atof(pSetCrweberIndex->m_MR_TC_3YEAR);
  m_dHANDY_TC_3YEAR == atof(pSetCrweberIndex->m_HANDY_TC_3YEAR);
}

void CCrweberIndex::SaveData(CSetCrweberIndex* pSetCrweberIndex) {
  pSetCrweberIndex->m_Day = m_lDay;
  pSetCrweberIndex->m_TD1 = ConvertValueToString(m_dTD1);
  pSetCrweberIndex->m_TD2 = ConvertValueToString(m_dTD2);
  pSetCrweberIndex->m_TD3C = ConvertValueToString(m_dTD3C);
  pSetCrweberIndex->m_TD5 = ConvertValueToString(m_dTD5);
  pSetCrweberIndex->m_TD6 = ConvertValueToString(m_dTD6);
  pSetCrweberIndex->m_TD7 = ConvertValueToString(m_dTD7);
  pSetCrweberIndex->m_TD8 = ConvertValueToString(m_dTD8);
  pSetCrweberIndex->m_TD9 = ConvertValueToString(m_dTD9);
  pSetCrweberIndex->m_TD12 = ConvertValueToString(m_dTD12);
  pSetCrweberIndex->m_TD15 = ConvertValueToString(m_dTD15);
  pSetCrweberIndex->m_TD19 = ConvertValueToString(m_dTD19);
  pSetCrweberIndex->m_TD20 = ConvertValueToString(m_dTD20);
  pSetCrweberIndex->m_TD21 = ConvertValueToString(m_dTD21);
  pSetCrweberIndex->m_VLCC_USGSPORE = ConvertValueToString(m_dVLCC_USGSPORE * 100);
  pSetCrweberIndex->m_SUEZMAX_CBSUSG = ConvertValueToString(m_dSUEZMAX_CBSUSG);
  pSetCrweberIndex->m_TC1 = ConvertValueToString(m_dTC1);
  pSetCrweberIndex->m_TC2 = ConvertValueToString(m_dTC2);
  pSetCrweberIndex->m_TC3 = ConvertValueToString(m_dTC3);
  pSetCrweberIndex->m_TC4 = ConvertValueToString(m_dTC4);
  pSetCrweberIndex->m_TC5 = ConvertValueToString(m_dTC5);
  pSetCrweberIndex->m_TC14 = ConvertValueToString(m_dTC14);
  pSetCrweberIndex->m_CPP_USGCBS = ConvertValueToString(m_dCPP_USGCBS * 1000);

  pSetCrweberIndex->m_VLCC_TC_1YEAR = ConvertValueToString(m_dVLCC_TC_1YEAR);
  pSetCrweberIndex->m_SUEZMAX_TC_1YEAR = ConvertValueToString(m_dSUEZMAX_TC_1YEAR);
  pSetCrweberIndex->m_AFRAMAX_TC_1YEAR = ConvertValueToString(m_dAFRAMAX_TC_1YEAR);
  pSetCrweberIndex->m_PANAMAX_TC_1YEAR = ConvertValueToString(m_dPANAMAX_TC_1YEAR);
  pSetCrweberIndex->m_MR_TC_1YEAR = ConvertValueToString(m_dMR_TC_1YEAR);
  pSetCrweberIndex->m_HANDY_TC_1YEAR = ConvertValueToString(m_dHANDY_TC_1YEAR);
  pSetCrweberIndex->m_VLCC_TC_3YEAR = ConvertValueToString(m_dVLCC_TC_3YEAR);
  pSetCrweberIndex->m_SUEZMAX_TC_3YEAR = ConvertValueToString(m_dSUEZMAX_TC_3YEAR);
  pSetCrweberIndex->m_AFRAMAX_TC_3YEAR = ConvertValueToString(m_dAFRAMAX_TC_3YEAR);
  pSetCrweberIndex->m_PANAMAX_TC_3YEAR = ConvertValueToString(m_dPANAMAX_TC_3YEAR);
  pSetCrweberIndex->m_MR_TC_3YEAR = ConvertValueToString(m_dMR_TC_3YEAR);
  pSetCrweberIndex->m_HANDY_TC_3YEAR = ConvertValueToString(m_dHANDY_TC_3YEAR);
}