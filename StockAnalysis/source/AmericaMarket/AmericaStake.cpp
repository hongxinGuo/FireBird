#include"globedef.h"
#include"accessory.h"
#include "AmericaStake.h"

#include"SetAmericaStakeDayLine.h"

CAmericaStake::CAmericaStake() : CObject() {
  Reset();
}

void CAmericaStake::Reset(void) {
  m_strIPODate = _T(" ");
  m_strCurrency = _T(" ");
  m_strType = _T(" ");
  m_strCountry = _T(" ");
  m_strDescription = _T(" ");
  m_strDisplaySymbol = _T(" ");
  m_strExchange = _T(" ");
  m_strFinnhubIndustry = _T(" ");
  m_strLogo = _T(" ");
  m_strName = _T(" ");
  m_strPhone = _T(" ");
  m_strSymbol = _T(" ");
  m_strTicker = _T(" ");
  m_strWebURL = _T(" ");
  m_strAddress = _T(" ");
  m_strCity = _T(" ");
  m_strCusip = _T(" ");
  m_strSedol = _T(" ");
  m_strGgroup = _T(" ");
  m_strGind = _T(" ");
  m_strGsector = _T(" ");
  m_strGsubind = _T(" ");
  m_strNaics = _T(" ");
  m_strNaicsNationalIndustry = _T(" ");
  m_strNaicsSector = _T(" ");
  m_strNaicsSubsector = _T(" ");
  m_strState = _T(" ");
  m_lProfileUpdateDate = 19800101;
  m_lDayLineStartDate = 29900101;
  m_lDayLineEndDate = 19800101;
  m_lLastRTDataUpdateDate = 19800101;
  m_lIPOStatus = __STAKE_NOT_CHECKED__;

  m_lEmployeeTotal = 0;
  m_dMarketCapitalization = 0;
  m_dShareOutstanding = 0;

  m_fDayLineDBUpdated = false;
  m_fUpdateDatabase = false;
  m_fInquiryAmericaStake = true;
  m_fDayLineNeedUpdate = true;
  m_fDayLineNeedSaving = false;

  m_TransactionTime = 0;
  m_lHigh = 0;
  m_lLow = 0;
  m_lLastClose = 0;
  m_lNew = 0;
  m_lOpen = 0;

  m_vDayLine.resize(0);
}

void CAmericaStake::Load(CSetAmericaStake& setAmericaStake) {
  m_strSymbol = setAmericaStake.m_Symbol;
  m_strDescription = setAmericaStake.m_Description;
  m_strDisplaySymbol = setAmericaStake.m_DisplaySymbol;
  m_strType = setAmericaStake.m_Type;
  m_strCurrency = setAmericaStake.m_Currency;
  m_strAddress = setAmericaStake.m_Address;
  m_strCity = setAmericaStake.m_City;
  m_strCountry = setAmericaStake.m_Country;
  m_strCusip = setAmericaStake.m_Cusip;
  m_strSedol = setAmericaStake.m_Sedol;
  m_lEmployeeTotal = setAmericaStake.m_EmployeeTotal;
  m_strExchange = setAmericaStake.m_Exchange;
  m_strGgroup = setAmericaStake.m_Ggroup;
  m_strGind = setAmericaStake.m_Gind;
  m_strGsector = setAmericaStake.m_Gsector;
  m_strGsubind = setAmericaStake.m_Gsubind;
  m_strIPODate = setAmericaStake.m_IPODate;
  m_strIsin = setAmericaStake.m_Isin;
  m_dMarketCapitalization = atof(setAmericaStake.m_MarketCapitalization);
  m_strNaics = setAmericaStake.m_Naics;
  m_strNaicsNationalIndustry = setAmericaStake.m_NaicsNationalIndustry;
  m_strNaicsSector = setAmericaStake.m_NaicsSector;
  m_strNaicsSubsector = setAmericaStake.m_NaicsSubsector;
  m_strName = setAmericaStake.m_Name;
  m_strPhone = setAmericaStake.m_Phone;
  m_dShareOutstanding = atof(setAmericaStake.m_ShareOutstanding);
  m_strState = setAmericaStake.m_State;
  m_strTicker = setAmericaStake.m_Ticker;
  m_strWebURL = setAmericaStake.m_WebURL;
  m_strLogo = setAmericaStake.m_Logo;
  m_strFinnhubIndustry = setAmericaStake.m_FinnhubIndustry;
  m_lProfileUpdateDate = setAmericaStake.m_ProfileUpdateDate;
  m_lDayLineStartDate = setAmericaStake.m_DayLineStartDate;
  m_lDayLineEndDate = setAmericaStake.m_DayLineEndDate;
  m_lLastRTDataUpdateDate = setAmericaStake.m_LastRTDataUpdateDate;
  m_lIPOStatus = setAmericaStake.m_IPOStatus;
  if ((m_strType.GetLength() < 2) || (m_strCurrency.GetLength() < 2)) {
    //m_fInquiryAmericaStake = false;
    //m_lProfileUpdateDate = gl_pAmericaStakeMarket->GetFormatedMarketDate();
  }
  SetCheckingDayLineStatus();
}

