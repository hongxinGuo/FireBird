#include"pch.h"

#include"TiingoStock.h"

#include "TiingoStockDailyMeta.h"
#include "TimeConvert.h"
#include "WorldMarket.h"

bool IsTiingoStock(const CVirtualStockPtr& pStock) {
	if (pStock == nullptr) return false;
	return typeid(*pStock) == typeid(CTiingoStock);
}

CTiingoStock::CTiingoStock() {
	m_v52WeekHighDate.clear();
	m_v52WeekLowDate.clear();
	SetExchange("US");
	CTiingoStock::ResetAllUpdateDate();
}

void CTiingoStock::ResetAllUpdateDate() {
	SetStatementLastUpdatedDate(toLocalDays(19800101));
	SetCompanyFinancialStatementUpdateDate(toLocalDays(19800101));
	SetDailyUpdateDate(toLocalDays(19800101));
	SetDayLineStartDate(toLocalDays(29900101));
	SetDayLineEndDate(toLocalDays(19800101));
	SetDayLineProcessDate(toLocalDays(19800101));
	SetHistoryDayLineStartDate(chrono::local_days(chrono::days(0)));
	SetHistoryDayLineEndDate(chrono::local_days(chrono::days(0)));
	SetUpdateStockDailyMetaDate(toLocalDays(19800101));
}

void CTiingoStock::UpdateRTData(const CTiingoIEXTopOfBook& IEXTopOfBook) {
	m_tpTime = IEXTopOfBook.m_timeStamp;
	m_lOpen = IEXTopOfBook.m_lOpen;
	m_lHigh = IEXTopOfBook.m_lHigh;
	m_lLow = IEXTopOfBook.m_lLow;
	m_lLastClose = IEXTopOfBook.m_lLastClose;
	m_lNew = IEXTopOfBook.m_lNew;
	m_llVolume = IEXTopOfBook.m_llVolume;
}

void CTiingoStock::UpdateDayLine(const CTiingoCandleLinesPtr& vTempDayLine) {
	m_dataDayLine.Unload();
	m_dataDayLine.Reserve(vTempDayLine->size());

	for (auto& p : *vTempDayLine) {
		m_dataDayLine.Add(p);
	}
	m_dataDayLine.SetDataLoaded(true);
}

