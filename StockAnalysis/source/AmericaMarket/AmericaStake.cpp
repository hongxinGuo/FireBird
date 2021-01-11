#include"globedef.h"
#include"accessory.h"
#include "AmericaStake.h"

#include"SetAmericaStakeDayLine.h"

CAmericaStake::CAmericaStake() : CObject() {
  Reset();
}

void CAmericaStake::Reset(void) {
  // Finnhub Symbol数据
  m_strIPODate = _T(" ");
  m_strCurrency = _T(" ");
  m_strType = _T(" ");
  m_strMic = _T(" ");
  m_strFigi = _T(" ");
  m_strCountry = _T(" ");
  m_strDescription = _T(" ");
  m_strDisplaySymbol = _T(" ");
  m_strExchange = _T(" ");
  m_strFinnhubIndustry = _T(" ");
  m_strPeer = _T(" ");
  m_strLogo = _T(" ");
  m_strName = _T(" ");
  m_strPhone = _T(" ");
  m_strSymbol = _T(" ");
  m_strTicker = _T(" ");
  m_strWebURL = _T(" ");
  m_strAddress = _T(" ");
  m_strCity = _T(" ");
  m_strCusip = _T(" ");
  m_strIsin = _T(" ");
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
  m_lPeerUpdateDate = 19800101;
  m_lLastEPSSurpriseUpdateDate = 19800101;
  m_lIPOStatus = __STAKE_NOT_CHECKED__;

  // Tiingo Symbol数据
  m_strTiingoPermaTicker = _T("");
  m_fIsActive = false;
  m_fIsADR = false;
  m_strTiingoIndustry = _T("");
  m_strTiingoSector = _T("");
  m_iSICCode = 0;
  m_strSICIndustry = _T("");
  m_strSICSector = _T("");
  m_strCompanyWebSite = _T("");
  m_strSECFilingWebSite = _T("");
  m_lDailyDataUpdateDate = 19800101;
  m_lStatementUpdateDate = 19800101;

  m_lEmployeeTotal = 0;
  m_dMarketCapitalization = 0;
  m_dShareOutstanding = 0;

  m_fUpdateDatabase = false;
  m_fInquiryAmericaStake = true;
  m_fEPSSurpriseNeedUpdate = true;
  m_fEPSSurpriseNeedSave = false;
  m_fFinnhubPeerUpdated = false;
  m_fDayLineNeedUpdate = true;
  m_fDayLineNeedSaving = false;

  m_TransactionTime = 0;
  m_lHigh = 0;
  m_lLow = 0;
  m_lLastClose = 0;
  m_lNew = 0;
  m_lOpen = 0;
  m_llTotalValue = 0;
  m_llCurrentValue = 0;
  m_llAmount = 0;
  m_lUpDown = 0;
  m_llVolume = 0;
  m_dUpDownRate = 0;
  m_dChangeHandRate = 0;

  m_vDayLine.resize(0);
}

void CAmericaStake::Load(CSetAmericaStake& setAmericaStake) {
  m_strSymbol = setAmericaStake.m_Symbol;
  m_strDescription = setAmericaStake.m_Description;
  m_strDisplaySymbol = setAmericaStake.m_DisplaySymbol;
  m_strType = setAmericaStake.m_Type;
  m_strMic = setAmericaStake.m_Mic;
  m_strFigi = setAmericaStake.m_Figi;
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
  m_strPeer = setAmericaStake.m_Peer;
  m_lProfileUpdateDate = setAmericaStake.m_ProfileUpdateDate;
  m_lDayLineStartDate = setAmericaStake.m_DayLineStartDate;
  m_lDayLineEndDate = setAmericaStake.m_DayLineEndDate;
  m_lLastRTDataUpdateDate = setAmericaStake.m_LastRTDataUpdateDate;
  m_lPeerUpdateDate = setAmericaStake.m_PeerUpdateDate;
  m_lLastEPSSurpriseUpdateDate = setAmericaStake.m_LastEPSSurpriseUpdateDate;
  m_lIPOStatus = setAmericaStake.m_IPOStatus;

  // Tiingo信息
  m_strTiingoPermaTicker = setAmericaStake.m_TiingoPermaTicker;
  m_fIsActive = setAmericaStake.m_IsActive;
  m_fIsADR = setAmericaStake.m_IsADR;
  m_iSICCode = setAmericaStake.m_SICCode;
  m_strSICIndustry = setAmericaStake.m_SICIndustry;
  m_strSICSector = setAmericaStake.m_SICSector;
  m_strTiingoIndustry = setAmericaStake.m_TiingoIndustry;
  m_strTiingoSector = setAmericaStake.m_TiingoSector;
  m_strCompanyWebSite = setAmericaStake.m_CompanyWebSite;
  m_strSECFilingWebSite = setAmericaStake.m_SECFilingWebSite;
  m_lDailyDataUpdateDate = setAmericaStake.m_DailyDataUpdateDate;
  m_lStatementUpdateDate = setAmericaStake.m_StatementUpdateDate;
}

