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
  void LoadData(CSetCrweberIndex& setCrweberIndex);
  void SaveData(CSetCrweberIndex& setCewwberIndex);
  void AppendData(CSetCrweberIndex& setCrweberIndex);

  bool IsTodayUpdated(void) noexcept { return m_fTodayUpdated; }
  bool IsDataChanged(void);

  void SetNewestDataTime(long lDay) noexcept { m_lDay = lDay; }
  long GetCurrentDataTime(void) noexcept { return m_lDay; }

  double GetTD1(void) noexcept { return m_dTD1; }
  double GetTD2(void) noexcept { return m_dTD2; }
  double GetTD3C(void) noexcept { return m_dTD3C; }
  double GetTD5(void) noexcept { return m_dTD5; }
  double GetTD6(void) noexcept { return m_dTD6; }
  double GetTD7(void) noexcept { return m_dTD7; }
  double GetTD8(void) noexcept { return m_dTD8; }
  double GetTD9(void) noexcept { return m_dTD9; }
  double GetTD12(void) noexcept { return m_dTD12; }
  double GetTD15(void) noexcept { return m_dTD15; }
  double GetTD19(void) noexcept { return m_dTD19; }
  double GetTD20(void) noexcept { return m_dTD20; }
  double GetTD21(void) noexcept { return m_dTD21; }
  double GetVLCC_USGSPORE(void) noexcept { return m_dVLCC_USGSPORE; }
  double GetSUEZMAX_CBSUSG(void) noexcept { return m_dSUEZMAX_CBSUSG; }
  double GetTC1(void) noexcept { return m_dTC1; }
  double GetTC2(void) noexcept { return m_dTC2; }
  double GetTC3(void) noexcept { return m_dTC3; }
  double GetTC4(void) noexcept { return m_dTC4; }
  double GetTC5(void) noexcept { return m_dTC5; }
  double GetTC14(void) noexcept { return m_dTC14; }
  double GetCPP_USGCBS(void) noexcept { return m_dCPP_USGCBS; }
  double GetVLCC_1YEAR(void) noexcept { return m_dVLCC_TC_1YEAR; }
  double GetSUEZMAX_1YEAR(void) noexcept { return m_dSUEZMAX_TC_1YEAR; }
  double GetAFRAMAX_1YEAR(void) noexcept { return m_dAFRAMAX_TC_1YEAR; }
  double GetMR_1YEAR(void) noexcept { return m_dMR_TC_1YEAR; }
  double GetHANDY_1YEAR(void) noexcept { return m_dHANDY_TC_1YEAR; }
  double GetVLCC_3YEAR(void) noexcept { return m_dVLCC_TC_3YEAR; }
  double GetSUEZMAX_3YEAR(void) noexcept { return m_dSUEZMAX_TC_3YEAR; }
  double GetAFRAMAX_3YEAR(void) { return m_dAFRAMAX_TC_3YEAR; }
  double GetMR_3YEAR(void) noexcept { return m_dMR_TC_3YEAR; }
  double GetHANDY_3YEAR(void) noexcept { return m_dHANDY_TC_3YEAR; }

public:
  long m_lID;
  long m_lDay;
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
  long m_lLastUpdateDay;
};

typedef shared_ptr<CCrweberIndex> CrweberIndexPtr;
