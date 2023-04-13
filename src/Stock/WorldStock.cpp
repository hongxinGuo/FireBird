#include"pch.h"

#include"CallableFunction.h"

#include"ConvertToString.h"
#include"TimeConvert.h"
#include"InsiderSentiment.h"
#include"InsiderTransaction.h"
#include"EPSSurprise.h"

#include "WorldStock.h"
#include"WorldMarket.h"

#include"FinnhubCompanyNews.h"
#include "InfoReport.h"
#include "JsonParse.h"

#include"SetInsiderSentiment.h"
#include"SetEPSSurprise.h"

using namespace std;

CWorldStock::CWorldStock() {
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
	m_jsonPeer = json({});
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
	m_lDayLineStartDate = 29900101;
	m_lDayLineEndDate = 19800101;

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

	m_lEmployeeTotal = 0;
	m_dMarketCapitalization = 0;
	m_dShareOutstanding = 0;

	m_fUpdateCompanyProfile = true;
	m_fUpdateCompanyNews = true;
	m_fUpdateBasicFinancial = true;
	m_fEPSSurpriseUpdated = false;
	m_fEPSSurpriseNeedSave = false;
	m_fUpdateFinnhubPeer = true;

	m_fUpdateFinnhubInsiderTransaction = true;
	m_fSaveFinnhubInsiderTransaction = false;
	m_fUpdateFinnhubInsiderSentiment = true;
	m_fSaveFinnhubInsiderSentiment = false;
	m_fUpdateFinnhubBasicFinancialDB = false;

	m_lInsiderSentimentStartDate = 19800101;
	m_lInsiderTransactionStartDate = 19800101;

	m_pBasicFinancial = nullptr;

	CWorldStock::Reset();
}

CWorldStock::~CWorldStock() {}