bool CAmericaStake::CheckDayLineUpdateStatus(long lTodayDate, long lLastTradeDate, long lTime, long lDayOfWeek) {
  ASSERT(IsDayLineNeedUpdate()); // 默认状态为日线数据需要更新
  if ((lDayOfWeek > 0) && (lDayOfWeek < 6)) {
    if (!m_fIsActive) SetDayLineNeedUpdate(false);
  }
  if (IsNullStock()) {
    SetDayLineNeedUpdate(false);
  }
  else if (IsDelisted()) { // 摘牌股票?
    if (lDayOfWeek != 6) { // 每星期六检查一次
      SetDayLineNeedUpdate(false);
    }
  }
  else if ((!IsNotChecked()) && (gl_pAmericaMarket->IsEarlyThen(m_lDayLineEndDate, gl_pAmericaMarket->GetFormatedMarketDate(), 100))) {
    SetDayLineNeedUpdate(false);
  }
  else {
    if ((lDayOfWeek > 0) && (lDayOfWeek < 6)) {
      if (lTime > 170000) {
        if (lTodayDate <= GetDayLineEndDate()) { // 最新日线数据为今日的数据，而当前时间为下午五时之后
          SetDayLineNeedUpdate(false); // 日线数据不需要更新
        }
      }
      else {
        if (lLastTradeDate <= GetDayLineEndDate()) { // 最新日线数据为上一个交易日的数据,而当前时间为下午五时之前。
          SetDayLineNeedUpdate(false); // 日线数据不需要更新
        }
      }
    }
    else if (lLastTradeDate <= GetDayLineEndDate()) { // 最新日线数据为上一个交易日的数据,而当前时间为下午五时之前。
      SetDayLineNeedUpdate(false); // 日线数据不需要更新
    }
  }
  return m_fDayLineNeedUpdate;
}

