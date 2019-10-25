#pragma once

#include"stdafx.h"

#include"SetCrweberIndex.h"

using namespace std;
#include<vector>
#include<memory>

class CCrweberIndex final {
public:
  CCrweberIndex();
  ~CCrweberIndex();
  void Reset(void);
  void SetData(CSetCrweberIndex* pSetCrweberIndex);
  void SaveData(CSetCrweberIndex* pSetCewwberIndex);

  bool IsTodayUpdated(long lDay) { if (lDay == m_lDay) { m_fTodayUpdated = true; return true; } else { return false; } }
  bool IsTodayUpdated(void) { return m_fTodayUpdated; }

  void SetNewestDataTime(long lDay) { m_lDay = lDay; }
  long GetCurrentDataTime(void) { return m_lDay; }

  long GetTD1(void) { return m_lTD1; }
  long GetTD2(void) { return m_lTD2; }
  long GetTD3C(void) { return m_lTD3C; }
  long GetTD5(void) { return m_lTD5; }
  long GetTD6(void) { return m_lTD6; }
  long GetTD7(void) { return m_lTD7; }
  long GetTD8(void) { return m_lTD8; }
  long GetTD9(void) { return m_lTD9; }
  long GetTD12(void) { return m_lTD12; }
  long GetTD15(void) { return m_lTD15; }
  long GetTD19(void) { return m_lTD19; }
  long GetTD20(void) { return m_lTD20; }
  long GetTD21(void) { return m_lTD21; }
  long GetVLCC_USGSPORT(void) { return m_lVLCC_USGSPORE; }
  long GetSUEZMAX_CBSUSG(void) { return m_lSUEZMAX_CBSUSG; }
  long GetTC1(void) { return m_lTC1; }
  long GetTC2(void) { return m_lTC2; }
  long GetTC3(void) { return m_lTC3; }
  long GetTC4(void) { return m_lTC4; }
  long GetTC5(void) { return m_lTC5; }
  long GetTC14(void) { return m_lTC14; }
  long GetCPP_USGCBS(void) { return m_lCPP_USGCBS; }

public:
  long m_lID;
  long m_lDay;
  long m_lTD1;
  long m_lTD2;
  long m_lTD3C;
  long m_lTD5;
  long m_lTD6;
  long m_lTD7;
  long m_lTD8;
  long m_lTD9;
  long m_lTD12;
  long m_lTD15;
  long m_lTD19;
  long m_lTD20;
  long m_lTD21;
  long m_lVLCC_USGSPORE;
  long m_lSUEZMAX_CBSUSG;
  long m_lTC1;
  long m_lTC2;
  long m_lTC3;
  long m_lTC4;
  long m_lTC5;
  long m_lTC14;
  long m_lCPP_USGCBS;
  long m_lVLCC_TC_1YEAR;
  long m_lSUEZMAX_TC_1YEAR;
  long m_lAFRAMAX_TC_1YEAR;
  long m_lPANAMAX_TC_1YEAR;
  long m_lMR_TC_1YEAR;
  long m_lHANDY_TC_1YEAR;
  long m_lVLCC_TC_3YEAR;
  long m_lSUEZMAX_TC_3YEAR;
  long m_lAFRAMAX_TC_3YEAR;
  long m_lPANAMAX_TC_3YEAR;
  long m_lMR_TC_3YEAR;
  long m_lHANDY_TC_3YEAR;

  bool m_fTodayUpdated;
};

typedef shared_ptr<CCrweberIndex> CrweberIndexPtr;
