#include"globedef.h"
#include"accessory.h"
#include "WorldStock.h"

#include"SetWorldStockDayLine.h"

CWorldStock::CWorldStock() : CObject() {
  Reset();
}

void CWorldStock::Reset(void) {
  // Finnhub Symbol����
  m_strIPODate = _T(" ");
  m_strCurrency = _T(" ");
  m_strType = _T(" ");
  m_strMic = _T(" ");
  m_strFigi = _T(" ");
  m_strCountry = _T(" ");
  m_strDescription = _T(" ");
  m_strDisplaySymbol = _T(" ");
  m_strListedExchange = _T(" ");
  m_strFinnhubIndustry = _T(" ");
  m_strPeer = _T(" ");
  m_strLogo = _T(" ");
  m_strName = _T(" ");
  m_strPhone = _T(" ");
  m_strSymbol = _T(" ");
  m_strExchangeCode = _T("US");
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

  // Tiingo Symbol����
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

  m_strSymbolForSort = _T("");

  m_fUpdateDatabase = false;
  m_fInquiryStockProfile = true;
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

void CWorldStock::Load(CSetWorldStock& setWorldStock) {
  m_strSymbol = setWorldStock.m_Symbol;
  m_strExchangeCode = setWorldStock.m_ExchangeCode;
  m_strDescription = setWorldStock.m_Description;
  m_strDisplaySymbol = setWorldStock.m_DisplaySymbol;
  m_strType = setWorldStock.m_Type;
  m_strMic = setWorldStock.m_Mic;
  m_strFigi = setWorldStock.m_Figi;
  m_strCurrency = setWorldStock.m_Currency;
  m_strAddress = setWorldStock.m_Address;
  m_strCity = setWorldStock.m_City;
  m_strCountry = setWorldStock.m_Country;
  m_strCusip = setWorldStock.m_Cusip;
  m_strSedol = setWorldStock.m_Sedol;
  m_lEmployeeTotal = setWorldStock.m_EmployeeTotal;
  m_strListedExchange = setWorldStock.m_ListedExchange;
  m_strGgroup = setWorldStock.m_Ggroup;
  m_strGind = setWorldStock.m_Gind;
  m_strGsector = setWorldStock.m_Gsector;
  m_strGsubind = setWorldStock.m_Gsubind;
  m_strIPODate = setWorldStock.m_IPODate;
  m_strIsin = setWorldStock.m_Isin;
  m_dMarketCapitalization = atof(setWorldStock.m_MarketCapitalization);
  m_strNaics = setWorldStock.m_Naics;
  m_strNaicsNationalIndustry = setWorldStock.m_NaicsNationalIndustry;
  m_strNaicsSector = setWorldStock.m_NaicsSector;
  m_strNaicsSubsector = setWorldStock.m_NaicsSubsector;
  m_strName = setWorldStock.m_Name;
  m_strPhone = setWorldStock.m_Phone;
  m_dShareOutstanding = atof(setWorldStock.m_ShareOutstanding);
  m_strState = setWorldStock.m_State;
  m_strTicker = setWorldStock.m_Ticker;
  m_strWebURL = setWorldStock.m_WebURL;
  m_strLogo = setWorldStock.m_Logo;
  m_strFinnhubIndustry = setWorldStock.m_FinnhubIndustry;
  m_strPeer = setWorldStock.m_Peer;
  m_lProfileUpdateDate = setWorldStock.m_ProfileUpdateDate;
  m_lDayLineStartDate = setWorldStock.m_DayLineStartDate;
  m_lDayLineEndDate = setWorldStock.m_DayLineEndDate;
  m_lLastRTDataUpdateDate = setWorldStock.m_LastRTDataUpdateDate;
  m_lPeerUpdateDate = setWorldStock.m_PeerUpdateDate;
  m_lLastEPSSurpriseUpdateDate = setWorldStock.m_LastEPSSurpriseUpdateDate;
  m_lIPOStatus = setWorldStock.m_IPOStatus;

  // Tiingo��Ϣ
  m_strTiingoPermaTicker = setWorldStock.m_TiingoPermaTicker;
  m_fIsActive = setWorldStock.m_IsActive;
  m_fIsADR = setWorldStock.m_IsADR;
  m_iSICCode = setWorldStock.m_SICCode;
  m_strSICIndustry = setWorldStock.m_SICIndustry;
  m_strSICSector = setWorldStock.m_SICSector;
  m_strTiingoIndustry = setWorldStock.m_TiingoIndustry;
  m_strTiingoSector = setWorldStock.m_TiingoSector;
  m_strCompanyWebSite = setWorldStock.m_CompanyWebSite;
  m_strSECFilingWebSite = setWorldStock.m_SECFilingWebSite;
  m_lDailyDataUpdateDate = setWorldStock.m_DailyDataUpdateDate;
  m_lStatementUpdateDate = setWorldStock.m_StatementUpdateDate;
}

bool CWorldStock::CheckDayLineUpdateStatus(long lTodayDate, long lLastTradeDate, long lTime, long lDayOfWeek) {
  ASSERT(IsDayLineNeedUpdate()); // Ĭ��״̬Ϊ����������Ҫ����
  if ((lDayOfWeek > 0) && (lDayOfWeek < 6)) {
    if (!m_fIsActive) SetDayLineNeedUpdate(false);
  }
  if (IsNullStock()) {
    SetDayLineNeedUpdate(false);
  }
  else if (IsDelisted()) { // ժ�ƹ�Ʊ?
    if (lDayOfWeek != 6) { // ÿ���������һ��
      SetDayLineNeedUpdate(false);
    }
  }
  else if ((!IsNotChecked()) && (gl_pWorldMarket->IsEarlyThen(m_lDayLineEndDate, gl_pWorldMarket->GetFormatedMarketDate(), 100))) {
    SetDayLineNeedUpdate(false);
  }
  else {
    if ((lDayOfWeek > 0) && (lDayOfWeek < 6)) {
      if (lTime > 170000) {
        if (lTodayDate <= GetDayLineEndDate()) { // ������������Ϊ���յ����ݣ�����ǰʱ��Ϊ������ʱ֮��
          SetDayLineNeedUpdate(false); // �������ݲ���Ҫ����
        }
      }
      else {
        if (lLastTradeDate <= GetDayLineEndDate()) { // ������������Ϊ��һ�������յ�����,����ǰʱ��Ϊ������ʱ֮ǰ��
          SetDayLineNeedUpdate(false); // �������ݲ���Ҫ����
        }
      }
    }
    else if (lLastTradeDate <= GetDayLineEndDate()) { // ������������Ϊ��һ�������յ�����,����ǰʱ��Ϊ������ʱ֮ǰ��
      SetDayLineNeedUpdate(false); // �������ݲ���Ҫ����
    }
  }
  return m_fDayLineNeedUpdate;
}

void CWorldStock::Save(CSetWorldStock& setWorldStock) {
  // �������ݿ�ĸ�ʽΪ�������ַ������ʶ���Ҫ����ʵ���ַ����ĳ��ȡ�
  m_strSymbol = m_strSymbol.Left(45);
  m_strExchangeCode = m_strExchangeCode.Left(45);
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
  m_strListedExchange = m_strListedExchange.Left(100);
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

  setWorldStock.m_Symbol = m_strSymbol;
  setWorldStock.m_ExchangeCode = m_strExchangeCode;
  setWorldStock.m_Description = m_strDescription;
  setWorldStock.m_DisplaySymbol = m_strDisplaySymbol;
  setWorldStock.m_Type = m_strType;
  setWorldStock.m_Mic = m_strMic;
  setWorldStock.m_Figi = m_strFigi;
  setWorldStock.m_Currency = m_strCurrency;
  setWorldStock.m_Address = m_strAddress;
  setWorldStock.m_City = m_strCity;
  setWorldStock.m_Country = m_strCountry;
  setWorldStock.m_Cusip = m_strCusip;
  setWorldStock.m_Sedol = m_strSedol;
  setWorldStock.m_EmployeeTotal = m_lEmployeeTotal;
  setWorldStock.m_ListedExchange = m_strListedExchange;
  setWorldStock.m_Ggroup = m_strGgroup;
  setWorldStock.m_Gind = m_strGind;
  setWorldStock.m_Gsector = m_strGsector;
  setWorldStock.m_Gsubind = m_strGsubind;
  setWorldStock.m_IPODate = m_strIPODate;
  setWorldStock.m_Isin = m_strIsin;
  setWorldStock.m_MarketCapitalization = ConvertValueToString(m_dMarketCapitalization);
  setWorldStock.m_Naics = m_strNaics;
  setWorldStock.m_NaicsNationalIndustry = m_strNaicsNationalIndustry;
  setWorldStock.m_NaicsSector = m_strNaicsSector;
  setWorldStock.m_NaicsSubsector = m_strNaicsSubsector;
  setWorldStock.m_Name = m_strName;
  setWorldStock.m_Phone = m_strPhone;
  setWorldStock.m_ShareOutstanding = ConvertValueToString(m_dShareOutstanding);
  setWorldStock.m_State = m_strState;
  setWorldStock.m_Ticker = m_strTicker;
  setWorldStock.m_WebURL = m_strWebURL;
  setWorldStock.m_Logo = m_strLogo;
  setWorldStock.m_FinnhubIndustry = m_strFinnhubIndustry;
  setWorldStock.m_Peer = m_strPeer;
  setWorldStock.m_ProfileUpdateDate = m_lProfileUpdateDate;
  setWorldStock.m_DayLineStartDate = m_lDayLineStartDate;
  setWorldStock.m_DayLineEndDate = m_lDayLineEndDate;
  setWorldStock.m_PeerUpdateDate = m_lPeerUpdateDate;
  setWorldStock.m_LastRTDataUpdateDate = m_lLastRTDataUpdateDate;
  setWorldStock.m_LastEPSSurpriseUpdateDate = m_lLastEPSSurpriseUpdateDate;
  setWorldStock.m_IPOStatus = m_lIPOStatus;

  // Tiingo��Ϣ
  setWorldStock.m_TiingoPermaTicker = m_strTiingoPermaTicker;
  setWorldStock.m_IsActive = m_fIsActive;
  setWorldStock.m_IsADR = m_fIsADR;
  setWorldStock.m_SICCode = m_iSICCode;
  setWorldStock.m_SICIndustry = m_strSICIndustry;
  setWorldStock.m_SICSector = m_strSICSector;
  setWorldStock.m_TiingoIndustry = m_strTiingoIndustry;
  setWorldStock.m_TiingoSector = m_strTiingoSector;
  setWorldStock.m_CompanyWebSite = m_strCompanyWebSite;
  setWorldStock.m_SECFilingWebSite = m_strSECFilingWebSite;
  setWorldStock.m_DailyDataUpdateDate = m_lDailyDataUpdateDate;
  setWorldStock.m_StatementUpdateDate = m_lStatementUpdateDate;
}

void CWorldStock::Update(CSetWorldStock& setWorldStock) {
  setWorldStock.Edit();
  Save(setWorldStock);
  setWorldStock.Update();
}

void CWorldStock::Append(CSetWorldStock& setWorldStock) {
  setWorldStock.AddNew();
  Save(setWorldStock);
  setWorldStock.Update();
}

void CWorldStock::SaveDayLine(void) {
  CSetWorldStockDayLine setWorldStockDayLine, setSaveWorldStockDayLine;

  vector<CDayLinePtr> vDayLine;
  CDayLinePtr pDayLine = nullptr;
  long lCurrentPos = 0, lSizeOfOldDayLine = 0;

  ASSERT(m_vDayLine.size() > 0);

  setWorldStockDayLine.m_strFilter = _T("[Symbol] = '");
  setWorldStockDayLine.m_strFilter += m_strSymbol + _T("'");
  setWorldStockDayLine.m_strSort = _T("[Date]");

  setWorldStockDayLine.Open();
  while (!setWorldStockDayLine.IsEOF()) {
    pDayLine = make_shared<CDayLine>();
    pDayLine->LoadWorldMarketData(&setWorldStockDayLine);
    vDayLine.push_back(pDayLine);
    lSizeOfOldDayLine++;
    setWorldStockDayLine.MoveNext();
  }
  if (lSizeOfOldDayLine > 0) {
    if (vDayLine.at(0)->GetFormatedMarketDate() < m_lDayLineStartDate) {
      m_lDayLineStartDate = vDayLine.at(0)->GetFormatedMarketDate();
    }
  }
  setWorldStockDayLine.Close();

  setSaveWorldStockDayLine.m_strFilter = _T("[ID] = 1");
  setSaveWorldStockDayLine.Open();
  setSaveWorldStockDayLine.m_pDatabase->BeginTrans();
  lCurrentPos = 0;
  for (int i = 0; i < m_vDayLine.size(); i++) { // ����������洢�ģ���Ҫ��ͷ����ʼ�洢
    pDayLine = m_vDayLine.at(i);
    if (pDayLine->GetFormatedMarketDate() < m_lDayLineStartDate) { // �и���������ݣ�
      pDayLine->AppendWorldMarketData(&setSaveWorldStockDayLine);
    }
    else {
      while ((lCurrentPos < lSizeOfOldDayLine) && (vDayLine.at(lCurrentPos)->GetFormatedMarketDate() < pDayLine->GetFormatedMarketDate())) lCurrentPos++;
      if (lCurrentPos < lSizeOfOldDayLine) {
        if (vDayLine.at(lCurrentPos)->GetFormatedMarketDate() > pDayLine->GetFormatedMarketDate()) {
          pDayLine->AppendWorldMarketData(&setSaveWorldStockDayLine);
        }
      }
      else {
        pDayLine->AppendWorldMarketData(&setSaveWorldStockDayLine);
      }
    }
  }
  setSaveWorldStockDayLine.m_pDatabase->CommitTrans();
  setSaveWorldStockDayLine.Close();
}

bool CWorldStock::UpdateEPSSurpriseDB(void) {
  CSetEPSSurprise setEPSSurprise;

  if (m_vEPSSurprise.size() == 0) return true;
  if (m_vEPSSurprise.at(m_vEPSSurprise.size() - 1)->m_lDate > m_lLastEPSSurpriseUpdateDate) {
    m_fUpdateDatabase = true;
  }
  else return false; // û���������򷵻�

  setEPSSurprise.m_strFilter = _T("[ID] = 1");
  setEPSSurprise.Open();
  setEPSSurprise.m_pDatabase->BeginTrans();
  for (auto& pEPSSurprise : m_vEPSSurprise) { // ����������洢�ģ���Ҫ��ͷ����ʼ�洢
    if (pEPSSurprise->m_lDate > m_lLastEPSSurpriseUpdateDate) {
      pEPSSurprise->Append(setEPSSurprise);
    }
  }
  setEPSSurprise.m_pDatabase->CommitTrans();
  setEPSSurprise.Close();
  m_lLastEPSSurpriseUpdateDate = m_vEPSSurprise.at(m_vEPSSurprise.size() - 1)->m_lDate;

  return true;
}

void CWorldStock::UpdateDayLine(vector<CDayLinePtr>& vDayLine) {
  m_vDayLine.resize(0);
  for (auto& pDayLine : vDayLine) {
    m_vDayLine.push_back(pDayLine);
  }
}

void CWorldStock::UpdateEPSSurprise(vector<CEPSSurprisePtr>& vEPSSurprise) {
  m_vEPSSurprise.resize(0);
  for (auto& p : vEPSSurprise) {
    m_vEPSSurprise.push_back(p);
  }
}

void CWorldStock::UpdateSymbolForSort(void) {
  m_strSymbolForSort = m_strExchangeCode + _T(".") + m_strSymbol;
}

void CWorldStock::UpdateDayLineStartEndDate(void) {
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

bool CWorldStock::IsDayLineNeedSavingAndClearFlag(void) {
  const bool fNeedSaveing = m_fDayLineNeedSaving.exchange(false);
  return fNeedSaveing;
}

bool CWorldStock::HaveNewDayLineData(void) {
  if (m_vDayLine.size() == 0) return false;
  if ((m_vDayLine.at(m_vDayLine.size() - 1)->GetFormatedMarketDate() > GetDayLineEndDate())
      || (m_vDayLine.at(0)->GetFormatedMarketDate() < GetDayLineStartDate())) {
    return true;
  }
  else return false;
}

bool CWorldStock::CheckEPSSurpriseStatus(long lCurrentDate) {
  if (IsNullStock() || IsDelisted()) {
    m_fEPSSurpriseNeedUpdate = false;
  }
  else if (m_lLastEPSSurpriseUpdateDate == 19700101) { // û�����ݣ�
    m_fEPSSurpriseNeedUpdate = false;
  }
  else if (!IsEarlyThen(m_lLastEPSSurpriseUpdateDate, lCurrentDate, 135)) { // �в�����135������ݣ�
    m_fEPSSurpriseNeedUpdate = false;
  }
  else if (IsEarlyThen(m_lLastEPSSurpriseUpdateDate, lCurrentDate, 225) && (m_lLastEPSSurpriseUpdateDate != 19800101)) { // ������225������ݣ�
    m_fEPSSurpriseNeedUpdate = false;
  }
  return m_fEPSSurpriseNeedUpdate;
}

bool CWorldStock::IsEPSSurpriseNeedSaveAndClearFlag(void) {
  const bool fNeedSave = m_fEPSSurpriseNeedSave.exchange(false);
  return fNeedSave;
}

bool CWorldStock::CheckPeerStatus(long lCurrentDate) {
  if (IsNullStock() || IsDelisted()) {
    m_fFinnhubPeerUpdated = true;
  }
  else if (!IsEarlyThen(m_lPeerUpdateDate, lCurrentDate, 90)) { // �в�����45������ݣ�
    m_fFinnhubPeerUpdated = true;
  }
  return m_fFinnhubPeerUpdated;
}

CString CWorldStock::GetFinnhubDayLineInquiryString(time_t tCurrentTime) {
  CString strMiddle = _T(""), strMiddle2 = _T(""), strMiddle3 = _T("");
  CString strTemp;
  char buffer[50];
  time_t tStartTime = 0;
  long lStartDate = 0;

  strMiddle += m_strSymbol;
  strMiddle += _T("&resolution=D");
  strMiddle += _T("&from=");
  lStartDate = gl_pWorldMarket->GetNextDay(m_lDayLineEndDate);
  tStartTime = FormatToTTime(lStartDate);
  if (tStartTime < (tCurrentTime - (time_t)(365) * 24 * 3600)) {// ����˻�ֻ�ܶ�ȡһ�����ڵ��������ݡ�
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

CString CWorldStock::GetTiingoDayLineInquiryString(long lCurrentDate) {
  CString strMiddle = _T("");
  CString strTemp;
  char buffer[50];
  long year = lCurrentDate / 10000;
  long month = lCurrentDate / 100 - year * 100;
  long date = lCurrentDate - year * 10000 - month * 100;

  strMiddle += m_strSymbol;
  strMiddle += _T("/prices?&startDate=1950-1-1&endDate=");
  sprintf_s(buffer, _T("%4d-%2d-%2d"), year, month, date);
  strTemp = buffer;
  strMiddle += strTemp;

  return strMiddle;
}

bool CWorldStock::IsUSMarket(void) {
  if (m_strExchangeCode.Compare(_T("US")) == 0) return true;
  else return false;
}