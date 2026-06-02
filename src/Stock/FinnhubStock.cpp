#include"pch.h"

#include"TimeConvert.h"
#include"InsiderSentiment.h"
#include"InsiderTransaction.h"
#include"EPSSurprise.h"

#include "FinnhubStock.h"

#include "dataBaseConnector.h"
#include"WorldMarket.h"

#include"FinnhubCompanyNews.h"
#include "InfoReport.h"

CFinnhubStock::CFinnhubStock() {
	SetExchange("US");
	CFinnhubStock::ResetAllUpdateDate();
}

CFinnhubStock::~CFinnhubStock() = default;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// 所有更新日期的json数据皆在此预设。这样保证提取数据时不会发生exception，如果发生了exception,则程序出现错误。
/// 可以使用exception判断参数名称的正确与否。
///
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void CFinnhubStock::ResetAllUpdateDate() {
	m_jsonUpdateDate["DayLineStartDate"] = m_lDayLineStartDate = 29900101;
	m_jsonUpdateDate["DayLineEndDate"] = m_lDayLineEndDate = 19800101;
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
	if (m_dShareOutstanding > 0 && m_dMarketCapitalization > 0) {
		if (!IsEarlyThen(GetCompanyNewsUpdateDate(), lTodayDate, 6)) {
			// 每星期更新一次公司新闻
			m_fUpdateCompanyNews = false;
		}
		else m_fUpdateCompanyNews = true;
	}
	else {
		// 未上市股票或无市值股票，每月更新一次公司新闻
		if (!IsEarlyThen(GetCompanyNewsUpdateDate(), lTodayDate, 30)) {
			m_fUpdateCompanyNews = false;
		}
		else m_fUpdateCompanyNews = true;
	}

	return m_fUpdateCompanyNews;
}