void CWorldStock::Reset() {
	CVirtualStock::Reset();

	ResetAllUpdateDate();
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
	m_jsonPeer = json({});
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
	m_lDayLineStartDate = 29900101;
	m_lDayLineEndDate = 19800101;

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

	m_lEmployeeTotal = 0;
	m_dMarketCapitalization = 0;
	m_dShareOutstanding = 0;

	m_fUpdateCompanyProfile = true;
	m_fUpdateCompanyNews = true;
	m_fUpdateBasicFinancial = true;
	m_fEPSSurpriseUpdated = false;
	m_fEPSSurpriseNeedSave = false;
	m_fUpdateFinnhubPeer = true;

	m_fUpdateFinnhubInsiderTransaction = true;
	m_fSaveFinnhubInsiderTransaction = false;
	m_fUpdateFinnhubInsiderSentiment = true;
	m_fSaveFinnhubInsiderSentiment = false;
	m_fUpdateFinnhubBasicFinancialDB = false;

	m_lInsiderSentimentStartDate = 19800101;
	m_lInsiderTransactionStartDate = 19800101;

	m_pBasicFinancial = nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// 所有更新日期的json数据皆在此预设。这样保证提取数据时不会发生exception，如果发生了exception,则程序出现错误。
/// 可以使用exception判断参数名称的正确与否。
///
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void CWorldStock::ResetAllUpdateDate() {
	m_jsonUpdateDate["Finnhub"]["StockFundamentalsCompanyProfileConcise"] = 19800101;
	m_jsonUpdateDate["Finnhub"]["StockFundamentalsCompanyNews"] = 19800101;
	m_jsonUpdateDate["Finnhub"]["StockFundamentalsBasicFinancials"] = 19800101;
	m_jsonUpdateDate["Finnhub"]["StockPriceQuote"] = 19800101;
	m_jsonUpdateDate["Finnhub"]["StockFundamentalsPeer"] = 19800101;
	m_jsonUpdateDate["Finnhub"]["StockFundamentalsInsiderTransaction"] = 19800101;
	m_jsonUpdateDate["Finnhub"]["StockFundamentalsInsiderSentiment"] = 19800101;
	m_jsonUpdateDate["Finnhub"]["StockEstimatesEPSSurprise"] = 19800101;
	//Tiingo自成一体
	m_jsonUpdateDate["Tiingo"]["StockFundamentalsCompanyProfile"] = 19800101;
	m_jsonUpdateDate["Tiingo"]["StockPriceCandles"] = 19800101;
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
	if (setWorldStock.m_Peer.GetLength() > 2) {
		// ReSharper disable once CppNoDiscardExpression
		CreateJsonWithNlohmann(m_jsonPeer, setWorldStock.m_Peer);
	}
	m_lDayLineStartDate = setWorldStock.m_DayLineStartDate;
	m_lDayLineEndDate = setWorldStock.m_DayLineEndDate;
	if (setWorldStock.m_UpdateDate.IsEmpty()) {
		ResetAllUpdateDate();
	}
	else {
		// ReSharper disable once CppNoDiscardExpression
		CreateJsonWithNlohmann(m_jsonUpdateDate, setWorldStock.m_UpdateDate);
	}
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
}

void CWorldStock::CheckUpdateStatus(long lTodayDate) {
	CheckProfileUpdateStatus(lTodayDate);
	CheckBasicFinancialUpdateStatus(lTodayDate);
	CheckCompanyNewsUpdateStatus(lTodayDate);
	CheckDayLineUpdateStatus(lTodayDate, gl_pWorldMarket->GetLastTradeDate(), gl_pWorldMarket->GetMarketTime(), gl_pWorldMarket->GetDayOfWeek());
	CheckEPSSurpriseStatus(lTodayDate);
	CheckPeerStatus(lTodayDate);
	CheckInsiderTransactionStatus(lTodayDate);
	CheckInsiderSentimentStatus(lTodayDate);
}

void CWorldStock::CheckProfileUpdateStatus(long lTodayDate) {
	if (IsEarlyThen(GetProfileUpdateDate(), lTodayDate, gl_systemConfiguration.GetStockProfileUpdateRate())) {
		m_fUpdateCompanyProfile = true;
	}
	else {
		m_fUpdateCompanyProfile = false;
	}
}

///
/// 默认状态为每周更新一次
///
bool CWorldStock::CheckCompanyNewsUpdateStatus(long lTodayDate) {
	ASSERT(m_fUpdateCompanyNews);
	if (!IsEarlyThen(GetCompanyNewsUpdateDate(), lTodayDate, 6)) {
		// 每星期更新一次公司新闻
		m_fUpdateCompanyNews = false;
	}
	else m_fUpdateCompanyNews = true;

	return m_fUpdateCompanyNews;
}

/// <summary>
/// 系统每季更新一次数据，故查询两次即可满足。所以设定45天查询一次
/// </summary>
/// <param name="lTodayDate"></param>
/// <returns></returns>
bool CWorldStock::CheckBasicFinancialUpdateStatus(long lTodayDate) {
	if (IsEarlyThen(GetBasicFinancialUpdateDate(), lTodayDate, gl_systemConfiguration.GetStockBasicFinancialUpdateRate())) {
		// 系统每季更新一次数据，故查询两次即可。
		m_fUpdateBasicFinancial = true;
	}
	else { m_fUpdateBasicFinancial = false; }
	return m_fUpdateBasicFinancial;
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
	if (IsNullStock()) {
		SetDayLineNeedUpdate(false);
		return m_fDayLineNeedUpdate;
	}
	if (IsDelisted() || IsNotYetList()) {
		// 摘牌股票?
		if (lDayOfWeek != 4) {
			// 每星期四检查一次
			SetDayLineNeedUpdate(false);
			return m_fDayLineNeedUpdate;
		}
	}
	else if ((!IsNotChecked()) && (IsEarlyThen(m_lDayLineEndDate, gl_pWorldMarket->GetMarketDate(), 100))) {
		SetDayLineNeedUpdate(false);
		return m_fDayLineNeedUpdate;
	}
	else {
		if ((lDayOfWeek > 0) && (lDayOfWeek < 6)) {
			// 周一至周五
			if (lTime > 170000) {
				if (lTodayDate <= GetDayLineEndDate()) {
					// 最新日线数据为今日的数据，而当前时间为下午五时之后
					SetDayLineNeedUpdate(false); // 日线数据不需要更新
					return m_fDayLineNeedUpdate;
				}
			}
			else {
				if (lLastTradeDate <= GetDayLineEndDate()) {
					// 最新日线数据为上一个交易日的数据,而当前时间为下午五时之前。
					SetDayLineNeedUpdate(false); // 日线数据不需要更新
					return m_fDayLineNeedUpdate;
				}
			}
		}
		else if (lLastTradeDate <= GetDayLineEndDate()) {
			// 周六周日时， 最新日线数据为上一个交易日的数据
			SetDayLineNeedUpdate(false); // 日线数据不需要更新
			return m_fDayLineNeedUpdate;
		}
	}
	return m_fDayLineNeedUpdate;
}

void CWorldStock::Save(CSetWorldStock& setWorldStock) const {
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
	const string sPeer = m_jsonPeer.dump();
	ASSERT(sPeer.size() < 2000);
	setWorldStock.m_Peer = sPeer.c_str();
	setWorldStock.m_DayLineStartDate = m_lDayLineStartDate;
	setWorldStock.m_DayLineEndDate = m_lDayLineEndDate;

	const string sUpdateDate = m_jsonUpdateDate.dump();
	setWorldStock.m_UpdateDate = sUpdateDate.c_str();
	ASSERT(sUpdateDate.size() < 10000);
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

void CWorldStock::SaveInsiderTransaction() {
	try {
		CSetInsiderTransaction setInsiderTransaction, setSaveInsiderTransaction;

		vector<CInsiderTransactionPtr> vInsiderTransaction;
		CInsiderTransactionPtr pInsiderTransaction = nullptr;
		long lSizeOfOldInsiderTransaction = 0;

		ASSERT(!m_vInsiderTransaction.empty());

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
		if (lSizeOfOldInsiderTransaction > 0) { if (vInsiderTransaction.at(0)->m_lTransactionDate < m_lInsiderTransactionStartDate) { m_lInsiderTransactionStartDate = vInsiderTransaction.at(0)->m_lTransactionDate; } }
		setInsiderTransaction.Close();

		setSaveInsiderTransaction.m_strFilter = _T("[ID] = 1");
		setSaveInsiderTransaction.Open();
		setSaveInsiderTransaction.m_pDatabase->BeginTrans();
		for (int i = 0; i < m_vInsiderTransaction.size(); i++) {
			pInsiderTransaction = m_vInsiderTransaction.at(i);
			if (find_if(vInsiderTransaction.begin(), vInsiderTransaction.end(),
			            [pInsiderTransaction](CInsiderTransactionPtr& p) {
				            return ((p->m_strSymbol.Compare(pInsiderTransaction->m_strSymbol) == 0) // 股票代码
					            && (p->m_lTransactionDate == pInsiderTransaction->m_lTransactionDate) // 交易时间
					            && (p->m_strPersonName.Compare(pInsiderTransaction->m_strPersonName) == 0) // 内部交易人员
					            && (p->m_strTransactionCode.Compare(pInsiderTransaction->m_strTransactionCode) == 0)); // 交易细节
			            }) == vInsiderTransaction.end()) {
				// 如果股票代码、人名、交易日期或者交易细节为新的数据，则存储该数据
				pInsiderTransaction->Append(setSaveInsiderTransaction);
			}
		}
		setSaveInsiderTransaction.m_pDatabase->CommitTrans();
		setSaveInsiderTransaction.Close();
	}
	catch (CException* e) {
		ReportInformationAndDeleteException(e);
	}
}

void CWorldStock::SaveInsiderSentiment() {
	try {
		CSetInsiderSentiment setInsiderSentiment, setSaveInsiderSentiment;

		vector<CInsiderSentimentPtr> vInsiderSentiment;
		CInsiderSentimentPtr pInsiderSentiment = nullptr;
		long lSizeOfOldInsiderSentiment = 0;

		ASSERT(!m_vInsiderSentiment.empty());

		setInsiderSentiment.m_strFilter = _T("[Symbol] = '");
		setInsiderSentiment.m_strFilter += m_strSymbol + _T("'");
		setInsiderSentiment.m_strSort = _T("[Date]");

		setInsiderSentiment.Open();
		while (!setInsiderSentiment.IsEOF()) {
			pInsiderSentiment = make_shared<CInsiderSentiment>();
			pInsiderSentiment->Load(setInsiderSentiment);
			vInsiderSentiment.push_back(pInsiderSentiment);
			lSizeOfOldInsiderSentiment++;
			setInsiderSentiment.MoveNext();
		}
		if (lSizeOfOldInsiderSentiment > 0) { if (vInsiderSentiment.at(0)->m_lDate < m_lInsiderSentimentStartDate) { m_lInsiderSentimentStartDate = vInsiderSentiment.at(0)->m_lDate; } }
		setInsiderSentiment.Close();

		setSaveInsiderSentiment.m_strFilter = _T("[ID] = 1");
		setSaveInsiderSentiment.Open();
		setSaveInsiderSentiment.m_pDatabase->BeginTrans();
		for (int i = 0; i < m_vInsiderSentiment.size(); i++) {
			pInsiderSentiment = m_vInsiderSentiment.at(i);
			if (find_if(vInsiderSentiment.begin(), vInsiderSentiment.end(),
			            [pInsiderSentiment](CInsiderSentimentPtr& p) {
				            return (p->m_lDate == pInsiderSentiment->m_lDate); // 报告时间
			            }) == vInsiderSentiment.end()) {
				// 如果报告日期为新的数据，则存储该数据
				pInsiderSentiment->Append(setSaveInsiderSentiment);
			}
		}
		setSaveInsiderSentiment.m_pDatabase->CommitTrans();
		setSaveInsiderSentiment.Close();
	}
	catch (CException* e) {
		ReportInformationAndDeleteException(e);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 将接收到的新闻存储入数库中。
// 接收到的新闻事先已经按时间顺序存于vector中，只存储数据库中没有的时间点的新闻。
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool CWorldStock::UpdateCompanyNewsDB() {
	ASSERT(m_vCompanyNews.size() > 0);
	const long lSize = static_cast<long>(m_vCompanyNews.size());
	if (m_strSymbol.GetLength() > 0) {
		CCompanyNewsPtr pCompanyNews;
		CSetCompanyNews setCompanyNews;
		long lCurrentPos = 0;
		setCompanyNews.m_strFilter = _T("[Symbol] = '");
		setCompanyNews.m_strFilter += m_strSymbol + _T("'");
		setCompanyNews.m_strSort = _T("[DateTime]");

		setCompanyNews.Open();
		setCompanyNews.m_pDatabase->BeginTrans();
		while (!setCompanyNews.IsEOF()) {
			pCompanyNews = m_vCompanyNews.at(lCurrentPos);
			while ((atoll(setCompanyNews.m_DateTime) < pCompanyNews->m_llDateTime) && !setCompanyNews.IsEOF()) setCompanyNews.MoveNext();
			if (setCompanyNews.IsEOF()) break;
			if ((atoll(setCompanyNews.m_DateTime) > pCompanyNews->m_llDateTime)) {
				// 没有这个时间点的新闻？
				pCompanyNews->Append(setCompanyNews);
			}
			if (++lCurrentPos == lSize) break;
		}
		for (long i = lCurrentPos; i < lSize; i++) {
			pCompanyNews = m_vCompanyNews.at(i);
			pCompanyNews->Append(setCompanyNews);
		}
		setCompanyNews.m_pDatabase->CommitTrans();
		setCompanyNews.Close();
	}
	return true;
}

bool CWorldStock::UpdateEPSSurpriseDB() {
	CSetEPSSurprise setEPSSurprise;
	const long lLastEPSSurpriseUpdateDate = GetLastEPSSurpriseUpdateDate();

	if (m_vEPSSurprise.empty()) return true;
	if (m_vEPSSurprise.at(m_vEPSSurprise.size() - 1)->m_lDate > lLastEPSSurpriseUpdateDate) { SetUpdateProfileDB(true); }
	else return false; // 没有新数据则返回

	setEPSSurprise.m_strFilter = _T("[ID] = 1");
	setEPSSurprise.Open();
	setEPSSurprise.m_pDatabase->BeginTrans();
	for (const auto& pEPSSurprise : m_vEPSSurprise) {
		// 数据是正序存储的，需要从头部开始存储
		if (pEPSSurprise->m_lDate > lLastEPSSurpriseUpdateDate) { pEPSSurprise->Append(setEPSSurprise); }
	}
	setEPSSurprise.m_pDatabase->CommitTrans();
	setEPSSurprise.Close();
	SetLastEPSSurpriseUpdateDate(m_vEPSSurprise.at(m_vEPSSurprise.size() - 1)->m_lDate);

	return true;
}

bool CWorldStock::UpdateDayLineDB() {
	if (IsDayLineNeedSavingAndClearFlag()) {
		// 清除标识需要与检测标识处于同一原子过程中，防止同步问题出现
		if (GetDayLineSize() > 0) {
			if (HaveNewDayLineData()) {
				SaveDayLine();
				UpdateDayLineStartEndDate();
				SetUpdateProfileDB(true);
				const CString str = GetSymbol() + _T("日线资料存储完成");
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

void CWorldStock::AppendBasicFinancialAnnual() const {
	try {
		CSetFinnhubStockBasicFinancialAnnual setAnnual;
		setAnnual.m_strFilter = _T("[ID] = 1");
		setAnnual.Open();
		setAnnual.m_pDatabase->BeginTrans();
		m_pBasicFinancial->AppendAnnualData(setAnnual);
		setAnnual.m_pDatabase->CommitTrans();
		setAnnual.Close();
	}
	catch (CException* e) {
		ReportInformationAndDeleteException(e);
	}
}

void CWorldStock::AppendBasicFinancialQuarter() const {
	try {
		CSetFinnhubStockBasicFinancialQuarter setQuarter;

		setQuarter.m_strFilter = _T("[ID] = 1");
		setQuarter.Open();
		setQuarter.m_pDatabase->BeginTrans();
		m_pBasicFinancial->AppendQuarterData(setQuarter);
		setQuarter.m_pDatabase->CommitTrans();
		setQuarter.Close();
	}
	catch (CException* e) {
		ReportInformationAndDeleteException(e);
	}
}

void CWorldStock::UpdateCompanyNews(const CCompanyNewsVectorPtr& pvCompanyNews) {
	m_vCompanyNews.resize(0);
	for (auto& p : *pvCompanyNews) {
		m_vCompanyNews.push_back(p);
	}
	ranges::sort(m_vCompanyNews, [](const CCompanyNewsPtr& p1, const CCompanyNewsPtr& p2) { return (p1->m_llDateTime < p2->m_llDateTime); }); // 此序列需要按时间顺序存放，以利于与存储于数据库中的数据作比较。
}

void CWorldStock::UpdateEPSSurprise(const vector<CEPSSurprisePtr>& vEPSSurprise) {
	m_vEPSSurprise.resize(0);
	for (auto& p : vEPSSurprise) {
		m_vEPSSurprise.push_back(p);
	}
}

bool CWorldStock::IsNeedUpdateProfile(const CTiingoStockPtr& pTiingoStock) {
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
	if (GetTiingoDailyDataUpdateDate() != pTiingoStock->m_lDailyDataUpdateDate) return true;
	if (GetTiingoStatementUpdateDate() != pTiingoStock->m_lStatementUpdateDate) return true;
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// 使用TiingoStock更新。
// Ticker, Name, Location和ReportingCurrency这四个数据不使用。
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
void CWorldStock::UpdateStockProfile(const CTiingoStockPtr& pTiingoStock) {
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
	SetTiingoDailyDataUpdateDate(pTiingoStock->m_lDailyDataUpdateDate);
	SetTiingoStatementUpdateDate(pTiingoStock->m_lStatementUpdateDate);
}

void CWorldStock::UpdateDayLineStartEndDate() {
	long lStartDate = 0, lEndDate = 0;
	if (m_dataDayLine.GetStartEndDate(lStartDate, lEndDate)) {
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

bool CWorldStock::HaveNewDayLineData() const {
	if (m_dataDayLine.Size() == 0) return false;
	if ((m_dataDayLine.GetData(m_dataDayLine.Size() - 1)->GetMarketDate() > m_lDayLineEndDate)
		|| (m_dataDayLine.GetData(0)->GetMarketDate() < m_lDayLineStartDate))
		return true;
	return false;
}

bool CWorldStock::UpdateBasicFinancial(const CFinnhubStockBasicFinancialPtr& pFinnhubStockBasicFinancial) {
	m_pBasicFinancial = pFinnhubStockBasicFinancial;

	return true;
}

bool CWorldStock::CheckEPSSurpriseStatus(long lCurrentDate) {
	const long lLastEPSSurpriseUpdateDate = GetLastEPSSurpriseUpdateDate();
	if (IsNullStock() || IsDelisted()) { m_fEPSSurpriseUpdated = true; }
	else if (lLastEPSSurpriseUpdateDate == 19700101) {
		// 没有数据？
		m_fEPSSurpriseUpdated = true;
	}
	else if (!IsEarlyThen(lLastEPSSurpriseUpdateDate, lCurrentDate, 135)) {
		// 有不早于135天的数据？
		m_fEPSSurpriseUpdated = true;
	}
	else if (IsEarlyThen(lLastEPSSurpriseUpdateDate, lCurrentDate, 225) && (lLastEPSSurpriseUpdateDate != 19800101)) {
		// 有早于225天的数据？
		m_fEPSSurpriseUpdated = true;
	}
	else { m_fEPSSurpriseUpdated = false; }
	return m_fEPSSurpriseUpdated;
}

bool CWorldStock::IsEPSSurpriseNeedSaveAndClearFlag() {
	const bool fNeedSave = m_fEPSSurpriseNeedSave.exchange(false);
	return fNeedSave;
}

bool CWorldStock::CheckPeerStatus(long lCurrentDate) {
	if (IsNullStock() || IsDelisted()) { m_fUpdateFinnhubPeer = false; }
	else if (!IsEarlyThen(GetPeerUpdateDate(), lCurrentDate, gl_systemConfiguration.GetStockPeerUpdateRate())) {
		// 有不早于90天的数据？
		m_fUpdateFinnhubPeer = false;
	}
	else { m_fUpdateFinnhubPeer = true; }
	return m_fUpdateFinnhubPeer;
}

void CWorldStock::UpdateInsiderTransaction(const vector<CInsiderTransactionPtr>& vInsiderTransaction) {
	m_vInsiderTransaction.resize(0);

	for (auto pInsiderTransaction : vInsiderTransaction) { m_vInsiderTransaction.push_back(pInsiderTransaction); }
}

bool CWorldStock::CheckInsiderTransactionStatus(long lCurrentDate) {
	if (!IsUSMarket()) { m_fUpdateFinnhubInsiderTransaction = false; }
	else if (IsNullStock() || IsDelisted()) { m_fUpdateFinnhubInsiderTransaction = false; }
	else if (!IsEarlyThen(GetInsiderTransactionUpdateDate(), lCurrentDate, gl_systemConfiguration.GetInsideTransactionUpdateRate())) {
		// 有不早于30天的数据？
		m_fUpdateFinnhubInsiderTransaction = false;
	}
	else { m_fUpdateFinnhubInsiderTransaction = true; }
	return m_fUpdateFinnhubInsiderTransaction;
}

void CWorldStock::UpdateInsiderSentiment(const vector<CInsiderSentimentPtr>& vInsiderSentiment) {
	m_vInsiderSentiment.resize(0);

	for (auto pInsiderSentiment : vInsiderSentiment) {
		m_vInsiderSentiment.push_back(pInsiderSentiment);
	}
}

bool CWorldStock::CheckInsiderSentimentStatus(long lCurrentDate) {
	if (!IsUSMarket()) { m_fUpdateFinnhubInsiderSentiment = false; }
	else if (IsNullStock() || IsDelisted()) { m_fUpdateFinnhubInsiderSentiment = false; }
	else if (!IsEarlyThen(GetInsiderSentimentUpdateDate(), lCurrentDate, gl_systemConfiguration.GetInsideSentimentUpdateRate())) {
		// 有不早于30天的数据？
		m_fUpdateFinnhubInsiderSentiment = false;
	}
	else { m_fUpdateFinnhubInsiderSentiment = true; }
	return m_fUpdateFinnhubInsiderSentiment;
}

long CWorldStock::GetProfileUpdateDate() {
	try {
		const long lDate = m_jsonUpdateDate.at(_T("Finnhub")).at("StockFundamentalsCompanyProfileConcise");
		return lDate;
	}
	catch (json::exception&) {
		return 19800101;
	}
}

void CWorldStock::SetProfileUpdateDate(const long lProfileUpdateDate) noexcept {
	m_jsonUpdateDate["Finnhub"]["StockFundamentalsCompanyProfileConcise"] = lProfileUpdateDate;
}

long CWorldStock::GetCompanyNewsUpdateDate() {
	try {
		const long lDate = m_jsonUpdateDate.at(_T("Finnhub")).at("StockFundamentalsCompanyNews");
		return lDate;
	}
	catch (json::exception&) {
		return 19800101;
	}
}

void CWorldStock::SetCompanyNewsUpdateDate(const long lCompanyNewsUpdateDate) noexcept {
	m_jsonUpdateDate["Finnhub"]["StockFundamentalsCompanyNews"] = lCompanyNewsUpdateDate;
}

long CWorldStock::GetBasicFinancialUpdateDate() {
	try {
		const long lDate = m_jsonUpdateDate.at(_T("Finnhub")).at("StockFundamentalsBasicFinancials");
		return lDate;
	}
	catch (json::exception&) {
		return 19800101;
	}
}

void CWorldStock::SetBasicFinancialUpdateDate(const long lBasicFinancialUpdateDate) noexcept {
	m_jsonUpdateDate["Finnhub"]["StockFundamentalsBasicFinancials"] = lBasicFinancialUpdateDate;
}

long CWorldStock::GetLastRTDataUpdateDate() {
	try {
		const long lDate = m_jsonUpdateDate.at(_T("Finnhub")).at("StockPriceQuote");
		return lDate;
	}
	catch (json::exception&) {
		return 19800101;
	}
}

void CWorldStock::SetLastRTDataUpdateDate(const long lDate) noexcept {
	m_jsonUpdateDate["Finnhub"]["StockPriceQuote"] = lDate;
}

long CWorldStock::GetPeerUpdateDate() {
	try {
		const long lDate = m_jsonUpdateDate.at(_T("Finnhub")).at("StockFundamentalsPeer");
		return lDate;
	}
	catch (json::exception&) {
		return 19800101;
	}
}

void CWorldStock::SetPeerUpdateDate(const long lDate) noexcept {
	m_jsonUpdateDate["Finnhub"]["StockFundamentalsPeer"] = lDate;
}

long CWorldStock::GetInsiderTransactionUpdateDate() {
	try {
		const long lDate = m_jsonUpdateDate.at(_T("Finnhub")).at("StockFundamentalsInsiderTransaction");
		return lDate;
	}
	catch (json::exception&) {
		return 19800101;
	}
}

void CWorldStock::SetInsiderTransactionUpdateDate(const long lDate) noexcept {
	m_jsonUpdateDate["Finnhub"]["StockFundamentalsInsiderTransaction"] = lDate;
}

long CWorldStock::GetInsiderSentimentUpdateDate() {
	try {
		const long lDate = m_jsonUpdateDate.at(_T("Finnhub")).at("StockFundamentalsInsiderSentiment");
		return lDate;
	}
	catch (json::exception&) {
		return 19800101;
	}
}

void CWorldStock::SetInsiderSentimentUpdateDate(const long lDate) noexcept {
	m_jsonUpdateDate["Finnhub"]["StockFundamentalsInsiderSentiment"] = lDate;
}

long CWorldStock::GetLastEPSSurpriseUpdateDate() {
	try {
		const long lDate = m_jsonUpdateDate.at(_T("Finnhub")).at("StockEstimatesEPSSurprise");
		return lDate;
	}
	catch (json::exception&) {
		return 19800101;
	}
}

void CWorldStock::SetLastEPSSurpriseUpdateDate(const long lDate) noexcept {
	m_jsonUpdateDate["Finnhub"]["StockEstimatesEPSSurprise"] = lDate;
}

long CWorldStock::GetTiingoStatementUpdateDate() {
	try {
		const long lDate = m_jsonUpdateDate.at(_T("Tiingo")).at("StockFundamentalsCompanyProfile");
		return lDate;
	}
	catch (json::exception&) {
		return 19800101;
	}
}

void CWorldStock::SetTiingoStatementUpdateDate(const long lStatementUpdateDate) noexcept {
	m_jsonUpdateDate["Tiingo"]["StockFundamentalsCompanyProfile"] = lStatementUpdateDate;
}

long CWorldStock::GetTiingoDailyDataUpdateDate() {
	try {
		const long lDate = m_jsonUpdateDate.at(_T("Tiingo")).at("StockPriceCandles");
		return lDate;
	}
	catch (json::exception&) {
		return 19800101;
	}
}

void CWorldStock::SetTiingoDailyDataUpdateDate(const long lDailyDataUpdateDate) noexcept {
	m_jsonUpdateDate["Tiingo"]["StockPriceCandles"] = lDailyDataUpdateDate;
}

CString CWorldStock::GetFinnhubDayLineInquiryParam(time_t tCurrentTime) const {
	CString strParam = _T("");
	char buffer[50];

	strParam += m_strSymbol;
	strParam += _T("&resolution=D");
	strParam += _T("&from=");
	const time_t tStartTime = (tCurrentTime - static_cast<time_t>(365) * 24 * 3600); // 检查最近一年的数据
	sprintf_s(buffer, _T("%I64i"), tStartTime);
	CString strTemp = buffer;
	strParam += strTemp;
	strParam += _T("&to=");
	sprintf_s(buffer, _T("%I64i"), tCurrentTime);
	strTemp = buffer;
	strParam += strTemp;

	return strParam;
}

CString CWorldStock::GetTiingoDayLineInquiryParam(long lStartDate, long lCurrentDate) const {
	CString strParam = _T("");
	char buffer[50];
	const long year = lCurrentDate / 10000;
	const long month = lCurrentDate / 100 - year * 100;
	const long date = lCurrentDate - year * 10000 - month * 100;

	const long year2 = lStartDate / 10000;
	const long month2 = lStartDate / 100 - year2 * 100;
	const long date2 = lStartDate - year2 * 10000 - month2 * 100;

	strParam += m_strSymbol;
	strParam += _T("/prices?&startDate=");
	sprintf_s(buffer, _T("%4d-%d-%d"), year2, month2, date2);
	CString strTemp = buffer;
	strParam += strTemp;
	strParam += _T("&endDate=");
	sprintf_s(buffer, _T("%4d-%d-%d"), year, month, date);
	strTemp = buffer;
	strParam += strTemp;

	return strParam;
}

bool CWorldStock::IsUSMarket() const {
	if (m_strExchangeCode.Compare(_T("US")) == 0) return true;
	return false;
}