void CAmericaStake::SetCheckingDayLineStatus(void) {
  ASSERT(IsDayLineNeedUpdate()); // 默认状态为日线数据需要更新
  if (m_lIPOStatus == __STAKE_NULL__) {
    SetDayLineNeedUpdate(false);
  }
  // 不再更新日线数据比上个交易日要新的股票。其他所有的股票都查询一遍，以防止出现新股票或者老的股票重新活跃起来。
  else if (gl_pAmericaStakeMarket->GetLastTradeDate() <= GetDayLineEndDate()) { // 最新日线数据为今日或者上一个交易日的数据。
    SetDayLineNeedUpdate(false); // 日线数据不需要更新
  }
}

bool CAmericaStake::CheckDayLineUpdateStatus() {
  ASSERT(IsDayLineNeedUpdate()); // 默认状态为日线数据需要更新
  // 不再更新日线数据比上个交易日要新的股票。其他所有的股票都查询一遍，以防止出现新股票或者老的股票重新活跃起来。
  if (gl_pAmericaStakeMarket->GetLastTradeDate() <= GetDayLineEndDate()) { // 最新日线数据为今日或者上一个交易日的数据。
    SetDayLineNeedUpdate(false); // 日线数据不需要更新
  }
  return true;
}

void CAmericaStake::Save(CSetAmericaStake& setAmericaStake) {
  setAmericaStake.m_Symbol = m_strSymbol;
  setAmericaStake.m_Description = m_strDescription;
  setAmericaStake.m_DisplaySymbol = m_strDisplaySymbol;
  setAmericaStake.m_Type = m_strType;
  setAmericaStake.m_Currency = m_strCurrency;
  setAmericaStake.m_Address = m_strAddress;
  setAmericaStake.m_City = m_strCity;
  setAmericaStake.m_Country = m_strCountry;
  setAmericaStake.m_Cusip = m_strCusip;
  setAmericaStake.m_Sedol = m_strSedol;
  setAmericaStake.m_EmployeeTotal = m_lEmployeeTotal;
  setAmericaStake.m_Exchange = m_strExchange;
  setAmericaStake.m_Ggroup = m_strGgroup;
  setAmericaStake.m_Gind = m_strGind;
  setAmericaStake.m_Gsector = m_strGsector;
  setAmericaStake.m_Gsubind = m_strGsubind;
  setAmericaStake.m_IPODate = m_strIPODate;
  setAmericaStake.m_Isin = m_strIsin;
  setAmericaStake.m_MarketCapitalization = ConvertValueToString(m_dMarketCapitalization);
  setAmericaStake.m_Naics = m_strNaics;
  setAmericaStake.m_NaicsNationalIndustry = m_strNaicsNationalIndustry;
  setAmericaStake.m_NaicsSector = m_strNaicsSector;
  setAmericaStake.m_NaicsSubsector = m_strNaicsSubsector;
  setAmericaStake.m_Name = m_strName;
  setAmericaStake.m_Phone = m_strPhone;
  setAmericaStake.m_ShareOutstanding = ConvertValueToString(m_dShareOutstanding);
  setAmericaStake.m_State = m_strState;
  setAmericaStake.m_Ticker = m_strTicker;
  setAmericaStake.m_WebURL = m_strWebURL;
  setAmericaStake.m_Logo = m_strLogo;
  setAmericaStake.m_FinnhubIndustry = m_strFinnhubIndustry;
  setAmericaStake.m_ProfileUpdateDate = m_lProfileUpdateDate;
  setAmericaStake.m_DayLineStartDate = m_lDayLineStartDate;
  setAmericaStake.m_DayLineEndDate = m_lDayLineEndDate;
  setAmericaStake.m_LastRTDataUpdateDate = m_lLastRTDataUpdateDate;
  setAmericaStake.m_IPOStatus = m_lIPOStatus;
  if (m_strWebURL.GetLength() > 100) {
    TRACE("%s字符串太长%d\n", m_strSymbol.GetBuffer(), m_strWebURL.GetLength());
  }
  TRACE("更新股票：%s\n", m_strSymbol.GetBuffer());
}

void CAmericaStake::Update(CSetAmericaStake& setAmericaStake) {
  setAmericaStake.Edit();
  Save(setAmericaStake);
  setAmericaStake.Update();
}

void CAmericaStake::Append(CSetAmericaStake& setAmericaStake) {
  setAmericaStake.AddNew();
  Save(setAmericaStake);
  setAmericaStake.Update();
}

