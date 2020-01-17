#pragma once

#include"stdafx.h"

#include"SetPotenDailyBriefing.h"
#include"WebDataReceived.h"

using namespace std;
#include<vector>

class CPotenDailyBriefing final {
public:
  CPotenDailyBriefing();
  ~CPotenDailyBriefing();
  void Reset(void);
  void LoadData(CSetPotenDailyBriefing& setPotenDailyBriefing);
  void SaveData(CSetPotenDailyBriefing& setPotenDailyBriefing);
  void AppendData(CSetPotenDailyBriefing& setPotenDailyBriefing);

  bool ReadData(CWebDataReceivedPtr pWebDataReceived);

  bool IsTodayUpdated(void) noexcept { return m_fTodayUpdated; }
  bool IsDataChanged(void);

  void SetNewestDataTime(long lDay) noexcept { m_lDay = lDay; }
  long GetCurrentDataTime(void) noexcept { return m_lDay; }

  double GetTD3C(void) noexcept { return m_dTD3C; }
  double GetTD9(void) noexcept { return m_dTD9; }
  double GetTD20(void) noexcept { return m_dTD20; }
  double GetTD21(void) noexcept { return m_dTD21; }
  double GetTC1(void) noexcept { return m_dTC1; }
  double GetTC2(void) noexcept { return m_dTC2; }
  double GetTC5(void) noexcept { return m_dTC5; }
  double GetTC14(void) noexcept { return m_dTC14; }
  double GetVLCC_1YEAR(void) noexcept { return m_dVLCC_TC_1YEAR; }
  double GetSUEZMAX_1YEAR(void) noexcept { return m_dSUEZMAX_TC_1YEAR; }
  double GetAFRAMAX_1YEAR(void) noexcept { return m_dAFRAMAX_TC_1YEAR; }
  double GetMR_1YEAR(void) noexcept { return m_dMR_TC_1YEAR; }
  double GetHANDY_1YEAR(void) noexcept { return m_dHANDY_TC_1YEAR; }

public:
  double ConvertStringToTC(CString str);
  long ConvertStringToTime(CString str);
  double GetOneValue(CWebDataReceivedPtr pWebDataReceived);
  CString GetNextString(CWebDataReceivedPtr pWebDataReceived);

public:
  long m_lID;
  long m_lDay;
  double m_dTD3C;
  double m_dTD9;
  double m_dTD20;
  double m_dTD21;
  double m_dTC1;
  double m_dTC2;
  double m_dTC5;
  double m_dTC14;
  double m_dVLCC_TC_1YEAR;
  double m_dSUEZMAX_TC_1YEAR;
  double m_dAFRAMAX_TC_1YEAR;
  double m_dLR2_TC_1YEAR;
  double m_dLR1_TC_1YEAR;
  double m_dMR_TC_1YEAR;
  double m_dHANDY_TC_1YEAR;

  double m_dVLCC_NewBuild;
  double m_dSUEZMAX_NewBuild;
  double m_dAFRAMAX_NewBuild;
  double m_dLR2_NewBuild;
  double m_dLR1_NewBuild;
  double m_dMR_NewBuild;
  double m_dHANDYMAX_NewBuild;
  double m_dVLCC_5YearsOld;
  double m_dSUEZMAX_5YearsOld;
  double m_dAFRAMAX_5YearsOld;
  double m_dLR2_5YearsOld;
  double m_dLR1_5YearsOld;
  double m_dMR_5YearsOld;
  double m_dHANDYMAX_5YearsOld;

  bool m_fTodayUpdated;
  long m_lLastUpdateDay;

protected:
};

typedef CPotenDailyBriefing* PotenDailyBriefingPtr;