void CTiingoStock::UpdateFinancialStateDB() {
	using namespace StockMarket;
	const auto& t = TiingoCompanyFinancialState{};
	auto db = gl_dbStockMarket.get();
	auto tx = sqlpp::start_transaction(db);

	auto result = db(select(all_of(t)).from(t).where(t.Symbol == m_strSymbol).order_by(t.YearQuarter.desc()));
	size_t rows = result.size();
	if (rows > 0) {
		auto& row = result.front();
		if (row.YearQuarter >= m_pvFinancialState->at(0)->m_yearQuarter) {
			db(sqlpp::remove_from(t).where(t.Symbol == GetSymbol() && t.YearQuarter >= m_pvFinancialState->at(0)->m_yearQuarter));
		}
	}
	for (auto& p : *m_pvFinancialState) {
		db(sqlpp::insert_into(t).set(
			t.Symbol = m_strSymbol,
			t.Exchange = p->m_exchange,
			t.YearQuarter = p->m_yearQuarter,
			t.accoci = p->m_accoci,
			t.acctPay = p->m_acctPay,
			t.acctRec = p->m_acctRec,
			t.assetsCurrent = p->m_assetsCurrent,
			t.assetsNonCurrent = p->m_assetsNonCurrent,
			t.assetTurnover = p->m_assetTurnover,
			t.bookVal = p->m_bookVal,
			t.businessAcqDisposals = p->m_businessAcqDisposals,
			t.bvps = p->m_bvps,
			t.capex = p->m_capex,
			t.cashAndEq = p->m_cashAndEq,
			t.consolidatedIncome = p->m_consolidatedIncome,
			t.costRev = p->m_costRev,
			t.currentRatio = p->m_currentRatio,
			t.debt = p->m_debt,
			t.debtCurrent = p->m_debtCurrent,
			t.debtEquity = p->m_debtEquity,
			t.debtNonCurrent = p->m_debtNonCurrent,
			t.deferredRev = p->m_deferredRev,
			t.depamor = p->m_depamor,
			t.deposits = p->m_deposits,
			t.ebit = p->m_ebit,
			t.ebitda = p->m_ebitda,
			t.ebt = p->m_ebt,
			t.enterpriseVal = p->m_enterpriseVal,
			t.eps = p->m_eps,
			t.epsDil = p->m_epsDil,
			t.epsQoQ = p->m_epsQoQ,
			t.equity = p->m_equity,
			t.freeCashFlow = p->m_freeCashFlow,
			t.fxRate = p->m_fxRate,
			t.grossMargin = p->m_grossMargin,
			t.grossProfit = p->m_grossProfit,
			t.intangibles = p->m_intangibles,
			t.intexp = p->m_intexp,
			t.inventory = p->m_inventory,
			t.investments = p->m_investments,
			t.investmentsAcqDisposals = p->m_investmentsAcqDisposals,
			t.investmentsCurrent = p->m_investmentsCurrent,
			t.investmentsNonCurrent = p->m_investmentsNonCurrent,
			t.issrepayDebt = p->m_issrepayDebt,
			t.issrepayEquity = p->m_issrepayEquity,
			t.liabilitiesCurrent = p->m_liabilitiesCurrent,
			t.liabilitiesNonCurrent = p->m_liabilitiesNonCurrent,
			t.longTermDebtEquity = p->m_longTermDebtEquity,
			t.marketCap = p->m_marketCap,
			t.ncf = p->m_ncf,
			t.ncff = p->m_ncff,
			t.ncfi = p->m_ncfi,
			t.ncfo = p->m_ncfo,
			t.ncfx = p->m_ncfx,
			t.netinc = p->m_netinc,
			t.netIncComStock = p->m_netIncComStock,
			t.netIncDiscOps = p->m_netIncDiscOps,
			t.netMargin = p->m_netMargin,
			t.nonControllingInterests = p->m_nonControllingInterests,
			t.opex = p->m_opex,
			t.opinc = p->m_opinc,
			t.opMargin = p->m_opMargin,
			t.payDiv = p->m_payDiv,
			t.pbRatio = p->m_pbRatio,
			t.peRatio = p->m_peRatio,
			t.piotroskiFScore = p->m_piotroskiFScore,
			t.ppeq = p->m_ppeq,
			t.prefDVDs = p->m_prefDVDs,
			t.profitMargin = p->m_profitMargin,
			t.retainedEarnings = p->m_retainedEarnings,
			t.revenue = p->m_revenue,
			t.revenueQoQ = p->m_revenueQoQ,
			t.rnd = p->m_rnd,
			t.roa = p->m_roa,
			t.roe = p->m_roe,
			t.rps = p->m_rps,
			t.sbcomp = p->m_sbcomp,
			t.sga = p->m_sga,
			t.shareFactor = p->m_shareFactor,
			t.sharesBasic = p->m_sharesBasic,
			t.shareswa = p->m_shareswa,
			t.shareswaDil = p->m_shareswaDil,
			t.taxAssets = p->m_taxAssets,
			t.taxExp = p->m_taxExp,
			t.taxLiabilities = p->m_taxLiabilities,
			t.totalAssets = p->m_totalAssets,
			t.totalLiabilities = p->m_totalLiabilities,
			t.trailingPEG1Y = p->m_trailingPEG1Y
		));
	}
	tx.commit();
}

