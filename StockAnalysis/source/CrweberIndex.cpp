#include"stdafx.h"

#include "CrweberIndex.h"

CCrweberIndex::CCrweberIndex() {
  m_lID = 0;
  m_lDay = 0;
  m_lTD1 = m_lTD2 = m_lTD3C = m_lTD5 = m_lTD6 = m_lTD7 = m_lTD8 = m_lTD9 = m_lTD12 = m_lTD15 = m_lTD19 = m_lTD20
    = m_lTD21 = m_lVLCC_USGSPORE = m_lSUEZMAX_CBSUSG = 0;
  m_lTC1 = m_lTC2 = m_lTC3 = m_lTC4 = m_lTC5 = m_lTC14 = m_lCPP_USGCBS = 0;

  m_fTodayUpdated = false;
}

CCrweberIndex::~CCrweberIndex() {
}

void CCrweberIndex::Reset(void) {
  m_fTodayUpdated = false;
}

void CCrweberIndex::SetData(CSetCrweberIndex* pSetCrweberIndex) {
  m_lDay = pSetCrweberIndex->m_Day;
  m_lTD1 = pSetCrweberIndex->m_TD1;
  m_lTD2 = pSetCrweberIndex->m_TD2;
  m_lTD3C = pSetCrweberIndex->m_TD3C;
  m_lTD5 = pSetCrweberIndex->m_TD5;
  m_lTD6 = pSetCrweberIndex->m_TD6;
  m_lTD7 = pSetCrweberIndex->m_TD7;
  m_lTD8 = pSetCrweberIndex->m_TD8;
  m_lTD9 = pSetCrweberIndex->m_TD9;
  m_lTD12 = pSetCrweberIndex->m_TD12;
  m_lTD15 = pSetCrweberIndex->m_TD15;
  m_lTD19 = pSetCrweberIndex->m_TD19;
  m_lTD20 = pSetCrweberIndex->m_TD20;
  m_lTD21 = pSetCrweberIndex->m_TD21;
  m_lVLCC_USGSPORE = pSetCrweberIndex->m_VLCC_USGSPORE;
  m_lSUEZMAX_CBSUSG = pSetCrweberIndex->m_SUEZMAX_CBSUSG;
  m_lTC1 = pSetCrweberIndex->m_TC1;
  m_lTC2 = pSetCrweberIndex->m_TC2;
  m_lTC3 = pSetCrweberIndex->m_TC3;
  m_lTC4 = pSetCrweberIndex->m_TC4;
  m_lTC5 = pSetCrweberIndex->m_TC5;
  m_lTC14 = pSetCrweberIndex->m_TC14;
  m_lCPP_USGCBS = pSetCrweberIndex->m_CPP_USGCBS;
}

void CCrweberIndex::SaveData(CSetCrweberIndex* pSetCrweberIndex) {
  pSetCrweberIndex->m_Day = m_lDay;
  pSetCrweberIndex->m_TD1 = m_lTD1;
  pSetCrweberIndex->m_TD2 = m_lTD2;
  pSetCrweberIndex->m_TD3C = m_lTD3C;
  pSetCrweberIndex->m_TD5 = m_lTD5;
  pSetCrweberIndex->m_TD6 = m_lTD6;
  pSetCrweberIndex->m_TD7 = m_lTD7;
  pSetCrweberIndex->m_TD8 = m_lTD8;
  pSetCrweberIndex->m_TD9 = m_lTD9;
  pSetCrweberIndex->m_TD12 = m_lTD12;
  pSetCrweberIndex->m_TD15 = m_lTD15;
  pSetCrweberIndex->m_TD19 = m_lTD19;
  pSetCrweberIndex->m_TD20 = m_lTD20;
  pSetCrweberIndex->m_TD21 = m_lTD21;
  pSetCrweberIndex->m_VLCC_USGSPORE = m_lVLCC_USGSPORE;
  pSetCrweberIndex->m_SUEZMAX_CBSUSG = m_lSUEZMAX_CBSUSG;
  pSetCrweberIndex->m_TC1 = m_lTC1;
  pSetCrweberIndex->m_TC2 = m_lTC2;
  pSetCrweberIndex->m_TC3 = m_lTC3;
  pSetCrweberIndex->m_TC4 = m_lTC4;
  pSetCrweberIndex->m_TC5 = m_lTC5;
  pSetCrweberIndex->m_TC14 = m_lTC14;
  pSetCrweberIndex->m_CPP_USGCBS = m_lCPP_USGCBS;
}