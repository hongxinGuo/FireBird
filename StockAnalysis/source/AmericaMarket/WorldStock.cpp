#include"pch.h"

#include"accessory.h"
#include"TimeConvert.h"

#include "WorldStock.h"
#include"WorldMarket.h"

#include"SetWorldStockDayLine.h"

CWorldStock::CWorldStock() : CVirtualStock() {
	Reset();
}

CWorldStock::~CWorldStock() {
	m_pBasicFinancial = nullptr;
}

void CWorldStock::Reset(void) {
	CVirtualStock::Reset();
	// Finnhub Symbol数据
	m_strIPODate = _T(" ");
	m_strCurrency = _T(" ");
	m_strType = _T(" ");
	m_strMic = _T(" ");
	m_strFigi = _T(" ");
	m_strShareClassFIGI = _T("");
	m_strSymbol2 = _T("");
	m_strCountry = _T(" ");
	m_strListedExchange = _T(" ");
	m_strFinnhubIndustry = _T(" ");
	m_strPeer = _T(" ");
	m_strLogo = _T(" ");
	m_strName = _T(" ");
	m_strPhone = _T(" ");
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
	m_lBasicFinancialUpdateDate = 19800101;
	m_lDayLineStartDate = 29900101;
	m_lDayLineEndDate = 19800101;
	m_lLastRTDataUpdateDate = 19800101;
	m_lPeerUpdateDate = 19800101;
	m_lInsiderTransactionUpdateDate = 19800101;
	m_lLastEPSSurpriseUpdateDate = 19800101;

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

	m_fProfileUpdated = false;
	m_fBasicFinancialUpdated = false;
	m_fEPSSurpriseUpdated = false;
	m_fEPSSurpriseNeedSave = false;
	m_fFinnhubPeerUpdated = false;

	m_fFinnhubInsiderTransactionNeedUpdate = true;
	m_fUpdateFinnhubBasicFinancialDB = false;
	m_fFinnhubInsiderTransactionNeedSave = false;

	m_pBasicFinancial = nullptr;
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
	m_lBasicFinancialUpdateDate = setWorldStock.m_BasicFinancialUpdateDate;
	m_lDayLineStartDate = setWorldStock.m_DayLineStartDate;
	m_lDayLineEndDate = setWorldStock.m_DayLineEndDate;
	m_lLastRTDataUpdateDate = setWorldStock.m_LastRTDataUpdateDate;
	m_lPeerUpdateDate = setWorldStock.m_PeerUpdateDate;
	m_lInsiderTransactionUpdateDate = setWorldStock.m_InsiderTransactionUpdateDate;
	m_lLastEPSSurpriseUpdateDate = setWorldStock.m_LastEPSSurpriseUpdateDate;
	m_lIPOStatus = setWorldStock.m_IPOStatus;

	// Tiingo信息
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

void CWorldStock::CheckUpdateStatus(long lTodayDate) {
	CheckProfileUpdateStatus(lTodayDate);
	CheckBasicFinancialUpdateStatus(lTodayDate);
	CheckDayLineUpdateStatus(lTodayDate, gl_pWorldMarket->GetLastTradeDate(), gl_pWorldMarket->GetMarketTime(), gl_pWorldMarket->GetDayOfWeek());
	CheckEPSSurpriseStatus(lTodayDate);
	CheckPeerStatus(lTodayDate);
	CheckInsiderTransactionStatus(lTodayDate);
}

bool CWorldStock::CheckProfileUpdateStatus(long lTodayDate) {
	if (IsEarlyThen(GetProfileUpdateDate(), lTodayDate, gl_systemConfigeration.GetStockProfileUpdateRate())) {
		m_fProfileUpdated = false;
	}
	else {
		m_fProfileUpdated = true;
	}
	return m_fProfileUpdated;
}

/// <summary>
/// 系统每季更新一次数据，故查询两次即可满足。所以设定45天查询一次
/// </summary>
/// <param name="lTodayDate"></param>
/// <returns></returns>
bool CWorldStock::CheckBasicFinancialUpdateStatus(long lTodayDate) {
	if (IsEarlyThen(GetBasicFinancialUpdateDate(), lTodayDate, gl_systemConfigeration.GetStockBasicFinancialUpdateRate())) { // 系统每季更新一次数据，故查询两次即可。
		m_fBasicFinancialUpdated = false;
	}
	else {
		m_fBasicFinancialUpdated = true;
	}
	return m_fBasicFinancialUpdated;
}

/// <summary>
/// 默认状态为日线需要更新。
/// 未上市股票无需查询，摘牌股票每星期六查询一次，
/// </summary>
/// <param name="lTodayDate"></param>
/// <param name="lLastTradeDate"></param>
/// <param name="lTime"></param>
/// <param name="lDayOfWeek"></param>
/// <returns></returns>
bool CWorldStock::CheckDayLineUpdateStatus(long lTodayDate, long lLastTradeDate, long lTime, long lDayOfWeek) {
	ASSERT(IsDayLineNeedUpdate()); // 默认状态为日线数据需要更新
	if (lDayOfWeek != 4) {
		if (!m_fIsActive) {
			SetDayLineNeedUpdate(false);
			return m_fDayLineNeedUpdate;
		}
	}
	if (IsNullStock()) {
		SetDayLineNeedUpdate(false);
		return m_fDayLineNeedUpdate;
	}
	else if (IsDelisted() || IsNotYetList()) { // 摘牌股票?
		if (lDayOfWeek != 4) { // 每星期四检查一次
			SetDayLineNeedUpdate(false);
			return m_fDayLineNeedUpdate;
		}
	}
	else if ((!IsNotChecked()) && (gl_pWorldMarket->IsEarlyThen(m_lDayLineEndDate, gl_pWorldMarket->GetMarketDate(), 100))) {
		SetDayLineNeedUpdate(false);
		return m_fDayLineNeedUpdate;
	}
	else {
		if ((lDayOfWeek > 0) && (lDayOfWeek < 6)) { // 周一至周五
			if (lTime > 170000) {
				if (lTodayDate <= GetDayLineEndDate()) { // 最新日线数据为今日的数据，而当前时间为下午五时之后
					SetDayLineNeedUpdate(false); // 日线数据不需要更新
					return m_fDayLineNeedUpdate;
				}
			}
			else {
				if (lLastTradeDate <= GetDayLineEndDate()) { // 最新日线数据为上一个交易日的数据,而当前时间为下午五时之前。
					SetDayLineNeedUpdate(false); // 日线数据不需要更新
					return m_fDayLineNeedUpdate;
				}
			}
		}
		else if (lLastTradeDate <= GetDayLineEndDate()) { // 周六周日时， 最新日线数据为上一个交易日的数据
			SetDayLineNeedUpdate(false); // 日线数据不需要更新
			return m_fDayLineNeedUpdate;
		}
	}
	return m_fDayLineNeedUpdate;
}

void CWorldStock::Save(CSetWorldStock& setWorldStock) {
	// 由于数据库的格式为定长的字符串，故而需要限制实际字符串的长度。
	setWorldStock.m_Symbol = m_strSymbol.Left(20);
	setWorldStock.m_ExchangeCode = m_strExchangeCode.Left(3);
	setWorldStock.m_Description = m_strDescription.Left(200);
	setWorldStock.m_DisplaySymbol = m_strDisplaySymbol.Left(20);
	setWorldStock.m_Type = m_strType.Left(25);
	setWorldStock.m_Mic = m_strMic.Left(20);
	setWorldStock.m_Figi = m_strFigi.Left(20);
	setWorldStock.m_Currency = m_strCurrency.Left(20);
	setWorldStock.m_Address = m_strAddress.Left(100);
	setWorldStock.m_City = m_strCity.Left(20);
	setWorldStock.m_Country = m_strCountry.Left(20);
	setWorldStock.m_Cusip = m_strCusip.Left(20);
	setWorldStock.m_Sedol = m_strSedol.Left(45);
	setWorldStock.m_EmployeeTotal = m_lEmployeeTotal;
	setWorldStock.m_ListedExchange = m_strListedExchange.Left(100);
	setWorldStock.m_Ggroup = m_strGgroup.Left(45);
	setWorldStock.m_Gind = m_strGind.Left(45);
	setWorldStock.m_Gsector = m_strGsector.Left(45);
	setWorldStock.m_Gsubind = m_strGsubind.Left(45);
	setWorldStock.m_IPODate = m_strIPODate.Left(20);
	setWorldStock.m_Isin = m_strIsin.Left(45);
	setWorldStock.m_MarketCapitalization = ConvertValueToString(m_dMarketCapitalization);
	setWorldStock.m_Naics = m_strNaics.Left(45);
	setWorldStock.m_NaicsNationalIndustry = m_strNaicsNationalIndustry.Left(100);
	setWorldStock.m_NaicsSector = m_strNaicsSector.Left(100);
	setWorldStock.m_NaicsSubsector = m_strNaicsSubsector.Left(100);
	setWorldStock.m_Name = m_strName.Left(200);
	setWorldStock.m_Phone = m_strPhone.Left(100);
	setWorldStock.m_ShareOutstanding = ConvertValueToString(m_dShareOutstanding);
	setWorldStock.m_State = m_strState.Left(45);
	setWorldStock.m_Ticker = m_strTicker.Left(45);
	setWorldStock.m_WebURL = m_strWebURL.Left(150);
	setWorldStock.m_Logo = m_strLogo.Left(110);
	setWorldStock.m_FinnhubIndustry = m_strFinnhubIndustry.Left(100);
	setWorldStock.m_Peer = m_strPeer.Left(200);
	setWorldStock.m_ProfileUpdateDate = m_lProfileUpdateDate;
	setWorldStock.m_BasicFinancialUpdateDate = m_lBasicFinancialUpdateDate;
	setWorldStock.m_DayLineStartDate = m_lDayLineStartDate;
	setWorldStock.m_DayLineEndDate = m_lDayLineEndDate;
	setWorldStock.m_PeerUpdateDate = m_lPeerUpdateDate;
	setWorldStock.m_InsiderTransactionUpdateDate = m_lInsiderTransactionUpdateDate;
	setWorldStock.m_LastRTDataUpdateDate = m_lLastRTDataUpdateDate;
	setWorldStock.m_LastEPSSurpriseUpdateDate = m_lLastEPSSurpriseUpdateDate;
	setWorldStock.m_IPOStatus = m_lIPOStatus;

	// Tiingo信息
	setWorldStock.m_TiingoPermaTicker = m_strTiingoPermaTicker;
	setWorldStock.m_IsActive = m_fIsActive;
	setWorldStock.m_IsADR = m_fIsADR;
	setWorldStock.m_SICCode = m_iSICCode;
	setWorldStock.m_SICIndustry = m_strSICIndustry.Left(100);
	setWorldStock.m_SICSector = m_strSICSector.Left(100);
	setWorldStock.m_TiingoIndustry = m_strTiingoIndustry.Left(100);
	setWorldStock.m_TiingoSector = m_strTiingoSector.Left(100);
	setWorldStock.m_CompanyWebSite = m_strCompanyWebSite.Left(100);
	setWorldStock.m_SECFilingWebSite = m_strSECFilingWebSite.Left(150);
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

void CWorldStock::SaveInsiderTransaction(void) {
	CSetInsiderTransaction setInsiderTransaction, setSaveInsiderTransaction;

	vector<CInsiderTransactionPtr> vInsiderTransaction;
	CInsiderTransactionPtr pInsiderTransaction = nullptr;
	long lCurrentPos = 0, lSizeOfOldInsiderTransaction = 0;

	ASSERT(m_vInsiderTransaction.size() > 0);

	setInsiderTransaction.m_strFilter = _T("[Symbol] = '");
	setInsiderTransaction.m_strFilter += m_strSymbol + _T("'");
	setInsiderTransaction.m_strSort = _T("[TransactionDate]");

	setInsiderTransaction.Open();
	while (!setInsiderTransaction.IsEOF()) {
		pInsiderTransaction = make_shared<CInsiderTransaction>();
		pInsiderTransaction->Load(setInsiderTransaction);
		vInsiderTransaction.push_back(pInsiderTransaction);
		lSizeOfOldInsiderTransaction++;
		setInsiderTransaction.MoveNext();
	}
	if (lSizeOfOldInsiderTransaction > 0) {
		if (vInsiderTransaction.at(0)->m_lTransactionDate < m_lInsiderTransactionStartDate) {
			m_lInsiderTransactionStartDate = vInsiderTransaction.at(0)->m_lTransactionDate;
		}
	}
	setInsiderTransaction.Close();

	setSaveInsiderTransaction.m_strFilter = _T("[ID] = 1");
	setSaveInsiderTransaction.Open();
	setSaveInsiderTransaction.m_pDatabase->BeginTrans();
	lCurrentPos = 0;
	for (int i = 0; i < m_vInsiderTransaction.size(); i++) {
		pInsiderTransaction = m_vInsiderTransaction.at(i);
		if (find_if(vInsiderTransaction.begin(), vInsiderTransaction.end(),
			[pInsiderTransaction](CInsiderTransactionPtr& p) {
				return ((p->m_strSymbol.Compare(pInsiderTransaction->m_strSymbol) == 0) // 股票代码
					&& (p->m_lTransactionDate == pInsiderTransaction->m_lTransactionDate) // 交易时间
					&& (p->m_strPersonName.Compare(pInsiderTransaction->m_strPersonName) == 0) // 内部交易人员
					&& (p->m_strTransactionCode.Compare(pInsiderTransaction->m_strTransactionCode) == 0)); // 交易细节
			}) == vInsiderTransaction.end()) { // 如果股票代码、人名、交易日期或者交易细节为新的数据，则存储该数据
			pInsiderTransaction->Append(setSaveInsiderTransaction);
		}
	}
	setSaveInsiderTransaction.m_pDatabase->CommitTrans();
	setSaveInsiderTransaction.Close();
}

bool CWorldStock::UpdateEPSSurpriseDB(void) {
	CSetEPSSurprise setEPSSurprise;

	if (m_vEPSSurprise.size() == 0) return true;
	if (m_vEPSSurprise.at(m_vEPSSurprise.size() - 1)->m_lDate > m_lLastEPSSurpriseUpdateDate) {
		SetUpdateProfileDB(true);
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

bool CWorldStock::UpdateDayLineDB(void) {
	CString str;

	if (IsDayLineNeedSavingAndClearFlag()) { // 清除标识需要与检测标识处于同一原子过程中，防止同步问题出现
		if (GetDayLineSize() > 0) {
			if (HaveNewDayLineData()) {
				SaveDayLine();
				UpdateDayLineStartEndDate();
				SetUpdateProfileDB(true);
				str = GetSymbol() + _T("日线资料存储完成");
				gl_systemMessage.PushDayLineInfoMessage(str);
				//TRACE("更新%s日线数据\n", GetSymbol().GetBuffer());
				UnloadDayLine();
				return true;
			}
			UnloadDayLine();
		}
	}
	return false;
}

void CWorldStock::AppendBasicFinancialAnnual(void) {
	CSetFinnhubStockBasicFinancialAnnual setAnnual;

	setAnnual.m_strFilter = _T("[ID] = 1");
	setAnnual.Open();
	setAnnual.m_pDatabase->BeginTrans();
	m_pBasicFinancial->AppendAnnualData(setAnnual);
	setAnnual.m_pDatabase->CommitTrans();
	setAnnual.Close();
}

void CWorldStock::AppendBasicFinancialQuarter(void) {
	CSetFinnhubStockBasicFinancialQuarter setQuarter;

	setQuarter.m_strFilter = _T("[ID] = 1");
	setQuarter.Open();
	setQuarter.m_pDatabase->BeginTrans();
	m_pBasicFinancial->AppendQuarterData(setQuarter);
	setQuarter.m_pDatabase->CommitTrans();
	setQuarter.Close();
}

void CWorldStock::UpdateEPSSurprise(vector<CEPSSurprisePtr>& vEPSSurprise) {
	m_vEPSSurprise.resize(0);
	for (auto& p : vEPSSurprise) {
		m_vEPSSurprise.push_back(p);
	}
}

bool CWorldStock::IsNeedUpdateProfile(CTiingoStockPtr pTiingoStock) {
	if (m_strTiingoPermaTicker.Compare(pTiingoStock->m_strTiingoPermaTicker) != 0) return true;
	if (m_strName.Compare(pTiingoStock->m_strName) != 0) return true;
	if ((m_fIsActive != pTiingoStock->m_fIsActive)) return true;
	if (m_fIsADR != pTiingoStock->m_fIsADR) return true;
	if (m_strTiingoIndustry.Compare(pTiingoStock->m_strTiingoIndustry) != 0) return true;
	if (m_strTiingoSector.Compare(pTiingoStock->m_strTiingoSector) != 0) return true;
	if (m_strSICIndustry.Compare(pTiingoStock->m_strSICIndustry) != 0) return true;
	if (m_strSICSector.Compare(pTiingoStock->m_strSICSector) != 0) return true;
	if (m_iSICCode != pTiingoStock->m_iSICCode) return true;
	if (m_strCompanyWebSite.Compare(pTiingoStock->m_strCompanyWebSite) != 0) return true;
	if (m_strSECFilingWebSite.Compare(pTiingoStock->m_strSECFilingWebSite) != 0) return true;
	if (m_lDailyDataUpdateDate != pTiingoStock->m_lDailyDataUpdateDate) return true;
	if (m_lStatementUpdateDate != pTiingoStock->m_lStatementUpdateDate) return true;
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// 使用TiingoStock更新。
// Ticker, Name, Location和ReportingCurrency这四个数据不使用。
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CWorldStock::UpdateStockProfile(CTiingoStockPtr pTiingoStock) {
	m_strTiingoPermaTicker = pTiingoStock->m_strTiingoPermaTicker;
	m_strName = pTiingoStock->m_strName;
	m_fIsActive = pTiingoStock->m_fIsActive;
	m_fIsADR = pTiingoStock->m_fIsADR;
	m_strTiingoIndustry = pTiingoStock->m_strTiingoIndustry;
	m_strTiingoSector = pTiingoStock->m_strTiingoSector;
	m_strSICIndustry = pTiingoStock->m_strSICIndustry;
	m_strSICSector = pTiingoStock->m_strSICSector;
	m_iSICCode = pTiingoStock->m_iSICCode;
	m_strCompanyWebSite = pTiingoStock->m_strCompanyWebSite;
	m_strSECFilingWebSite = pTiingoStock->m_strSECFilingWebSite;
	m_lDailyDataUpdateDate = pTiingoStock->m_lDailyDataUpdateDate;
	m_lStatementUpdateDate = pTiingoStock->m_lStatementUpdateDate;
}

void CWorldStock::UpdateDayLineStartEndDate(void) {
	long lStartDate = 0, lEndDate = 0;
	bool fSucceed = m_dataDayLine.GetStartEndDate(lStartDate, lEndDate);
	if (fSucceed) {
		if (lStartDate < GetDayLineStartDate()) {
			SetDayLineStartDate(lStartDate);
			m_fUpdateProfileDB = true;
		}
		if (lEndDate > m_lDayLineEndDate) {
			SetDayLineEndDate(lEndDate);
			m_fUpdateProfileDB = true;
		}
	}
}

bool CWorldStock::HaveNewDayLineData(void) {
	if (m_dataDayLine.Size() == 0) return false;
	if ((m_dataDayLine.GetData(m_dataDayLine.Size() - 1)->GetMarketDate() > m_lDayLineEndDate)
		|| (m_dataDayLine.GetData(0)->GetMarketDate() < m_lDayLineStartDate)) return true;
	else return false;
}

bool CWorldStock::UpdateBasicFinancial(CFinnhubStockBasicFinancialPtr pFinnhubStockBasicFinancial) {
	m_pBasicFinancial = pFinnhubStockBasicFinancial;

	return true;
}

bool CWorldStock::CheckEPSSurpriseStatus(long lCurrentDate) {
	if (IsNullStock() || IsDelisted()) {
		m_fEPSSurpriseUpdated = true;
	}
	else if (m_lLastEPSSurpriseUpdateDate == 19700101) { // 没有数据？
		m_fEPSSurpriseUpdated = true;
	}
	else if (!IsEarlyThen(m_lLastEPSSurpriseUpdateDate, lCurrentDate, 135)) { // 有不早于135天的数据？
		m_fEPSSurpriseUpdated = true;
	}
	else if (IsEarlyThen(m_lLastEPSSurpriseUpdateDate, lCurrentDate, 225) && (m_lLastEPSSurpriseUpdateDate != 19800101)) { // 有早于225天的数据？
		m_fEPSSurpriseUpdated = true;
	}
	else {
		m_fEPSSurpriseUpdated = false;
	}
	return m_fEPSSurpriseUpdated;
}

bool CWorldStock::IsEPSSurpriseNeedSaveAndClearFlag(void) {
	const bool fNeedSave = m_fEPSSurpriseNeedSave.exchange(false);
	return fNeedSave;
}

bool CWorldStock::CheckPeerStatus(long lCurrentDate) {
	if (IsNullStock() || IsDelisted()) {
		m_fFinnhubPeerUpdated = true;
	}
	else if (!IsEarlyThen(m_lPeerUpdateDate, lCurrentDate, gl_systemConfigeration.GetStockPeerUpdateRate())) { // 有不早于90天的数据？
		m_fFinnhubPeerUpdated = true;
	}
	else {
		m_fFinnhubPeerUpdated = false;
	}
	return m_fFinnhubPeerUpdated;
}

void CWorldStock::UpdateInsiderTransaction(vector<CInsiderTransactionPtr>& vInsiderTransaction) {
	m_vInsiderTransaction.resize(0);

	for (auto pInsiderTransacton : vInsiderTransaction) {
		m_vInsiderTransaction.push_back(pInsiderTransacton);
	}
}

bool CWorldStock::CheckInsiderTransactionStatus(long lCurrentDate) {
	if (!IsUSMarket()) {
		m_fFinnhubInsiderTransactionNeedUpdate = false;
	}
	else if (IsNullStock() || IsDelisted()) {
		m_fFinnhubInsiderTransactionNeedUpdate = false;
	}
	else if (!IsEarlyThen(m_lInsiderTransactionUpdateDate, lCurrentDate, gl_systemConfigeration.GetInsideTransactionUpdateRate())) { // 有不早于30天的数据？
		m_fFinnhubInsiderTransactionNeedUpdate = false;
	}
	else {
		m_fFinnhubInsiderTransactionNeedUpdate = true;
	}
	return m_fFinnhubInsiderTransactionNeedUpdate;
}

CString CWorldStock::GetFinnhubDayLineInquiryString(time_t tCurrentTime) {
	CString strMiddle = _T(""), strMiddle2 = _T(""), strMiddle3 = _T("");
	CString strTemp;
	char buffer[50];
	time_t tStartTime = 0;

	strMiddle += m_strSymbol;
	strMiddle += _T("&resolution=D");
	strMiddle += _T("&from=");
	tStartTime = (tCurrentTime - (time_t)(365) * 24 * 3600);// 检查最近一年的数据
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
	strMiddle += _T("/prices?&startDate=1980-1-1&endDate=");
	sprintf_s(buffer, _T("%4d-%d-%d"), year, month, date);
	strTemp = buffer;
	strMiddle += strTemp;

	return strMiddle;
}

bool CWorldStock::IsUSMarket(void) {
	if (m_strExchangeCode.Compare(_T("US")) == 0) return true;
	else return false;
}