bool CAmericaStake::SaveDayLine(void) {
  CSetAmericaStakeDayLine setAmericaStakeDayLine;

  size_t lSize = 0;
  vector<CDayLinePtr> vDayLine;
  CDayLinePtr pDayLine = nullptr;
  long lCurrentPos = 0, lSizeOfOldDayLine = 0;
  bool fNeedUpdate = false;

  ASSERT(m_vDayLine.size() > 0);

  lSize = m_vDayLine.size();
  setAmericaStakeDayLine.m_strFilter = _T("[Symbol] = '");
  setAmericaStakeDayLine.m_strFilter += m_strSymbol + _T("'");
  setAmericaStakeDayLine.m_strSort = _T("[Date]");

  setAmericaStakeDayLine.Open();
  while (!setAmericaStakeDayLine.IsEOF()) {
    pDayLine = make_shared<CDayLine>();
    pDayLine->LoadAmericaMarketData(&setAmericaStakeDayLine);
    vDayLine.push_back(pDayLine);
    lCurrentPos++;
    setAmericaStakeDayLine.MoveNext();
  }
  setAmericaStakeDayLine.Close();

  lSizeOfOldDayLine = lCurrentPos;
  lCurrentPos = 0;
  setAmericaStakeDayLine.m_strFilter = _T("[ID] = 1");
  setAmericaStakeDayLine.Open();
  setAmericaStakeDayLine.m_pDatabase->BeginTrans();
  for (int i = 0; i < lSize; i++) { // 数据是正序存储的，需要从头部开始存储
    pDayLine = m_vDayLine.at(i);
    while ((lCurrentPos < lSizeOfOldDayLine) && (vDayLine.at(lCurrentPos)->GetFormatedMarketDate() < pDayLine->GetFormatedMarketDate())) lCurrentPos++;
    if (lCurrentPos < lSizeOfOldDayLine) {
      if (vDayLine.at(lCurrentPos)->GetFormatedMarketDate() > pDayLine->GetFormatedMarketDate()) {
        pDayLine->AppendAmericaMarketData(&setAmericaStakeDayLine);
        fNeedUpdate = true;
      }
    }
    else {
      pDayLine->AppendAmericaMarketData(&setAmericaStakeDayLine);
      fNeedUpdate = true;
    }
  }
  setAmericaStakeDayLine.m_pDatabase->CommitTrans();
  setAmericaStakeDayLine.Close();

  return fNeedUpdate;
}

void CAmericaStake::UpdateDayLine(vector<CDayLinePtr>& vDayLine) {
  m_vDayLine.resize(0);
  for (auto& pDayLine : vDayLine) {
    m_vDayLine.push_back(pDayLine);
  }
}

void CAmericaStake::UpdateDayLineStartEndDate(void) {
  if (m_vDayLine.size() == 0) {
    SetDayLineStartDate(29900101);
    SetDayLineEndDate(__CHINA_MARKET_BEGIN_DATE__);
  }
  else {
    if (m_vDayLine.at(0)->GetFormatedMarketDate() < GetDayLineStartDate()) {
      SetDayLineStartDate(m_vDayLine.at(0)->GetFormatedMarketDate());
      SetDayLineDBUpdated(true);
    }
    if (m_vDayLine.at(m_vDayLine.size() - 1)->GetFormatedMarketDate() > GetDayLineEndDate()) {
      SetDayLineEndDate(m_vDayLine.at(m_vDayLine.size() - 1)->GetFormatedMarketDate());
      SetDayLineDBUpdated(true);
    }
  }
}

bool CAmericaStake::IsDayLineNeedSavingAndClearFlag(void) {
  const bool fNeedSaveing = m_fDayLineNeedSaving.exchange(false);
  return fNeedSaveing;
}

bool CAmericaStake::HaveNewDayLineData(void) {
  if (m_vDayLine.size() <= 0) return false;
  if (m_vDayLine.at(m_vDayLine.size() - 1)->GetFormatedMarketDate() > GetDayLineEndDate()) return true;
  else return false;
}

CString CAmericaStake::GetDayLineInquiryString(time_t tCurrentTime) {
  CString strMiddle = _T(""), strMiddle2 = _T(""), strMiddle3 = _T("");
  CString strTemp;
  char buffer[50];
  time_t tStartTime = 0;

  strMiddle += m_strSymbol;
  strMiddle += _T("&resolution=D");
  strMiddle += _T("&from=");
  tStartTime = FormatToTTime(m_lDayLineEndDate);
  if (tStartTime < (tCurrentTime - (time_t)(365) * 24 * 3600)) {
    tStartTime = (tCurrentTime - (time_t)(365) * 24 * 3600);
  }
  sprintf_s(buffer, _T("%I64i"), (INT64)tStartTime); // 免费账户只能读取一年以内的日线数据。
  strTemp = buffer;
  strMiddle += strTemp;
  strMiddle += _T("&to=");
  sprintf_s(buffer, _T("%I64i"), tCurrentTime);
  strTemp = buffer;
  strMiddle += strTemp;

  return strMiddle;
}