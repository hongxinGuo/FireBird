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

  double GetTD1(void) { return m_dTD1; }
  double GetTD2(void) { return m_dTD2; }
  double GetTD3C(void) { return m_dTD3C; }
  double GetTD5(void) { return m_dTD5; }
  double GetTD6(void) { return m_dTD6; }
  double GetTD7(void) { return m_dTD7; }
  double GetTD8(void) { return m_dTD8; }
  double GetTD9(void) { return m_dTD9; }
  double GetTD12(void) { return m_dTD12; }
  double GetTD15(void) { return m_dTD15; }
  double GetTD19(void) { return m_dTD19; }
  double GetTD20(void) { return m_dTD20; }
  double GetTD21(void) { return m_dTD21; }
  double GetVLCC_USGSPORT(void) { return m_dVLCC_USGSPORE; }
  double GetSUEZMAX_CBSUSG(void) { return m_dSUEZMAX_CBSUSG; }
  double GetTC1(void) { return m_dTC1; }
  double GetTC2(void) { return m_dTC2; }
  double GetTC3(void) { return m_dTC3; }
  double GetTC4(void) { return m_dTC4; }
  double GetTC5(void) { return m_dTC5; }
  double GetTC14(void) { return m_dTC14; }
  double GetCPP_USGCBS(void) { return m_dCPP_USGCBS; }

public:
  double m_lID;
  double m_lDay;
  double m_dTD1;
  double m_dTD2;
  double m_dTD3C;
  double m_dTD5;
  double m_dTD6;
  double m_dTD7;
  double m_dTD8;
  double m_dTD9;
  double m_dTD12;
  double m_dTD15;
  double m_dTD19;
  double m_dTD20;
  double m_dTD21;
  double m_dVLCC_USGSPORE;
  double m_dSUEZMAX_CBSUSG;
  double m_dTC1;
  double m_dTC2;
  double m_dTC3;
  double m_dTC4;
  double m_dTC5;
  double m_dTC14;
  double m_dCPP_USGCBS;
  double m_dVLCC_TC_1YEAR;
  double m_dSUEZMAX_TC_1YEAR;
  double m_dAFRAMAX_TC_1YEAR;
  double m_dPANAMAX_TC_1YEAR;
  double m_dMR_TC_1YEAR;
  double m_dHANDY_TC_1YEAR;
  double m_dVLCC_TC_3YEAR;
  double m_dSUEZMAX_TC_3YEAR;
  double m_dAFRAMAX_TC_3YEAR;
  double m_dPANAMAX_TC_3YEAR;
  double m_dMR_TC_3YEAR;
  double m_dHANDY_TC_3YEAR;

  bool m_fTodayUpdated;
};

typedef shared_ptr<CCrweberIndex> CrweberIndexPtr;