/// <summary>
/// 系统每季更新一次数据，故查询两次即可满足。所以设定45天查询一次
/// </summary>
/// <param name="lTodayDate"></param>
/// <returns></returns>
bool CFinnhubStock::CheckBasicFinancialUpdateStatus(long lTodayDate) {
	ASSERT(m_fUpdateBasicFinancial);
	if (m_dShareOutstanding > 0 && m_dMarketCapitalization > 0) {
		if (IsEarlyThen(GetBasicFinancialUpdateDate(), lTodayDate, gl_systemConfiguration.GetStockBasicFinancialUpdateRate())) {
			// 系统每季更新一次数据，故查询两次即可。
			m_fUpdateBasicFinancial = true;
		}
		else { m_fUpdateBasicFinancial = false; }
	}
	else {
		// 未上市股票或无市值股票，每半年更新一次数据，故查询四次即可。
		if (IsEarlyThen(GetBasicFinancialUpdateDate(), lTodayDate, gl_systemConfiguration.GetStockBasicFinancialUpdateRate() * 2)) {
			m_fUpdateBasicFinancial = true;
		}
		else { m_fUpdateBasicFinancial = false; }
	}

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

void CFinnhubStock::UpdateInsiderTransactionDB() {
	try {
		using namespace StockMarket;
		const auto& t = FinnhubInsiderTransaction{};
		{
			auto db = gl_dbStockMarket.get();
			auto tx = sqlpp::start_transaction(db);

			{
				auto result = db(select(all_of(t)).from(t).order_by(t.TransactionDate.desc()).where(t.Symbol == m_strSymbol.c_str()));
				auto rows = result.size();
				if (rows == 0) {
					m_lInsiderTransactionEndDate = 19800101;
				}
				else {
					auto& row = result.front();
					m_lInsiderTransactionEndDate = row.TransactionDate; // 倒序排序，最新的日期位于第一个。
				}
			}
			tx.commit();
		}
		CInsiderTransaction insiderTransaction;
		auto db = gl_dbStockMarket.get();
		auto tx = sqlpp::start_transaction(db);
		auto multi_insert = insert_into(t).columns(t.Symbol, t.PersonName, t.Share, t.FilingDate, t.TransactionDate,
		                                           t.TransactionCode, t.ShareChange, t.TransactionPrice);

		int nValues = 0;
		for (size_t i = 0; i < m_vInsiderTransaction.size(); i++) {
			insiderTransaction = m_vInsiderTransaction.at(i);
			if (insiderTransaction.m_lTransactionDate > m_lInsiderTransactionEndDate) {
				multi_insert.values.add(
					t.Symbol = insiderTransaction.m_strSymbol,
					t.PersonName = insiderTransaction.m_strPersonName, // 人名最多100个字符
					t.Share = static_cast<double>(insiderTransaction.m_lShare), // 交易股数有可能超过int的范围，故而使用INT64。
					t.FilingDate = insiderTransaction.m_lFilingDate,
					t.TransactionDate = insiderTransaction.m_lTransactionDate,
					t.TransactionCode = insiderTransaction.m_strTransactionCode, // 交易代码最多4个字符
					t.ShareChange = static_cast<double>(insiderTransaction.m_lShareChange),// 交易股数有可能超过int的范围，故而使用INT64。
					t.TransactionPrice = insiderTransaction.m_dTransactionPrice
				);
				nValues++;
			}
			else {
				auto result2 = db(select(all_of(t)).from(t).where(
					t.Symbol == insiderTransaction.m_strSymbol.c_str() // 股票代码
					&& t.TransactionDate == insiderTransaction.m_lTransactionDate // 交易时间
					&& t.PersonName == insiderTransaction.m_strPersonName.c_str() // 内部交易人员
					&& t.TransactionCode == insiderTransaction.m_strTransactionCode.c_str()));
				auto rows2 = result2.size();
				if (rows2 == 0) {
					multi_insert.values.add(
						t.Symbol = insiderTransaction.m_strSymbol,
						t.PersonName = insiderTransaction.m_strPersonName, // 人名最多100个字符
						t.Share = static_cast<double>(insiderTransaction.m_lShare), // 交易股数有可能超过int的范围，故而使用INT64。
						t.FilingDate = insiderTransaction.m_lFilingDate,
						t.TransactionDate = insiderTransaction.m_lTransactionDate,
						t.TransactionCode = insiderTransaction.m_strTransactionCode, // 交易代码最多4个字符
						t.ShareChange = static_cast<double>(insiderTransaction.m_lShareChange),// 交易股数有可能超过int的范围，故而使用INT64。
						t.TransactionPrice = insiderTransaction.m_dTransactionPrice
					);
					nValues++;
				}
			}
		}
		if (nValues > 0) {
			db(multi_insert);
		}
		tx.commit();
	} catch (CException& e) {
		ReportInformation(e);
	}
}

void CFinnhubStock::UpdateInsiderSentimentDB() {
	//Todo:用sqlpp11重写该函数，使用sqlpp11的事务机制和批量插入功能来优化性能。可以先查询数据库中最新的报告日期，然后只插入那些报告日期晚于数据库中最新日期的数据。这样可以避免不必要的查询和插入操作，提高效率。
	/*
	 
	try {
		CSetInsiderSentiment setInsiderSentiment;

		vector<CInsiderSentiment> vInsiderSentiment;
		CInsiderSentiment insiderSentiment;
		long lSizeOfOldInsiderSentiment = 0;

		setInsiderSentiment.m_strFilter = "[Symbol] = '";
		setInsiderSentiment.m_strFilter += m_strSymbol.c_str();
		setInsiderSentiment.m_strFilter += "'";
		setInsiderSentiment.m_strSort = "[Date]";

		setInsiderSentiment.Open();
		while (!setInsiderSentiment.IsEOF()) {
			insiderSentiment.Load(setInsiderSentiment);
			vInsiderSentiment.push_back(insiderSentiment);
			lSizeOfOldInsiderSentiment++;
			setInsiderSentiment.MoveNext();
		}
		if (lSizeOfOldInsiderSentiment > 0) {
			m_lInsiderSentimentStartDate = min(vInsiderSentiment.at(0).m_lDate, m_lInsiderSentimentStartDate);
		}
		setInsiderSentiment.m_pDatabase->BeginTrans();
		for (size_t i = 0; i < m_pvInsiderSentiment->size(); i++) {
			insiderSentiment = m_pvInsiderSentiment->at(i);
			if (std::ranges::find_if(vInsiderSentiment.begin(), vInsiderSentiment.end(),
			                         [insiderSentiment](const CInsiderSentiment& p) {
				                         return (p.m_lDate == insiderSentiment.m_lDate); // 报告时间
			                         }) == vInsiderSentiment.end()) { // 如果没找到，则报告日期为新的数据，存储该数据
				insiderSentiment.Append(setInsiderSentiment);
			}
		}
		setInsiderSentiment.m_pDatabase->CommitTrans();
		setInsiderSentiment.Close();
	} catch (CException& e) {
		ReportInformation(e);
	}
	*/
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 将接收到的新闻存储入数库中。
// 接收到的新闻事先已经按时间顺序存于vector中，只存储数据库中没有的时间点的新闻。
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool CFinnhubStock::UpdateCompanyNewsDB() {
	ASSERT(!m_vCompanyNews.empty());
	const long lSize = static_cast<long>(m_vCompanyNews.size());
	//Note: 可以优化为二分法查找，找到第一个大于数据库中最新日期的新闻的索引位置，然后从该位置开始存储数据。
	//Todo: 可以优化为批量存储数据，而不是一条一条地存储数据。

	long long lCutoffDateTime = 0;
	using namespace StockMarket;
	const auto& t = FinnhubCompanyNews{};
	auto db = gl_dbStockMarket.get();
	db.start_transaction();
	auto multi_insert = insert_into(t).columns(t.Symbol, t.Category, t.DateTime, t.Headline, t.NewsID,
	                                           t.Image, t.RelatedSymbol, t.Source, t.Summary, t.URL);

	auto result = db(select(all_of(t)).from(t).unconditionally().order_by(t.DateTime.desc()));
	size_t rows = result.size();
	if (rows > 0) {
		auto& row = result.front();
		lCutoffDateTime = row.DateTime.value();
	}

	size_t iIndex = 0;
	while (m_vCompanyNews.at(iIndex).m_llDateTime <= lCutoffDateTime) iIndex++;

	for (size_t i = iIndex; i < m_vCompanyNews.size(); i++) {
		auto& companyNews = m_vCompanyNews.at(i);
		multi_insert.values.add(
			t.Symbol = companyNews.m_strCompanySymbol,
			t.Category = companyNews.m_strCategory,
			t.DateTime = static_cast<double>(companyNews.m_llDateTime),
			t.Headline = companyNews.m_strHeadLine,
			t.NewsID = companyNews.m_iNewsID,
			t.Image = companyNews.m_strImage,
			t.RelatedSymbol = companyNews.m_strRelatedSymbol,
			t.Source = companyNews.m_strSource,
			t.Summary = companyNews.m_strSummary,
			t.URL = companyNews.m_strURL
		);
	}
	db.commit_transaction();
	return true;
}

bool CFinnhubStock::UpdateEPSSurpriseDB() {
	const long lLastEPSSurpriseUpdateDate = GetLastEPSSurpriseUpdateDate();

	if (m_vEPSSurprise.empty()) return true;
	if (m_vEPSSurprise.at(m_vEPSSurprise.size() - 1).m_lDate > lLastEPSSurpriseUpdateDate) { SetUpdateProfileDB(true); }
	else return false; // 没有新数据则返回

	using namespace StockMarket;
	const auto& t = FinnhubStockEstimatesEpsSurprise{};
	auto db = gl_dbStockMarket.get();
	auto tx = sqlpp::start_transaction(db);
	auto multi_insert = insert_into(t).columns(t.Symbol, t.Date, t.Actual, t.Estimate);
	for (const auto& EPSSurprise : m_vEPSSurprise) {
		if (EPSSurprise.m_lDate > lLastEPSSurpriseUpdateDate) {
			multi_insert.values.add(
				t.Symbol = EPSSurprise.m_strSymbol,
				t.Date = EPSSurprise.m_lDate,
				t.Actual = EPSSurprise.m_dActual,
				t.Estimate = EPSSurprise.m_dEstimate
			);
		}
	}
	if (m_vEPSSurprise.size() > 0) {
		db(multi_insert);
	}
	tx.commit();
	SetLastEPSSurpriseUpdateDate(m_vEPSSurprise.at(m_vEPSSurprise.size() - 1).m_lDate);

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 将接收到的SEC Filings存储入数库中。
// 接收到的事先已经按accessNumber顺序存于vector中，只存储数据库中没有accessNumber的新闻。
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool CFinnhubStock::UpdateSECFilingsDB() const {
	const long lSize = static_cast<long>(m_vSECFilings.size());
	if (!m_strSymbol.empty()) {
		long lCurrentPos = 0;
		CSECFiling SECFilings;
		using namespace StockMarket;
		const auto& t = FinnhubStockSecFilings{};
		auto db = gl_dbStockMarket.get();
		auto tx = sqlpp::start_transaction(db);
		auto multi_insert = insert_into(t).columns(t.symbol, t.accessNumber, t.cik, t.filedDate,
		                                           t.acceptedDate, t.filingURL, t.reportURL, t.form);

		auto result = db(select(all_of(t)).from(t).where(t.symbol == m_strSymbol.c_str()).order_by(t.accessNumber.asc()));

		for (const auto& row : result) {
			SECFilings = m_vSECFilings.at(lCurrentPos);
			if (SECFilings.m_strAccessNumber.compare(row.accessNumber.value()) > 0) continue;
			if (SECFilings.m_strAccessNumber.compare(row.accessNumber.value()) < 0) {	// 没有这个AccessNumber的SEC Filings？
				multi_insert.values.add(
					t.symbol = m_strSymbol,
					t.accessNumber = SECFilings.m_strAccessNumber,
					t.cik = SECFilings.m_iCIK,
					t.filedDate = SECFilings.m_iFiledDate,
					t.acceptedDate = SECFilings.m_iAcceptedDate,
					t.filingURL = SECFilings.m_strFilingURL,
					t.reportURL = SECFilings.m_strReportURL,
					t.form = SECFilings.m_strForm
				);
			}
			if (++lCurrentPos == lSize) break;
		}
		for (long i = lCurrentPos; i < lSize; i++) {
			SECFilings = m_vSECFilings.at(i);
			multi_insert.values.add(
				t.symbol = m_strSymbol,
				t.accessNumber = SECFilings.m_strAccessNumber,
				t.cik = SECFilings.m_iCIK,
				t.filedDate = SECFilings.m_iFiledDate,
				t.acceptedDate = SECFilings.m_iAcceptedDate,
				t.filingURL = SECFilings.m_strFilingURL,
				t.reportURL = SECFilings.m_strReportURL,
				t.form = SECFilings.m_strForm
			);
		}
		if (m_vSECFilings.size() > 0) {
			db(multi_insert);
		}
		tx.commit();
	}
	return true;
}

bool CFinnhubStock::UpdateDayLineDB() {
	if (IsUpdateDayLineDB()) {
		SetUpdateDayLineDB(false);
		if (GetDayLineSize() > 0) {
			if (HaveNewDayLineData()) {
				SaveDayLineDB();
				UpdateDayLineStartEndDate();
				SetUpdateProfileDB(true);
				string str = GetSymbol();
				str += "日线资料存储完成";
				gl_systemMessage.PushDayLineInfoMessage(str);
				//TRACE(_T("更新%s日线数据\n"), GetSymbol().GetBuffer());
				UnloadDayLine();
				return true;
			}
			UnloadDayLine();
		}
	}
	return false;
}

void CFinnhubStock::UpdateCompanyNews(const CCompanyNewssPtr& pvCompanyNews) {
	m_vCompanyNews.reserve(pvCompanyNews->size());
	for (auto& p : *pvCompanyNews) {
		m_vCompanyNews.push_back(p);
	}
	std::ranges::sort(m_vCompanyNews, [](const CFinnhubCompanyNews& p1, const CFinnhubCompanyNews& p2) { return (p1.m_llDateTime < p2.m_llDateTime); }); // 此序列需要按时间顺序存放，以利于与存储于数据库中的数据作比较。
}

void CFinnhubStock::UpdateEPSSurprise(const CEPSSurprisesPtr& pvEPSSurprise) {
	m_vEPSSurprise.resize(0);
	m_vEPSSurprise.reserve(500);

	for (auto& p : *pvEPSSurprise) {
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
	if ((m_dataDayLine.GetData(m_dataDayLine.Size() - 1)->GetDate() > GetDayLineEndDate())
		|| (m_dataDayLine.GetData(0)->GetDate() < GetDayLineStartDate()))
		return true;
	return false;
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
		m_fUpdateSECFilings = false;
	}
	else if (!IsEarlyThen(lSECFilingsUpdateDate, lCurrentDate, gl_systemConfiguration.GetSECFilingsUpdateRate())) {	// 有不早于30天的数据？
		m_fUpdateSECFilings = false;
	}
	else {
		m_fUpdateSECFilings = true;
	}
	return m_fUpdateSECFilings;
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

void CFinnhubStock::UpdateInsiderTransaction(const CInsiderTransactionsPtr& pvInsiderTransaction) {
	m_vInsiderTransaction.resize(0);
	m_vInsiderTransaction.reserve(500);

	for (const auto& insiderTransaction : *pvInsiderTransaction) {
		m_vInsiderTransaction.push_back(insiderTransaction);
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

void CFinnhubStock::UpdateInsiderSentiment(const CInsiderSentimentsPtr& pvInsiderSentiment) {
	m_pvInsiderSentiment = pvInsiderSentiment;
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

void CFinnhubStock::SetSECFilings(const CSECFilingsPtr& pv) {
	m_vSECFilings.resize(0);
	m_vSECFilings.reserve(100);

	for (const auto& secFiling : *pv) {
		m_vSECFilings.push_back(secFiling);
	}
}

long CFinnhubStock::GetProfileUpdateDate() {
	try {
		const long lDate = m_jsonUpdateDate.at("Finnhub").at("StockFundamentalsCompanyProfileConcise");
		return lDate;
	} catch (nlohmannJson::exception&) {
		return 19800101;
	}
}

void CFinnhubStock::SetProfileUpdateDate(const long lProfileUpdateDate) noexcept {
	m_jsonUpdateDate["Finnhub"]["StockFundamentalsCompanyProfileConcise"] = lProfileUpdateDate;
}

long CFinnhubStock::GetCompanyNewsUpdateDate() {
	try {
		const long lDate = m_jsonUpdateDate.at("Finnhub").at("StockFundamentalsCompanyNews");
		return lDate;
	} catch (nlohmannJson::exception&) {
		return 19800101;
	}
}

void CFinnhubStock::SetCompanyNewsUpdateDate(const long lCompanyNewsUpdateDate) noexcept {
	m_jsonUpdateDate["Finnhub"]["StockFundamentalsCompanyNews"] = lCompanyNewsUpdateDate;
}

long CFinnhubStock::GetBasicFinancialUpdateDate() {
	try {
		const long lDate = m_jsonUpdateDate.at("Finnhub").at("StockFundamentalsBasicFinancials");
		return lDate;
	} catch (nlohmannJson::exception&) {
		return 19800101;
	}
}

void CFinnhubStock::SetBasicFinancialUpdateDate(const long lBasicFinancialUpdateDate) noexcept {
	m_jsonUpdateDate["Finnhub"]["StockFundamentalsBasicFinancials"] = lBasicFinancialUpdateDate;
}

long CFinnhubStock::GetLastRTDataUpdateDate() {
	try {
		const long lDate = m_jsonUpdateDate.at("Finnhub").at("StockPriceQuote");
		return lDate;
	} catch (nlohmannJson::exception&) {
		return 19800101;
	}
}

void CFinnhubStock::SetLastRTDataUpdateDate(const long lDate) noexcept {
	m_jsonUpdateDate["Finnhub"]["StockPriceQuote"] = lDate;
}

long CFinnhubStock::GetPeerUpdateDate() {
	try {
		const long lDate = m_jsonUpdateDate.at("Finnhub").at("StockFundamentalsPeer");
		return lDate;
	} catch (nlohmannJson::exception&) {
		return 19800101;
	}
}

void CFinnhubStock::SetPeerUpdateDate(const long lDate) noexcept {
	m_jsonUpdateDate["Finnhub"]["StockFundamentalsPeer"] = lDate;
}

long CFinnhubStock::GetInsiderTransactionUpdateDate() {
	try {
		const long lDate = m_jsonUpdateDate.at("Finnhub").at("StockFundamentalsInsiderTransaction");
		return lDate;
	} catch (nlohmannJson::exception&) {
		return 19800101;
	}
}

void CFinnhubStock::SetInsiderTransactionUpdateDate(const long lDate) noexcept {
	m_jsonUpdateDate["Finnhub"]["StockFundamentalsInsiderTransaction"] = lDate;
}

long CFinnhubStock::GetInsiderSentimentUpdateDate() {
	try {
		const long lDate = m_jsonUpdateDate.at("Finnhub").at("StockFundamentalsInsiderSentiment");
		return lDate;
	} catch (nlohmannJson::exception&) {
		return 19800101;
	}
}

void CFinnhubStock::SetInsiderSentimentUpdateDate(const long lDate) noexcept {
	m_jsonUpdateDate["Finnhub"]["StockFundamentalsInsiderSentiment"] = lDate;
}

long CFinnhubStock::GetLastEPSSurpriseUpdateDate() {
	try {
		const long lDate = m_jsonUpdateDate.at("Finnhub").at("StockEstimatesEPSSurprise");
		return lDate;
	} catch (nlohmannJson::exception&) {
		return 19800101;
	}
}

void CFinnhubStock::SetLastEPSSurpriseUpdateDate(const long lDate) noexcept {
	m_jsonUpdateDate["Finnhub"]["StockEstimatesEPSSurprise"] = lDate;
}

long CFinnhubStock::GetSECFilingsUpdateDate() {
	try {
		const long lDate = m_jsonUpdateDate.at("Finnhub").at("StockFundamentalsSECFilings");
		return lDate;
	} catch (nlohmannJson::exception&) {
		return 19800101;
	}
}

void CFinnhubStock::SetSECFilingsUpdateDate(const long lDate) noexcept {
	m_jsonUpdateDate["Finnhub"]["StockFundamentalsSECFilings"] = lDate;
}

string CFinnhubStock::GetFinnhubDayLineInquiryParam(time_t tCurrentTime) const {
	const time_t tStartTime = (tCurrentTime - static_cast<time_t>(365) * 24 * 3600); // 检查最近一年的数据

	string sParam = fmt::format("{}&resolution=D&from={:Ld}&to={:Ld}", m_strSymbol, tStartTime, tCurrentTime);

	return sParam;
}

string CFinnhubStock::GetTiingoDayLineInquiryParam(long lStartDate, long lCurrentDate) const {
	const long year = lCurrentDate / 10000;
	const long month = lCurrentDate / 100 - year * 100;
	const long date = lCurrentDate - year * 10000 - month * 100;

	const long yearStart = lStartDate / 10000;
	const long monthStart = lStartDate / 100 - yearStart * 100;
	const long dateStart = lStartDate - yearStart * 10000 - monthStart * 100;

	string sParam = fmt::format("{}/prices?&startDate={:4Ld}-{:Ld}-{:Ld}&endDate={:4Ld}-{:Ld}-{:Ld}", m_strSymbol, yearStart, monthStart, dateStart, year, month, date);

	return sParam;
}

string CFinnhubStock::GetFinnhubInsiderTransactionInquiryParam(time_t tCurrentTime) {
	string sParam = fmt::format("{}&from={:Ld}", m_strSymbol, GetInsiderTransactionUpdateDate());
	return sParam;
}

bool CFinnhubStock::IsUSMarket() const {
	if (m_strExchange == "US") return true;
	return false;
}