void CTiingoStock::UpdateProfile(const CTiingoStockPtr& pStock) {
	ASSERT(gl_systemConfiguration.IsPaidTypeTiingoAccount()); // 调用此函数时，必须保证是付费账户。
	if (pStock->m_strTiingoSector.length() > 1) m_strTiingoSector = pStock->m_strTiingoSector;
	if (pStock->m_strTiingoIndustry.length() > 1)m_strTiingoIndustry = pStock->m_strTiingoIndustry;
	if (pStock->m_iSicCode > 0)m_iSicCode = pStock->m_iSicCode;
	if (pStock->m_strSicSector.length() > 1)m_strSicSector = pStock->m_strSicSector;
	if (pStock->m_strSicIndustry.length() > 1)m_strSicIndustry = pStock->m_strSicIndustry;
	if (pStock->m_strLocation.length() > 1)m_strLocation = pStock->m_strLocation;
	if (pStock->m_strCompanyWebSite.length() > 1)m_strCompanyWebSite = pStock->m_strCompanyWebSite;
	if (pStock->m_strSECFilingWebSite.length() > 1)m_strSECFilingWebSite = pStock->m_strSECFilingWebSite;
	SetUpdateProfileDB(true);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 使用从daily meta处得到的数据更新tiingo股票profile。
// 目前只使用日线开始和结束两个数据，其他数据暂时忽略。
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTiingoStock::UpdateDailyMeta(const CTiingoStockDailyMetaPtr& pMeta) {
	if (pMeta->m_lHistoryDayLineStartDate == toLocalDays(19000101)) {
		SetHistoryDayLineStartDate(toLocalDays(19500101));
		SetUpdateProfileDB(true);
	}
	else {
		if (GetHistoryDayLineStartDate() != pMeta->m_lHistoryDayLineStartDate) {
			SetHistoryDayLineStartDate(pMeta->m_lHistoryDayLineStartDate);
			SetUpdateProfileDB(true);
		}
	}
	if (pMeta->m_lHistoryDayLineEndDate == toLocalDays(19000101)) {
		SetHistoryDayLineEndDate(toLocalDays(19500101));
		SetUpdateProfileDB(true);
	}
	else {
		if (GetHistoryDayLineEndDate() != pMeta->m_lHistoryDayLineEndDate) {
			SetHistoryDayLineEndDate(pMeta->m_lHistoryDayLineEndDate);
			SetUpdateProfileDB(true);
		}
	}
}

void CTiingoStock::UpdateDayLineStartEndDate() {
	chrono::local_days lStartDate = toLocalDays(19000101), lEndDate = toLocalDays(19000101);
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

void CTiingoStock::CreateWeekLine() {
	ASSERT(m_dataDayLine.IsDataLoaded());
	size_t index = 0;
	CTiingoCandleLine weekLine;
	size_t dayLineSize = m_dataDayLine.Size();
	long lastClose = 0;
	while (index < dayLineSize) {
		weekLine.Reset();
		auto pDayLine = m_dataDayLine.GetData(index);
		auto lCurrentEndDate = GetNextMonday(pDayLine->GetDate());
		weekLine.SetDate(pDayLine->GetDate());
		weekLine.SetLastClose(lastClose);
		weekLine.SetOpen(pDayLine->GetOpen());
		weekLine.SetLow(pDayLine->GetLow());
		do {
			if (pDayLine->GetHigh() > weekLine.GetHigh()) weekLine.SetHigh(pDayLine->GetHigh());
			if (pDayLine->GetLow() < weekLine.GetLow()) weekLine.SetLow(pDayLine->GetLow());
			weekLine.SetVolume(weekLine.GetVolume() + pDayLine->GetVolume());
			weekLine.SetAmount(weekLine.GetAmount() + pDayLine->GetAmount());
			weekLine.SetClose(pDayLine->GetClose());
			index++;
			if (index < dayLineSize) pDayLine = m_dataDayLine.GetData(index);
			else break;
			if (pDayLine->GetDate() >= lCurrentEndDate) break;
		} while (true);
		lastClose = weekLine.GetClose();
		if (weekLine.GetClose() > 0) m_dataWeekLine.Add(weekLine); // 有数据才存储
	}

	m_dataWeekLine.SetDataLoaded(true);
}

void CTiingoStock::CreateMonthLine() {
	ASSERT(m_dataDayLine.IsDataLoaded());
	size_t index = 0;
	CTiingoCandleLine monthLine;
	size_t monthLineSize = m_dataDayLine.Size();
	while (index < monthLineSize) {
		monthLine.Reset();
		auto pDayLine = m_dataDayLine.GetData(index++);
		chrono::local_days lCurrentEndDate = GetNextMonth(pDayLine->GetDate());
		monthLine.SetDate(pDayLine->GetDate());
		monthLine.SetOpen(pDayLine->GetOpen());
		monthLine.SetLow(pDayLine->GetLow());
		do {
			if (pDayLine->GetHigh() > monthLine.GetHigh()) monthLine.SetHigh(pDayLine->GetHigh());
			if (pDayLine->GetLow() < monthLine.GetLow()) monthLine.SetLow(pDayLine->GetLow());
			monthLine.SetVolume(monthLine.GetVolume() + pDayLine->GetVolume());
			monthLine.SetAmount(monthLine.GetAmount() + pDayLine->GetAmount());
			monthLine.SetClose(pDayLine->GetClose());
			if (index < monthLineSize) pDayLine = m_dataDayLine.GetData(index);
			else break;
			if (pDayLine->GetDate() < lCurrentEndDate) index++;
			else break;
		} while (true);

		if (monthLine.GetClose() > 0) m_dataMonthLine.Add(monthLine); // 有数据才存储
	}

	m_dataMonthLine.SetDataLoaded(true);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 由于tiingo更新完日线数据后需要再次处理日线，故而此处默认为不卸载日线。这样能够加速后面的处理过程。
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////
bool CTiingoStock::UpdateDayLineDB() {
	DeleteDuplicatedDayLine();

	SaveDayLineDB();
	string str = GetSymbol();
	str += "日线资料存储完成";
	gl_systemMessage.PushDayLineInfoMessage(str);

	return true;
}

bool CTiingoStock::IsDayLineDuplicated() noexcept {
	if (m_dataDayLine.Empty()) return false;
	if (m_dataDayLine.GetData(0)->GetDate() > GetDayLineEndDate()) return false;
	return true;
}

void CTiingoStock::DeleteDuplicatedDayLine() noexcept {
	ASSERT(!m_dataDayLine.Empty());
	using namespace StockMarket;
	const auto& t = TiingoStockDayline{};
	auto db = gl_dbStockMarket.get();
	auto tx = sqlpp::start_transaction(db);

	db(sqlpp::remove_from(t).where(t.Symbol == GetSymbol() && t.Date >= toFormattedDate(m_dataDayLine.GetData(0)->GetDate())));
	tx.commit();
}

void CTiingoStock::RebuildStockSplitDB() {
	if (!m_dataDayLine.IsDataLoaded()) {
		m_dataDayLine.LoadDB(m_strSymbol);
	}
	m_pvStockSplit->clear();
	for (size_t index = 0; index < m_dataDayLine.Size(); index++) {
		if (std::abs(m_dataDayLine.GetData(index)->GetSplitFactor() - 1.0) > EPSILON) {
			CStockSplit stockSplit;
			stockSplit.SetDate(m_dataDayLine.GetData(index)->GetDate());
			stockSplit.SetRatio(m_dataDayLine.GetData(index)->GetSplitFactor());
			m_pvStockSplit->push_back(stockSplit);
		}
	}
}

bool CTiingoStock::HaveNewDayLineData() {
	if (m_dataDayLine.Empty()) return false;
	if ((m_dataDayLine.GetData(m_dataDayLine.Size() - 1)->GetDate() > GetDayLineEndDate())
		|| (m_dataDayLine.GetData(0)->GetDate() < GetDayLineStartDate()))
		return true;
	return false;
}

void CTiingoStock::CheckUpdateStatus(chrono::local_days lTodayDate) {
	CheckFinancialStateUpdateStatus(lTodayDate);
	CheckDayLineUpdateStatus(lTodayDate);
	CheckStockDailyMetaStatus(lTodayDate);
}

void CTiingoStock::CheckFinancialStateUpdateStatus(chrono::local_days lTodayDate) {
	if (IsEarlyThen(GetCompanyFinancialStatementUpdateDate(), lTodayDate, gl_systemConfiguration.GetTiingoCompanyFinancialStateUpdateRate())) {
		m_fUpdateFinancialState = true;
	}
	else {
		m_fUpdateFinancialState = false;
	}
}

void CTiingoStock::CheckDayLineUpdateStatus(chrono::local_days lTodayDate) {
	if (GetDayLineEndDate() >= gl_pWorldMarket->GetCurrentTradeDate()) { // 已更新？
		m_fUpdateDayLine = false;
		return;
	}
	ASSERT(GetDayLineEndDate() < gl_pWorldMarket->GetCurrentTradeDate());
	m_fUpdateDayLine = true;
}

void CTiingoStock::CheckStockDailyMetaStatus(chrono::local_days lCurrentDate) {
	if (GetUpdateStockDailyMetaDate() >= gl_pWorldMarket->GetCurrentTradeDate()) {
		SetUpdateStockDailyMeta(false);
	}
	else {
		SetUpdateStockDailyMeta(true);
	}
}

chrono::local_days CTiingoStock::GetDayLineProcessDate() {
	long l;
	try {
		l = m_jsonUpdateDate["DayLineProcessDate"];
	} catch (nlohmannJson::exception&) {
		m_jsonUpdateDate["DayLineProcessDate"] = 19800101;
		l = 19800101;
	}
	return toLocalDays(l);
}

void CTiingoStock::Delete52WeekLowDate(chrono::local_days lDate) {
	auto it = std::ranges::find(m_v52WeekLowDate, lDate);
	if (it != m_v52WeekLowDate.end()) {
		m_v52WeekLowDate.erase(it);
	}
}

void CTiingoStock::Delete52WeekHighDate(chrono::local_days lDate) {
	auto it = std::ranges::find(m_v52WeekHighDate, lDate);
	if (it != m_v52WeekHighDate.end()) {
		m_v52WeekHighDate.erase(it);
	}
}

void CTiingoStock::Delete52WeekHighDB() const {
	using namespace StockMarket;
	const auto& t = TiingoStock52WeekHigh{};

	auto db = gl_dbStockMarket.get();
	auto tx = sqlpp::start_transaction(db);

	db(sqlpp::remove_from(t).where(t.Symbol == GetSymbol()));
	tx.commit();
}

void CTiingoStock::Delete52WeekLowDB() const {
	using namespace StockMarket;
	const auto& t = TiingoStock52WeekLow{};

	auto db = gl_dbStockMarket.get();
	auto tx = sqlpp::start_transaction(db);

	db(sqlpp::remove_from(t).where(t.Symbol == GetSymbol()));
	tx.commit();
}

bool CTiingoStock::IsEnough52WeekLow() {
	if (m_v52WeekLowDate.size() < 6) {
		return false;
	}
	ranges::sort(m_v52WeekLowDate, [](const chrono::local_days l1, const chrono::local_days l2) { return l1 > l2; });
	auto lDate = GetPrevDay(gl_pWorldMarket->GetCurrentTradeDate(), 365);
	if (m_v52WeekLowDate.at(4) > lDate) {
		return true;
	}
	return false;
}

void CTiingoStock::Load52WeekLowDB() {
	if (!m_v52WeekLowDate.empty()) return; // 如果已经装入了， 直接返回。

	using namespace StockMarket;
	const auto& t = TiingoStock52WeekLow{};

	auto db = gl_dbStockMarket.get();
	auto tx = sqlpp::start_transaction(db);

	auto result = db(select(all_of(t)).from(t).where(t.Symbol == GetSymbol()).order_by(t.Date.asc()));
	m_v52WeekLowDate.reserve(result.size<>());
	for (const auto& row : result) {
		m_v52WeekLowDate.push_back(toLocalDays(row.Date));
	}
	tx.commit();
}

/////////////////////////////////////////////////////////////////////////////////////////////
///
/// 处理日线
/// 目前只查找52周新低和52周新高。
/// 20260423: 找寻52周新高后，计算三个月内再创新高的几率，对于新低同样计算三个月内再创新低的几率。这个数据对于投资者来说是非常有用的。
///
/// Note 目前采用的方法极其简单，导致计算的时间较长，每个股票的计算时间大致为1-3秒
/// 故而采用后台持续执行的模式，单一工作线程调用。
/// 
///
//////////////////////////////////////////////////////////////////////////////////////////////
void CTiingoStock::ProcessDayLine() {
	if (gl_systemConfiguration.IsExitingSystem()) return;
	if (!IsDayLineLoaded()) {
		m_dataDayLine.LoadDB(GetSymbol());
	}

	auto endPos = m_dataDayLine.Size();
	if (endPos < 300) return; // 最少300个日线数据
	m_v52WeekHighDate.clear();
	m_v52WeekLowDate.clear();
	for (size_t index = 0; index < endPos; index++) {
		double d = m_dataDayLine.GetData(index)->GetClose();
		m_vClose.push_back(d);
	}

	double dSplitFactor = CalculateSplitFactor(0, endPos);
	AdjustedStockCloseValue(dSplitFactor, 0, endPos);

	//FindHighLow3(endPos); // 这种是最简单的。耗时虽然长，但与数据库操作相比还是短的，故而采用更快速的算法没有那么必须了。
	FindHighLow2(endPos); // 这种速度较快。
	m_dataDayLine.Unload();
	SetUpdate52WeekHighLowDB(true); // 设置存储52周新高和52周新低的标识。

	// 计算三个月内再创新高的几率
	CalculateNewHighHigher();
}

void CTiingoStock::ProcessDayLine2(long lLastCalculatedDate) {
	if (!IsDayLineLoaded()) {
		m_dataDayLine.LoadDB(GetSymbol());
	}

	auto endPos = m_dataDayLine.Size();
	if (endPos < 300) return; // 最少300个日线数据
	m_v52WeekHighDate.clear();
	m_v52WeekLowDate.clear();
	for (size_t index = 0; index < endPos; index++) {
		double d = m_dataDayLine.GetData(index)->GetClose();
		m_vClose.push_back(d);
	}

	double dSplitFactor = CalculateSplitFactor(0, endPos);
	AdjustedStockCloseValue(dSplitFactor, 0, endPos);
	FindHighLow2(endPos);

	UnloadDayLine();
	SetUpdate52WeekHighLowDB(true);
}

void CTiingoStock::ProcessDayLine3(long lLastCalculatedDate) {
	if (!IsDayLineLoaded()) {
		m_dataDayLine.LoadDB(GetSymbol());
	}
	auto endPos = m_dataDayLine.Size();

	if (endPos < 300) return; // 最少300个日线数据
	m_v52WeekHighDate.clear();
	m_v52WeekLowDate.clear();
	for (size_t index = 0; index < endPos; index++) {
		double d = m_dataDayLine.GetData(index)->GetClose();
		m_vClose.push_back(d);
	}

	double dSplitFactor = CalculateSplitFactor(0, endPos);
	AdjustedStockCloseValue(dSplitFactor, 0, endPos);
	FindHighLow3(endPos);
	m_dataDayLine.Unload();
	SetUpdate52WeekHighLowDB(true);

	// 计算三个月内再创新高的几率
	CalculateNewHighHigher();
	CalculateNewLowLower();
}

void CTiingoStock::FindHighLow3(size_t endPos) {
	for (size_t currentPos = 250; currentPos < endPos; currentPos++) { // 从第250个位置开始寻找。
		auto dClose = m_vClose[currentPos];
		switch (IsLowOrHigh(currentPos, dClose)) {
		case -1: // new low
			Add52WeekLowDate(m_dataDayLine.GetData(currentPos)->GetDate());
			break;
		case 1: // new high
			Add52WeekHighDate(m_dataDayLine.GetData(currentPos)->GetDate());
			break;
		default:
			break;
		}
	}
}

int CTiingoStock::IsLowOrHigh(size_t index, double dClose) const {
	ASSERT(index >= 250);
	bool fIsNewLow = true;
	bool fIsNewHigh = true;
	double belowClose = dClose + EPSILON; // 增加一点以利于判断相同的数值。
	for (size_t i = index - 250; i < index; i++) {
		if (m_vClose[i] < belowClose) {
			fIsNewLow = false;
			break;
		}
	}
	if (fIsNewLow) return -1; // 52周新低价
	double aboveClose = dClose - EPSILON;// 减少一点以利于判断相同的数值。
	for (size_t i = index - 250; i < index; i++) {
		if (m_vClose[i] > aboveClose) {
			fIsNewHigh = false;
			break;
		}
	}
	if (fIsNewHigh) return 1; // 52周新高价
	return 0; // 既非52周最高价亦非52周最低价
}

void CTiingoStock::FindHighLow2(size_t endPos) {
	FindAll52WeekLowDate(0, endPos);
	FindAll52WeekHighDate(0, endPos);
}

void CTiingoStock::FindAll52WeekLowDate(size_t beginPos, size_t endPos) {
	bool fFound = false;
	size_t currentBeginPos = beginPos;
	size_t currentPos = 0;
	size_t current52WeekLowPos = beginPos;
	auto dCurrent52WeekLowValue = m_vClose[beginPos];

	while (currentPos < endPos) {
		if (fFound) {
			bool fCurrentFound = false;
			double belowCurrent52WeekLow = dCurrent52WeekLowValue - EPSILON;
			for (auto index = currentBeginPos; index <= current52WeekLowPos; index++) {
				if (currentPos == endPos) {
					fCurrentFound = false;
					break;
				}
				if (m_vClose[currentPos] < belowCurrent52WeekLow) { // 找到了
					Add52WeekLowDate(m_dataDayLine.GetData(currentPos)->GetDate());
					dCurrent52WeekLowValue = m_vClose[currentPos];
					current52WeekLowPos = currentPos;
					currentPos++;
					currentBeginPos = currentPos - 250;
					fCurrentFound = true;
					break;
				}
				currentPos++;
			}
			if (!fCurrentFound) {
				currentBeginPos = current52WeekLowPos + 1;
				fFound = false;
			}
		}
		else { // 找寻下一个新低位置
			currentPos = currentBeginPos + 250 > endPos ? endPos : currentBeginPos + 250;
			current52WeekLowPos = FindCurrent52WeekLowPos(currentBeginPos, currentPos, dCurrent52WeekLowValue);
			dCurrent52WeekLowValue = m_vClose[current52WeekLowPos];
			fFound = true;
		}
	}
}

size_t CTiingoStock::FindCurrent52WeekLowPos(size_t beginPos, size_t endPos, double& value) const {
	ASSERT(beginPos < endPos);
	auto pos = beginPos;
	value = m_vClose[beginPos];
	double belowCurrentValue = value - EPSILON;

	for (auto index = beginPos; index < endPos; index++) {
		if (m_vClose[index] < belowCurrentValue) {
			pos = index;
			value = m_vClose[index];
			belowCurrentValue = value - EPSILON;
		}
	}
	return pos;
}

void CTiingoStock::FindAll52WeekHighDate(size_t beginPos, size_t endPos) {
	bool fFound = false;
	size_t currentBeginPos = beginPos;
	size_t currentEndPos = currentBeginPos + 250 > endPos ? endPos : currentBeginPos + 250;
	size_t current52WeekHighPos = beginPos;
	auto dCurrent52WeekHighValue = m_vClose[beginPos];

	while (currentBeginPos < endPos - 1) {
		if (fFound) { // 有最高价
			auto aboveCurrent52WeekHigh = dCurrent52WeekHighValue + EPSILON;
			bool fCurrentFound = false;
			for (auto index = currentBeginPos; index <= current52WeekHighPos; index++) { // 查询到最新的新高价
				if (currentEndPos == endPos) {
					currentBeginPos = endPos - 1;
					fCurrentFound = false;
					break;
				}
				if (m_vClose[currentEndPos] > aboveCurrent52WeekHigh) { // 找到了
					Add52WeekHighDate(m_dataDayLine.GetData(currentEndPos)->GetDate());
					dCurrent52WeekHighValue = m_vClose[currentEndPos];
					current52WeekHighPos = currentEndPos;
					fCurrentFound = true;
					currentBeginPos = current52WeekHighPos - 250;
					break;
				}
				currentEndPos++;
			}
			if (!fCurrentFound) {
				if (currentEndPos < endPos - 1) {
					currentBeginPos = current52WeekHighPos + 1;
					currentEndPos = endPos < currentBeginPos + 250 ? endPos : currentBeginPos + 250;
				}
				else {
					break; // 全部查询完毕
				}
				fFound = false;
			}
		}
		else { // 找寻下一个新高位置
			if (currentBeginPos + 250 < endPos) {
				currentEndPos = currentBeginPos + 250;
			}
			else {
				currentEndPos = endPos;
			}
			current52WeekHighPos = FindCurrent52WeekHighPos(currentBeginPos, currentEndPos, dCurrent52WeekHighValue);
			dCurrent52WeekHighValue = m_vClose[current52WeekHighPos];
			fFound = true;
		}
	}
}

size_t CTiingoStock::FindCurrent52WeekHighPos(size_t beginPos, size_t endPos, double& value) const {
	ASSERT(beginPos < endPos);
	auto pos = beginPos;
	value = m_vClose[beginPos];
	auto aboveCurrentValue = value + EPSILON;

	for (auto index = beginPos; index < endPos; index++) {
		if (m_vClose[index] > aboveCurrentValue) {
			pos = index;
			value = m_vClose[index];
			aboveCurrentValue = value + EPSILON;
		}
	}
	return pos;
}

double CTiingoStock::CalculateSplitFactor(size_t beginPos, size_t endPos) {
	double dRatio = 1;
	for (auto index = beginPos; index < endPos; index++) {
		auto splitFactor = m_dataDayLine.GetData(index)->GetSplitFactor();
		if (splitFactor > EPSILON) {
			dRatio *= splitFactor;
		}
	}
	return dRatio;
}

void CTiingoStock::AdjustedStockCloseValue(double dSplitFactor, size_t calculatePos, size_t dayLineSize) {
	double dCurrentSplitFactor = 1;
	if (dSplitFactor < 1) { // 总体是缩股的。
		for (auto index = dayLineSize - 1; index > calculatePos; index--) { // 向前复权（保持目前的股价不变）
			m_vClose[index] *= dCurrentSplitFactor; // 使用除法向前复权。要先计算，然后才算splitFactor
			auto currentSplitFactor = m_dataDayLine.GetData(index)->GetSplitFactor();
			if (currentSplitFactor < 1 + EPSILON && currentSplitFactor > 1 - EPSILON) {
				// do nothing
			}
			else {
				if (currentSplitFactor > EPSILON) {
					dCurrentSplitFactor /= currentSplitFactor;
				}
			}
		}
	}
	else { // 总体是扩股的。
		for (auto index = calculatePos; index < dayLineSize; index++) { // 向后复权（目前的股价会变大）
			auto currentSplitFactor = m_dataDayLine.GetData(index)->GetSplitFactor();
			if ((currentSplitFactor < 1 + EPSILON) && currentSplitFactor > 1 - EPSILON) {
				// do nothing
			}
			else {
				dCurrentSplitFactor *= currentSplitFactor;
			}
			m_vClose[index] *= dCurrentSplitFactor;
		}
	}
}

void CTiingoStock::CalculateNewHighHigher(int period) {
	if (m_v52WeekHighDate.empty()) return;
	auto currentDate = gl_pWorldMarket->GetCurrentTradeDate();
	for (size_t index = 0; index < m_v52WeekHighDate.size(); index++) {
		auto highDate = m_v52WeekHighDate.at(index);
		auto nextHighDate = m_v52WeekHighDate.size() > index + 1 ? m_v52WeekHighDate.at(index + 1) : toLocalDays(19000101);
		if (nextHighDate > toLocalDays(19000101) && nextHighDate <= GetNextDay(currentDate, period)) {
			m_lHighHigher++;
		}
		else {
			m_lNoHighHigher++;
		}
	}
	gl_pWorldMarket->AddNewHighHigher(m_lHighHigher);
	gl_pWorldMarket->AddNoNewHighHigher(m_lNoHighHigher);
}

void CTiingoStock::CalculateNewLowLower(int period) {
	if (m_v52WeekLowDate.empty()) return;
	auto currentDate = gl_pWorldMarket->GetCurrentTradeDate();
	for (size_t index = 0; index < m_v52WeekLowDate.size(); index++) {
		auto LowDate = m_v52WeekLowDate.at(index);
		auto nextLowDate = m_v52WeekLowDate.size() > index + 1 ? m_v52WeekLowDate.at(index + 1) : toLocalDays(19000101);
		if (nextLowDate > toLocalDays(19000101) && nextLowDate <= GetNextDay(currentDate, period)) {
			m_lLowLower++;
		}
		else {
			m_lNoLowLower++;
		}
	}
}