void CAmericaStake::Save(CSetAmericaStake& setAmericaStake) {
  // 由于数据库的格式为定长的字符串，故而需要限制实际字符串的长度。
  m_strSymbol = m_strSymbol.Left(45);
  m_strDescription = m_strDescription.Left(200);
  m_strDisplaySymbol = m_strDisplaySymbol.Left(45);
  m_strType = m_strType.Left(45);
  m_strMic = m_strMic.Left(45);
  m_strFigi = m_strFigi.Left(45);
  m_strCurrency = m_strCurrency.Left(45);
  m_strAddress = m_strAddress.Left(100);
  m_strCity = m_strCity.Left(45);
  m_strCountry = m_strCountry.Left(45);
  m_strCusip = m_strCusip.Left(45);
  m_strSedol = m_strSedol.Left(45);
  m_strExchange = m_strExchange.Left(100);
  m_strGgroup = m_strGgroup.Left(45);
  m_strGind = m_strGind.Left(45);
  m_strGsector = m_strGsector.Left(45);
  m_strGsubind = m_strGsubind.Left(45);
  m_strIPODate = m_strIPODate.Left(20);
  m_strIsin = m_strIsin.Left(45);
  m_strNaics = m_strNaics.Left(45);
  m_strNaicsNationalIndustry = m_strNaicsNationalIndustry.Left(45);
  m_strNaicsSector = m_strNaicsSector.Left(45);
  m_strNaicsSubsector = m_strNaicsSubsector.Left(45);
  m_strName = m_strName.Left(100);
  m_strPhone = m_strPhone.Left(100);
  m_strState = m_strState.Left(45);
  m_strTicker = m_strTicker.Left(45);
  m_strWebURL = m_strWebURL.Left(150);
  m_strLogo = m_strLogo.Left(110);
  m_strFinnhubIndustry = m_strFinnhubIndustry.Left(100);
  m_strPeer = m_strPeer.Left(200);

  m_strSICIndustry = m_strSICIndustry.Left(45);
  m_strSICSector = m_strSICSector.Left(45);
  m_strTiingoIndustry = m_strTiingoIndustry.Left(45);
  m_strTiingoSector = m_strTiingoSector.Left(45);
  m_strCompanyWebSite = m_strCompanyWebSite.Left(100);
  m_strSECFilingWebSite = m_strSECFilingWebSite.Left(100);

  setAmericaStake.m_Symbol = m_strSymbol;
  setAmericaStake.m_Description = m_strDescription;
  setAmericaStake.m_DisplaySymbol = m_strDisplaySymbol;
  setAmericaStake.m_Type = m_strType;
  setAmericaStake.m_Mic = m_strMic;
  setAmericaStake.m_Figi = m_strFigi;
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
  setAmericaStake.m_Peer = m_strPeer;
  setAmericaStake.m_ProfileUpdateDate = m_lProfileUpdateDate;
  setAmericaStake.m_DayLineStartDate = m_lDayLineStartDate;
  setAmericaStake.m_DayLineEndDate = m_lDayLineEndDate;
  setAmericaStake.m_PeerUpdateDate = m_lPeerUpdateDate;
  setAmericaStake.m_LastRTDataUpdateDate = m_lLastRTDataUpdateDate;
  setAmericaStake.m_LastEPSSurpriseUpdateDate = m_lLastEPSSurpriseUpdateDate;
  setAmericaStake.m_IPOStatus = m_lIPOStatus;

  // Tiingo信息
  setAmericaStake.m_TiingoPermaTicker = m_strTiingoPermaTicker;
  setAmericaStake.m_IsActive = m_fIsActive;
  setAmericaStake.m_IsADR = m_fIsADR;
  setAmericaStake.m_SICCode = m_iSICCode;
  setAmericaStake.m_SICIndustry = m_strSICIndustry;
  setAmericaStake.m_SICSector = m_strSICSector;
  setAmericaStake.m_TiingoIndustry = m_strTiingoIndustry;
  setAmericaStake.m_TiingoSector = m_strTiingoSector;
  setAmericaStake.m_CompanyWebSite = m_strCompanyWebSite;
  setAmericaStake.m_SECFilingWebSite = m_strSECFilingWebSite;
  setAmericaStake.m_DailyDataUpdateDate = m_lDailyDataUpdateDate;
  setAmericaStake.m_StatementUpdateDate = m_lStatementUpdateDate;
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

void CAmericaStake::SaveDayLine(void) {
  CSetAmericaStakeDayLine setAmericaStakeDayLine, setSaveAmericaStakeDayLine;

  vector<CDayLinePtr> vDayLine;
  CDayLinePtr pDayLine = nullptr;
  long lCurrentPos = 0, lSizeOfOldDayLine = 0;

  ASSERT(m_vDayLine.size() > 0);

  if (m_vDayLine.at(0)->GetFormatedMarketDate() > m_lDayLineEndDate) { // 只是添加新的数据
    setSaveAmericaStakeDayLine.m_strFilter = _T("[ID] = 1");
    setSaveAmericaStakeDayLine.Open();
    setSaveAmericaStakeDayLine.m_pDatabase->BeginTrans();
    for (auto& pDayLine2 : m_vDayLine) {
      pDayLine2->AppendAmericaMarketData(&setSaveAmericaStakeDayLine);
    }
    setSaveAmericaStakeDayLine.m_pDatabase->CommitTrans();
    setSaveAmericaStakeDayLine.Close();
  }
  else { // 需要与之前的数据做对比
    setAmericaStakeDayLine.m_strFilter = _T("[Symbol] = '");
    setAmericaStakeDayLine.m_strFilter += m_strSymbol + _T("'");
    setAmericaStakeDayLine.m_strSort = _T("[Date]");

    setAmericaStakeDayLine.Open();
    while (!setAmericaStakeDayLine.IsEOF()) {
      pDayLine = make_shared<CDayLine>();
      pDayLine->LoadAmericaMarketData(&setAmericaStakeDayLine);
      vDayLine.push_back(pDayLine);
      lSizeOfOldDayLine++;
      setAmericaStakeDayLine.MoveNext();
    }
    if (lSizeOfOldDayLine > 0) {
      if (vDayLine.at(0)->GetFormatedMarketDate() < m_lDayLineStartDate) {
        m_lDayLineStartDate = vDayLine.at(0)->GetFormatedMarketDate();
      }
    }
    setAmericaStakeDayLine.Close();

    setSaveAmericaStakeDayLine.m_strFilter = _T("[ID] = 1");
    setSaveAmericaStakeDayLine.Open();
    setSaveAmericaStakeDayLine.m_pDatabase->BeginTrans();
    lCurrentPos = 0;
    for (int i = 0; i < m_vDayLine.size(); i++) { // 数据是正序存储的，需要从头部开始存储
      pDayLine = m_vDayLine.at(i);
      if (pDayLine->GetFormatedMarketDate() < m_lDayLineStartDate) { // 有更早的新数据？
        pDayLine->AppendAmericaMarketData(&setSaveAmericaStakeDayLine);
      }
      else {
        while ((lCurrentPos < lSizeOfOldDayLine) && (vDayLine.at(lCurrentPos)->GetFormatedMarketDate() < pDayLine->GetFormatedMarketDate())) lCurrentPos++;
        if (lCurrentPos < lSizeOfOldDayLine) {
          if (vDayLine.at(lCurrentPos)->GetFormatedMarketDate() > pDayLine->GetFormatedMarketDate()) {
            pDayLine->AppendAmericaMarketData(&setSaveAmericaStakeDayLine);
          }
        }
        else {
          pDayLine->AppendAmericaMarketData(&setSaveAmericaStakeDayLine);
        }
      }
    }
    setSaveAmericaStakeDayLine.m_pDatabase->CommitTrans();
    setSaveAmericaStakeDayLine.Close();
  }
}

bool CAmericaStake::UpdateEPSSurpriseDB(void) {
  CSetEPSSurprise setEPSSurprise;

  if (m_vEPSSurprise.size() == 0) return true;
  if (m_vEPSSurprise.at(m_vEPSSurprise.size() - 1)->m_lDate > m_lLastEPSSurpriseUpdateDate) {
    m_fUpdateDatabase = true;
  }
  else return false; // 没有新数据则返回

  setEPSSurprise.m_strFilter = _T("[ID] = 1");
  setEPSSurprise.Open();
  setEPSSurprise.m_pDatabase->BeginTrans();
  for (auto& pEPSSurprise : m_vEPSSurprise) { // 数据是正序存储的，需要从头部开始存储
    if (pEPSSurprise->m_lDate > m_lLastEPSSurpriseUpdateDate) {
      pEPSSurprise->Append(setEPSSurprise);
    }
  }
  setEPSSurprise.m_pDatabase->CommitTrans();
  setEPSSurprise.Close();
  m_lLastEPSSurpriseUpdateDate = m_vEPSSurprise.at(m_vEPSSurprise.size() - 1)->m_lDate;

  return true;
}

void CAmericaStake::UpdateDayLine(vector<CDayLinePtr>& vDayLine) {
  m_vDayLine.resize(0);
  for (auto& pDayLine : vDayLine) {
    m_vDayLine.push_back(pDayLine);
  }
}

void CAmericaStake::UpdateEPSSurprise(vector<CEPSSurprisePtr>& vEPSSurprise) {
  m_vEPSSurprise.resize(0);
  for (auto& p : vEPSSurprise) {
    m_vEPSSurprise.push_back(p);
  }
}

void CAmericaStake::UpdateDayLineStartEndDate(void) {
  if (m_vDayLine.size() == 0) {
  }
  else {
    if (m_vDayLine.at(0)->GetFormatedMarketDate() < GetDayLineStartDate()) {
      SetDayLineStartDate(m_vDayLine.at(0)->GetFormatedMarketDate());
      m_fUpdateDatabase = true;
    }
    if (m_vDayLine.at(m_vDayLine.size() - 1)->GetFormatedMarketDate() > GetDayLineEndDate()) {
      SetDayLineEndDate(m_vDayLine.at(m_vDayLine.size() - 1)->GetFormatedMarketDate());
      m_fUpdateDatabase = true;
    }
  }
}

bool CAmericaStake::IsDayLineNeedSavingAndClearFlag(void) {
  const bool fNeedSaveing = m_fDayLineNeedSaving.exchange(false);
  return fNeedSaveing;
}

bool CAmericaStake::HaveNewDayLineData(void) {
  if (m_vDayLine.size() == 0) return false;
  if ((m_vDayLine.at(m_vDayLine.size() - 1)->GetFormatedMarketDate() > GetDayLineEndDate())
      || (m_vDayLine.at(0)->GetFormatedMarketDate() < GetDayLineStartDate())) {
    return true;
  }
  else return false;
}

bool CAmericaStake::CheckEPSSurpriseStatus(long lCurrentDate) {
  if (IsNullStock() || IsDelisted()) {
    m_fEPSSurpriseNeedUpdate = false;
  }
  else if (m_lLastEPSSurpriseUpdateDate == 19700101) { // 没有数据？
    m_fEPSSurpriseNeedUpdate = false;
  }
  else if (!IsEarlyThen(m_lLastEPSSurpriseUpdateDate, lCurrentDate, 45)) { // 有不早于45天的数据？
    m_fEPSSurpriseNeedUpdate = false;
  }
  return m_fEPSSurpriseNeedUpdate;
}

bool CAmericaStake::IsEPSSurpriseNeedSaveAndClearFlag(void) {
  const bool fNeedSave = m_fEPSSurpriseNeedSave.exchange(false);
  return fNeedSave;
}

bool CAmericaStake::CheckPeerStatus(long lCurrentDate) {
  if (IsNullStock() || IsDelisted()) {
    m_fFinnhubPeerUpdated = true;
  }
  else if (!IsEarlyThen(m_lPeerUpdateDate, lCurrentDate, 90)) { // 有不早于45天的数据？
    m_fFinnhubPeerUpdated = true;
  }
  return m_fFinnhubPeerUpdated;
}

CString CAmericaStake::GetFinnhubDayLineInquiryString(time_t tCurrentTime) {
  CString strMiddle = _T(""), strMiddle2 = _T(""), strMiddle3 = _T("");
  CString strTemp;
  char buffer[50];
  time_t tStartTime = 0;

  strMiddle += m_strSymbol;
  strMiddle += _T("&resolution=D");
  strMiddle += _T("&from=");
  tStartTime = FormatToTTime(m_lDayLineEndDate);
  if (tStartTime < (tCurrentTime - (time_t)(365) * 24 * 3600)) {// 免费账户只能读取一年以内的日线数据。
    tStartTime = (tCurrentTime - (time_t)(365) * 24 * 3600);
  }
  sprintf_s(buffer, _T("%I64i"), (INT64)tStartTime);
  strTemp = buffer;
  strMiddle += strTemp;
  strMiddle += _T("&to=");
  sprintf_s(buffer, _T("%I64i"), tCurrentTime);
  strTemp = buffer;
  strMiddle += strTemp;

  return strMiddle;
}

CString CAmericaStake::GetTiingoDayLineInquiryString(long lCurrentDate) {
  CString strMiddle = _T("");
  CString strTemp;
  char buffer[50];
  long year = lCurrentDate / 10000;
  long month = lCurrentDate / 100 - year * 100;
  long date = lCurrentDate - year * 10000 - month * 100;

  strMiddle += m_strSymbol;
  strMiddle += _T("/prices?&startDate=1980-1-1&endDate=");
  sprintf_s(buffer, _T("%4d-%2d-%2d"), year, month, date);
  strTemp = buffer;
  strMiddle += strTemp;

  return strMiddle;
}