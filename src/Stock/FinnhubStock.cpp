#include"pch.h"
import FireBird.System.Message;

import FireBird.Accessory.ConvertToString;
import FireBird.Accessory.TimeConvert;
#include"InsiderSentiment.h"
#include"InsiderTransaction.h"
#include"EPSSurprise.h"

#include "FinnhubStock.h"
#include"WorldMarket.h"

#include"FinnhubCompanyNews.h"
import FireBird.Accessory.InfoReport;
#include "JsonParse.h"

#include"SetInsiderSentiment.h"
#include"SetEPSSurprise.h"
#include "SetSECFilings.h"

CFinnhubStock::CFinnhubStock() {
	SetExchangeCode(_T("US"));
	CFinnhubStock::ResetAllUpdateDate();
}

CFinnhubStock::~CFinnhubStock() {
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// 所有更新日期的json数据皆在此预设。这样保证提取数据时不会发生exception，如果发生了exception,则程序出现错误。
/// 可以使用exception判断参数名称的正确与否。
///
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void CFinnhubStock::ResetAllUpdateDate() {
	m_jsonUpdateDate["DayLineStartDate"] = 29900101;
	m_jsonUpdateDate["DayLineEndDate"] = 19800101;
	m_jsonUpdateDate["Finnhub"]["StockFundamentalsCompanyProfileConcise"] = 19800101;
	m_jsonUpdateDate["Finnhub"]["StockFundamentalsCompanyNews"] = 19800101;
	m_jsonUpdateDate["Finnhub"]["StockFundamentalsBasicFinancials"] = 19800101;
	m_jsonUpdateDate["Finnhub"]["StockPriceQuote"] = 19800101;
	m_jsonUpdateDate["Finnhub"]["StockFundamentalsPeer"] = 19800101;
	m_jsonUpdateDate["Finnhub"]["StockFundamentalsInsiderTransaction"] = 19800101;
	m_jsonUpdateDate["Finnhub"]["StockFundamentalsInsiderSentiment"] = 19800101;
	m_jsonUpdateDate["Finnhub"]["StockFundamentalsSECFilings"] = 19800101;
	m_jsonUpdateDate["Finnhub"]["StockEstimatesEPSSurprise"] = 19800101;
}

void CFinnhubStock::Load(CSetFinnhubStock& setFinnhubStock) {
	m_strSymbol = setFinnhubStock.m_Symbol;
	m_strExchangeCode = setFinnhubStock.m_ExchangeCode;
	m_strDescription = setFinnhubStock.m_Description;
	m_strDisplaySymbol = setFinnhubStock.m_DisplaySymbol;
	m_strType = setFinnhubStock.m_Type;
	m_strMic = setFinnhubStock.m_Mic;
	m_strFigi = setFinnhubStock.m_Figi;
	m_strCurrency = setFinnhubStock.m_Currency;
	m_strAddress = setFinnhubStock.m_Address;
	m_strCity = setFinnhubStock.m_City;
	m_strCountry = setFinnhubStock.m_Country;
	m_strCusip = setFinnhubStock.m_Cusip;
	m_strSedol = setFinnhubStock.m_Sedol;
	m_lEmployeeTotal = setFinnhubStock.m_EmployeeTotal;
	//m_strExchangeCode = setFinnhubStock.m_ListedExchange;
	m_strGgroup = setFinnhubStock.m_Ggroup;
	m_strGind = setFinnhubStock.m_Gind;
	m_strGsector = setFinnhubStock.m_Gsector;
	m_strGsubind = setFinnhubStock.m_Gsubind;
	m_strIPODate = setFinnhubStock.m_IPODate;
	m_strIsin = setFinnhubStock.m_Isin;
	m_dMarketCapitalization = atof(setFinnhubStock.m_MarketCapitalization);
	m_strNaics = setFinnhubStock.m_Naics;
	m_strNaicsNationalIndustry = setFinnhubStock.m_NaicsNationalIndustry;
	m_strNaicsSector = setFinnhubStock.m_NaicsSector;
	m_strNaicsSubsector = setFinnhubStock.m_NaicsSubsector;
	m_strName = setFinnhubStock.m_Name;
	m_strPhone = setFinnhubStock.m_Phone;
	m_dShareOutstanding = atof(setFinnhubStock.m_ShareOutstanding);
	m_strState = setFinnhubStock.m_State;
	m_strTicker = setFinnhubStock.m_Ticker;
	m_strWebURL = setFinnhubStock.m_WebURL;
	m_strLogo = setFinnhubStock.m_Logo;
	m_strFinnhubIndustry = setFinnhubStock.m_FinnhubIndustry;
	if (setFinnhubStock.m_Peer.GetLength() > 2) {
		CreateJsonWithNlohmann(m_jsonPeer, setFinnhubStock.m_Peer);
	}
	m_lIPOStatus = setFinnhubStock.m_IPOStatus;

	LoadUpdateDate(setFinnhubStock.m_UpdateDate);
}

void CFinnhubStock::CheckUpdateStatus(long lTodayDate) {
	CheckProfileUpdateStatus(lTodayDate);
	CheckBasicFinancialUpdateStatus(lTodayDate);
	CheckCompanyNewsUpdateStatus(lTodayDate);
	CheckDayLineUpdateStatus(lTodayDate, gl_pWorldMarket->GetLastTradeDate(), gl_pWorldMarket->GetMarketTime(), gl_pWorldMarket->GetDayOfWeek());
	CheckEPSSurpriseStatus(lTodayDate);
	CheckSECFilingsStatus(lTodayDate);
	CheckPeerStatus(lTodayDate);
	CheckInsiderTransactionStatus(lTodayDate);
	CheckInsiderSentimentStatus(lTodayDate);
}

void CFinnhubStock::CheckProfileUpdateStatus(long lTodayDate) {
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
bool CFinnhubStock::CheckCompanyNewsUpdateStatus(long lTodayDate) {
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
bool CFinnhubStock::CheckBasicFinancialUpdateStatus(long lTodayDate) {
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
bool CFinnhubStock::CheckDayLineUpdateStatus(long lTodayDate, long lLastTradeDate, long lTime, long lDayOfWeek) {
	ASSERT(IsUpdateDayLine()); // 默认状态为日线数据需要更新
	if (IsNullStock()) {
		SetUpdateDayLine(false);
		return m_fUpdateDayLine;
	}
	if (IsDelisted() || IsNotYetList()) {
		// 摘牌股票?
		if (lDayOfWeek != 4) {
			// 每星期四检查一次
			SetUpdateDayLine(false);
			return m_fUpdateDayLine;
		}
	}
	else if ((!IsNotChecked()) && (IsEarlyThen(GetDayLineEndDate(), gl_pWorldMarket->GetMarketDate(), 100))) {
		SetUpdateDayLine(false);
		return m_fUpdateDayLine;
	}
	else {
		if ((lDayOfWeek > 0) && (lDayOfWeek < 6)) {
			// 周一至周五
			if (lTime > 170000) {
				if (lTodayDate <= GetDayLineEndDate()) {
					// 最新日线数据为今日的数据，而当前时间为下午五时之后
					SetUpdateDayLine(false); // 日线数据不需要更新
					return m_fUpdateDayLine;
				}
			}
			else {
				if (lLastTradeDate <= GetDayLineEndDate()) {
					// 最新日线数据为上一个交易日的数据,而当前时间为下午五时之前。
					SetUpdateDayLine(false); // 日线数据不需要更新
					return m_fUpdateDayLine;
				}
			}
		}
		else if (lLastTradeDate <= GetDayLineEndDate()) {
			// 周六周日时， 最新日线数据为上一个交易日的数据
			SetUpdateDayLine(false); // 日线数据不需要更新
			return m_fUpdateDayLine;
		}
	}
	return m_fUpdateDayLine;
}

void CFinnhubStock::Save(CSetFinnhubStock& setFinnhubStock) const {
	// 由于数据库的格式为定长的字符串，故而需要限制实际字符串的长度。
	setFinnhubStock.m_Symbol = m_strSymbol.Left(20);
	setFinnhubStock.m_ExchangeCode = m_strExchangeCode.Left(3);
	setFinnhubStock.m_Description = m_strDescription.Left(200);
	setFinnhubStock.m_DisplaySymbol = m_strDisplaySymbol.Left(20);
	setFinnhubStock.m_Type = m_strType.Left(25);
	setFinnhubStock.m_Mic = m_strMic.Left(20);
	setFinnhubStock.m_Figi = m_strFigi.Left(20);
	setFinnhubStock.m_Currency = m_strCurrency.Left(20);
	setFinnhubStock.m_Address = m_strAddress.Left(100);
	setFinnhubStock.m_City = m_strCity.Left(20);
	setFinnhubStock.m_Country = m_strCountry.Left(20);
	setFinnhubStock.m_Cusip = m_strCusip.Left(20);
	setFinnhubStock.m_Sedol = m_strSedol.Left(45);
	setFinnhubStock.m_EmployeeTotal = m_lEmployeeTotal;
	//setFinnhubStock.m_ListedExchange = m_strExchangeCode.Left(100);
	setFinnhubStock.m_Ggroup = m_strGgroup.Left(45);
	setFinnhubStock.m_Gind = m_strGind.Left(45);
	setFinnhubStock.m_Gsector = m_strGsector.Left(45);
	setFinnhubStock.m_Gsubind = m_strGsubind.Left(45);
	setFinnhubStock.m_IPODate = m_strIPODate.Left(20);
	setFinnhubStock.m_Isin = m_strIsin.Left(45);
	setFinnhubStock.m_MarketCapitalization = ConvertValueToString(m_dMarketCapitalization);
	setFinnhubStock.m_Naics = m_strNaics.Left(45);
	setFinnhubStock.m_NaicsNationalIndustry = m_strNaicsNationalIndustry.Left(100);
	setFinnhubStock.m_NaicsSector = m_strNaicsSector.Left(100);
	setFinnhubStock.m_NaicsSubsector = m_strNaicsSubsector.Left(100);
	setFinnhubStock.m_Name = m_strName.Left(200);
	setFinnhubStock.m_Phone = m_strPhone.Left(100);
	setFinnhubStock.m_ShareOutstanding = ConvertValueToString(m_dShareOutstanding);
	setFinnhubStock.m_State = m_strState.Left(45);
	setFinnhubStock.m_Ticker = m_strTicker.Left(45);
	setFinnhubStock.m_WebURL = m_strWebURL.Left(150);
	setFinnhubStock.m_Logo = m_strLogo.Left(110);
	setFinnhubStock.m_FinnhubIndustry = m_strFinnhubIndustry.Left(100);
	const string sPeer = m_jsonPeer.dump();
	ASSERT(sPeer.size() < 2000);
	setFinnhubStock.m_Peer = sPeer.c_str();

	const string sUpdateDate = m_jsonUpdateDate.dump();
	setFinnhubStock.m_UpdateDate = sUpdateDate.c_str();
	ASSERT(sUpdateDate.size() < 10000);
	setFinnhubStock.m_IPOStatus = m_lIPOStatus;
}

void CFinnhubStock::Update(CSetFinnhubStock& setFinnhubStock) const {
	setFinnhubStock.Edit();
	Save(setFinnhubStock);
	setFinnhubStock.Update();
}

void CFinnhubStock::Append(CSetFinnhubStock& setFinnhubStock) const {
	setFinnhubStock.AddNew();
	Save(setFinnhubStock);
	setFinnhubStock.Update();
}

void CFinnhubStock::UpdateInsiderTransactionDB() {
	try {
		CSetInsiderTransaction setInsiderTransaction;

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
			if (pInsiderTransaction->m_lTransactionDate > m_lInsiderTransactionEndDate) m_lInsiderTransactionEndDate = pInsiderTransaction->m_lTransactionDate;
			vInsiderTransaction.push_back(pInsiderTransaction);
			lSizeOfOldInsiderTransaction++;
			setInsiderTransaction.MoveNext();
		}

		setInsiderTransaction.m_pDatabase->BeginTrans();
		for (int i = 0; i < m_vInsiderTransaction.size(); i++) {
			pInsiderTransaction = m_vInsiderTransaction.at(i);
			if (pInsiderTransaction->m_lTransactionDate > m_lInsiderTransactionEndDate) {
				pInsiderTransaction->Append(setInsiderTransaction); // 较新的数据直接存储之
			}
			else { // 较旧的数据？
				if (std::ranges::find_if(vInsiderTransaction.begin(), vInsiderTransaction.end(),
				                         [pInsiderTransaction](const CInsiderTransactionPtr& p) {
					                         return ((p->m_strSymbol.Compare(pInsiderTransaction->m_strSymbol) == 0) // 股票代码
						                         && (p->m_lTransactionDate == pInsiderTransaction->m_lTransactionDate) // 交易时间
						                         && (p->m_strPersonName.Compare(pInsiderTransaction->m_strPersonName) == 0) // 内部交易人员
						                         && (p->m_strTransactionCode.Compare(pInsiderTransaction->m_strTransactionCode) == 0)); // 交易细节
				                         }) == vInsiderTransaction.end()) { // 如果没找到，则股票代码、人名、交易日期或者交易细节为新的数据，存储该数据
					pInsiderTransaction->Append(setInsiderTransaction);
				}
			}
		}
		setInsiderTransaction.m_pDatabase->CommitTrans();
		setInsiderTransaction.Close();
	} catch (CException* e) {
		ReportInformationAndDeleteException(e);
	}
}

void CFinnhubStock::UpdateInsiderSentimentDB() {
	try {
		CSetInsiderSentiment setInsiderSentiment;

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
		if (lSizeOfOldInsiderSentiment > 0) {
			if (vInsiderSentiment.at(0)->m_lDate < m_lInsiderSentimentStartDate) {
				m_lInsiderSentimentStartDate = vInsiderSentiment.at(0)->m_lDate;
			}
		}
		setInsiderSentiment.m_pDatabase->BeginTrans();
		for (int i = 0; i < m_vInsiderSentiment.size(); i++) {
			pInsiderSentiment = m_vInsiderSentiment.at(i);
			if (std::ranges::find_if(vInsiderSentiment.begin(), vInsiderSentiment.end(),
			                         [pInsiderSentiment](const CInsiderSentimentPtr& p) {
				                         return (p->m_lDate == pInsiderSentiment->m_lDate); // 报告时间
			                         }) == vInsiderSentiment.end()) { // 如果没找到，则报告日期为新的数据，存储该数据
				pInsiderSentiment->Append(setInsiderSentiment);
			}
		}
		setInsiderSentiment.m_pDatabase->CommitTrans();
		setInsiderSentiment.Close();
	} catch (CException* e) {
		ReportInformationAndDeleteException(e);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 将接收到的新闻存储入数库中。
// 接收到的新闻事先已经按时间顺序存于vector中，只存储数据库中没有的时间点的新闻。
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool CFinnhubStock::UpdateCompanyNewsDB() {
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
			if ((atoll(setCompanyNews.m_DateTime) > pCompanyNews->m_llDateTime)) {	// 没有这个时间点的新闻？
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

bool CFinnhubStock::UpdateEPSSurpriseDB() {
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

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 将接收到的SEC Filings存储入数库中。
// 接收到的事先已经按accessNumber顺序存于vector中，只存储数据库中没有accessNumber的新闻。
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool CFinnhubStock::UpdateSECFilingsDB() const {
	ASSERT(m_pvSECFilings->size() > 0);
	const long lSize = static_cast<long>(m_pvSECFilings->size());
	if (m_strSymbol.GetLength() > 0) {
		CSECFilingPtr pSECFilings;
		CSetSECFilings setSECFilings;
		long lCurrentPos = 0;
		setSECFilings.m_strFilter = _T("[Symbol] = '");
		setSECFilings.m_strFilter += m_strSymbol + _T("'");
		setSECFilings.m_strSort = _T("[accessNumber]");

		setSECFilings.Open();
		setSECFilings.m_pDatabase->BeginTrans();
		while (!setSECFilings.IsEOF()) {
			pSECFilings = m_pvSECFilings->at(lCurrentPos);
			while (!setSECFilings.IsEOF() && setSECFilings.m_AccessNumber.Compare(pSECFilings->m_strAccessNumber) < 0) setSECFilings.MoveNext();
			if (setSECFilings.IsEOF()) break;
			if (setSECFilings.m_AccessNumber.Compare(pSECFilings->m_strAccessNumber) > 0) {	// 没有这个AccessNumber的SEC Filings？
				pSECFilings->Append(setSECFilings);
			}
			if (++lCurrentPos == lSize) break;
		}
		for (long i = lCurrentPos; i < lSize; i++) {
			pSECFilings = m_pvSECFilings->at(i);
			pSECFilings->Append(setSECFilings);
		}
		setSECFilings.m_pDatabase->CommitTrans();
		setSECFilings.Close();
	}
	return true;
}

bool CFinnhubStock::UpdateDayLineDB() {
	if (IsUpdateDayLineDBAndClearFlag()) {
		// 清除标识需要与检测标识处于同一原子过程中，防止同步问题出现
		if (GetDayLineSize() > 0) {
			if (HaveNewDayLineData()) {
				SaveDayLineDB();
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

void CFinnhubStock::AppendBasicFinancialAnnual() const {
	try {
		CSetFinnhubStockBasicFinancialAnnual setAnnual;
		setAnnual.m_strFilter = _T("[ID] = 1");
		setAnnual.Open();
		setAnnual.m_pDatabase->BeginTrans();
		m_pBasicFinancial->AppendAnnualData(setAnnual);
		setAnnual.m_pDatabase->CommitTrans();
		setAnnual.Close();
	} catch (CException* e) {
		ReportInformationAndDeleteException(e);
	}
}

void CFinnhubStock::AppendBasicFinancialQuarter() const {
	try {
		CSetFinnhubStockBasicFinancialQuarter setQuarter;

		setQuarter.m_strFilter = _T("[ID] = 1");
		setQuarter.Open();
		setQuarter.m_pDatabase->BeginTrans();
		m_pBasicFinancial->AppendQuarterData(setQuarter);
		setQuarter.m_pDatabase->CommitTrans();
		setQuarter.Close();
	} catch (CException* e) {
		ReportInformationAndDeleteException(e);
	}
}

void CFinnhubStock::UpdateCompanyNews(const CCompanyNewssPtr& pvCompanyNews) {
	m_vCompanyNews.resize(0);
	for (auto& p : *pvCompanyNews) {
		m_vCompanyNews.push_back(p);
	}
	std::ranges::sort(m_vCompanyNews, [](const CCompanyNewsPtr& p1, const CCompanyNewsPtr& p2) { return (p1->m_llDateTime < p2->m_llDateTime); }); // 此序列需要按时间顺序存放，以利于与存储于数据库中的数据作比较。
}

void CFinnhubStock::UpdateEPSSurprise(const vector<CEPSSurprisePtr>& vEPSSurprise) {
	m_vEPSSurprise.resize(0);
	for (auto& p : vEPSSurprise) {
		m_vEPSSurprise.push_back(p);
	}
}

void CFinnhubStock::UpdateDayLineStartEndDate() {
	long lStartDate = 0, lEndDate = 0;
	if (m_dataDayLine.GetStartEndDate(lStartDate, lEndDate)) {
		if (lStartDate < GetDayLineStartDate()) {
			SetDayLineStartDate(lStartDate);
			m_fUpdateProfileDB = true;
		}
		if (lEndDate > GetDayLineEndDate()) {
			SetDayLineEndDate(lEndDate);
			m_fUpdateProfileDB = true;
		}
	}
}

bool CFinnhubStock::HaveNewDayLineData() {
	if (m_dataDayLine.Empty()) return false;
	if ((m_dataDayLine.GetData(m_dataDayLine.Size() - 1)->GetMarketDate() > GetDayLineEndDate())
		|| (m_dataDayLine.GetData(0)->GetMarketDate() < GetDayLineStartDate()))
		return true;
	return false;
}

bool CFinnhubStock::UpdateBasicFinancial(const CFinnhubStockBasicFinancialPtr& pFinnhubStockBasicFinancial) {
	m_pBasicFinancial = pFinnhubStockBasicFinancial;

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 默认每90天更新一次，已经900天没更新的即不再更新。
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CFinnhubStock::CheckEPSSurpriseStatus(long lCurrentDate) {
	const long lLastEPSSurpriseUpdateDate = GetLastEPSSurpriseUpdateDate();
	if (IsNullStock() || IsDelisted()) {
		m_fUpdateEPSSurprise = false;
	}
	else if ((lLastEPSSurpriseUpdateDate == 19700101) || (lLastEPSSurpriseUpdateDate == 19800101)) { // 没有数据？
		m_fUpdateEPSSurprise = false;
	}
	else if (IsEarlyThen(lLastEPSSurpriseUpdateDate, lCurrentDate, gl_systemConfiguration.GetEPSSurpriseUpdateRate() * 10) && (lLastEPSSurpriseUpdateDate != 19800101)) {// 有早于900天的数据？即已经不更新了
		m_fUpdateEPSSurprise = false;
	}
	else if (!IsEarlyThen(lLastEPSSurpriseUpdateDate, lCurrentDate, gl_systemConfiguration.GetEPSSurpriseUpdateRate())) {	// 有不早于90天的数据？
		m_fUpdateEPSSurprise = false;
	}
	else {
		m_fUpdateEPSSurprise = true;
	}
	return m_fUpdateEPSSurprise;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 默认每30天更新一次.
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CFinnhubStock::CheckSECFilingsStatus(long lCurrentDate) {
	const long lSECFilingsUpdateDate = GetSECFilingsUpdateDate();
	if (IsNullStock() || IsDelisted()) {
		m_fSECFilingsUpdated = true;
	}
	else if (!IsEarlyThen(lSECFilingsUpdateDate, lCurrentDate, gl_systemConfiguration.GetSECFilingsUpdateRate())) {	// 有不早于30天的数据？
		m_fSECFilingsUpdated = true;
	}
	else {
		m_fSECFilingsUpdated = false;
	}
	return m_fSECFilingsUpdated;
}

bool CFinnhubStock::CheckPeerStatus(long lCurrentDate) {
	if (IsNullStock() || IsDelisted()) {
		m_fUpdateFinnhubPeer = false;
	}
	else if (!IsEarlyThen(GetPeerUpdateDate(), lCurrentDate, gl_systemConfiguration.GetStockPeerUpdateRate())) {// 有不早于90天的数据？
		m_fUpdateFinnhubPeer = false;
	}
	else {
		m_fUpdateFinnhubPeer = true;
	}
	return m_fUpdateFinnhubPeer;
}

void CFinnhubStock::UpdateInsiderTransaction(const vector<CInsiderTransactionPtr>& vInsiderTransaction) {
	m_vInsiderTransaction.resize(0);

	for (auto pInsiderTransaction : vInsiderTransaction) {
		m_vInsiderTransaction.push_back(pInsiderTransaction);
	}
}

bool CFinnhubStock::CheckInsiderTransactionStatus(long lCurrentDate) {
	if (!IsUSMarket()) {
		m_fUpdateFinnhubInsiderTransaction = false;
	}
	else if (IsNullStock() || IsDelisted()) {
		m_fUpdateFinnhubInsiderTransaction = false;
	}
	else if (!IsEarlyThen(GetInsiderTransactionUpdateDate(), lCurrentDate, gl_systemConfiguration.GetInsideTransactionUpdateRate())) {// 有不早于30天的数据？
		m_fUpdateFinnhubInsiderTransaction = false;
	}
	else {
		m_fUpdateFinnhubInsiderTransaction = true;
	}
	return m_fUpdateFinnhubInsiderTransaction;
}

void CFinnhubStock::UpdateInsiderSentiment(const vector<CInsiderSentimentPtr>& vInsiderSentiment) {
	m_vInsiderSentiment.resize(0);

	for (auto pInsiderSentiment : vInsiderSentiment) {
		m_vInsiderSentiment.push_back(pInsiderSentiment);
	}
}

bool CFinnhubStock::CheckInsiderSentimentStatus(long lCurrentDate) {
	if (!IsUSMarket()) {
		m_fUpdateFinnhubInsiderSentiment = false;
	}
	else if (IsNullStock() || IsDelisted()) {
		m_fUpdateFinnhubInsiderSentiment = false;
	}
	else if (!IsEarlyThen(GetInsiderSentimentUpdateDate(), lCurrentDate, gl_systemConfiguration.GetInsideSentimentUpdateRate())) {// 有不早于30天的数据？
		m_fUpdateFinnhubInsiderSentiment = false;
	}
	else {
		m_fUpdateFinnhubInsiderSentiment = true;
	}
	return m_fUpdateFinnhubInsiderSentiment;
}

long CFinnhubStock::GetProfileUpdateDate() {
	try {
		const long lDate = m_jsonUpdateDate.at(_T("Finnhub")).at("StockFundamentalsCompanyProfileConcise");
		return lDate;
	} catch (nlohmann::ordered_json::exception&) {
		return 19800101;
	}
}

void CFinnhubStock::SetProfileUpdateDate(const long lProfileUpdateDate) noexcept {
	m_jsonUpdateDate["Finnhub"]["StockFundamentalsCompanyProfileConcise"] = lProfileUpdateDate;
}

long CFinnhubStock::GetCompanyNewsUpdateDate() {
	try {
		const long lDate = m_jsonUpdateDate.at(_T("Finnhub")).at("StockFundamentalsCompanyNews");
		return lDate;
	} catch (nlohmann::ordered_json::exception&) {
		return 19800101;
	}
}

void CFinnhubStock::SetCompanyNewsUpdateDate(const long lCompanyNewsUpdateDate) noexcept {
	m_jsonUpdateDate["Finnhub"]["StockFundamentalsCompanyNews"] = lCompanyNewsUpdateDate;
}

long CFinnhubStock::GetBasicFinancialUpdateDate() {
	try {
		const long lDate = m_jsonUpdateDate.at(_T("Finnhub")).at("StockFundamentalsBasicFinancials");
		return lDate;
	} catch (nlohmann::ordered_json::exception&) {
		return 19800101;
	}
}

void CFinnhubStock::SetBasicFinancialUpdateDate(const long lBasicFinancialUpdateDate) noexcept {
	m_jsonUpdateDate["Finnhub"]["StockFundamentalsBasicFinancials"] = lBasicFinancialUpdateDate;
}

long CFinnhubStock::GetLastRTDataUpdateDate() {
	try {
		const long lDate = m_jsonUpdateDate.at(_T("Finnhub")).at("StockPriceQuote");
		return lDate;
	} catch (nlohmann::ordered_json::exception&) {
		return 19800101;
	}
}

void CFinnhubStock::SetLastRTDataUpdateDate(const long lDate) noexcept {
	m_jsonUpdateDate["Finnhub"]["StockPriceQuote"] = lDate;
}

long CFinnhubStock::GetPeerUpdateDate() {
	try {
		const long lDate = m_jsonUpdateDate.at(_T("Finnhub")).at("StockFundamentalsPeer");
		return lDate;
	} catch (nlohmann::ordered_json::exception&) {
		return 19800101;
	}
}

void CFinnhubStock::SetPeerUpdateDate(const long lDate) noexcept {
	m_jsonUpdateDate["Finnhub"]["StockFundamentalsPeer"] = lDate;
}

long CFinnhubStock::GetInsiderTransactionUpdateDate() {
	try {
		const long lDate = m_jsonUpdateDate.at(_T("Finnhub")).at("StockFundamentalsInsiderTransaction");
		return lDate;
	} catch (nlohmann::ordered_json::exception&) {
		return 19800101;
	}
}

void CFinnhubStock::SetInsiderTransactionUpdateDate(const long lDate) noexcept {
	m_jsonUpdateDate["Finnhub"]["StockFundamentalsInsiderTransaction"] = lDate;
}

long CFinnhubStock::GetInsiderSentimentUpdateDate() {
	try {
		const long lDate = m_jsonUpdateDate.at(_T("Finnhub")).at("StockFundamentalsInsiderSentiment");
		return lDate;
	} catch (nlohmann::ordered_json::exception&) {
		return 19800101;
	}
}

void CFinnhubStock::SetInsiderSentimentUpdateDate(const long lDate) noexcept {
	m_jsonUpdateDate["Finnhub"]["StockFundamentalsInsiderSentiment"] = lDate;
}

long CFinnhubStock::GetLastEPSSurpriseUpdateDate() {
	try {
		const long lDate = m_jsonUpdateDate.at(_T("Finnhub")).at("StockEstimatesEPSSurprise");
		return lDate;
	} catch (nlohmann::ordered_json::exception&) {
		return 19800101;
	}
}

void CFinnhubStock::SetLastEPSSurpriseUpdateDate(const long lDate) noexcept {
	m_jsonUpdateDate["Finnhub"]["StockEstimatesEPSSurprise"] = lDate;
}

long CFinnhubStock::GetSECFilingsUpdateDate() {
	try {
		const long lDate = m_jsonUpdateDate.at(_T("Finnhub")).at("StockFundamentalsSECFilings");
		return lDate;
	} catch (nlohmann::ordered_json::exception&) {
		return 19800101;
	}
}

void CFinnhubStock::SetSECFilingsUpdateDate(const long lDate) noexcept {
	m_jsonUpdateDate["Finnhub"]["StockFundamentalsSECFilings"] = lDate;
}

CString CFinnhubStock::GetFinnhubDayLineInquiryParam(time_t tCurrentTime) const {
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

CString CFinnhubStock::GetTiingoDayLineInquiryParam(long lStartDate, long lCurrentDate) const {
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

CString CFinnhubStock::GetFinnhubInsiderTransactionInquiryParam(time_t tCurrentTime) {
	CString strParam = _T("");
	char buffer[50];

	strParam += m_strSymbol;
	strParam += _T("&from=");
	sprintf_s(buffer, _T("%i"), GetInsiderTransactionUpdateDate());
	const CString strTemp = buffer;
	strParam += strTemp;

	return strParam;
}

bool CFinnhubStock::IsUSMarket() const {
	if (m_strExchangeCode.Compare(_T("US")) == 0) return true;
	return false;
}
