#include"pch.h"

#include"TimeConvert.h"
#include"ChinaStockCodeConverter.h"

#include "ChinaStock.h"

#include"ChinaMarket.h"
#include "dataBaseConnector.h"
#include"MonthLine.h"

////////////////////////////////////////////////////////////////////////////////////////////
//
// 判断strStockCode是否为沪深A股主板的股票代码。
// 沪市A股代码以600或601开头，深市A股代码以000或001开头。
//
///////////////////////////////////////////////////////////////////////////////////////////////
bool IsShareA(const string& strStockCode) {
	const string strSymbol = GetStockSymbol(strStockCode);
	if (IsShanghaiExchange(strStockCode)) {
		if ((strSymbol[0] == '6') && (strSymbol[1] == '0')) {
			if ((strSymbol[2] == '0') || (strSymbol[2] == '1')) {
				return true;
			}
		}
	}
	else if (IsShenzhenExchange(strStockCode)) {
		if ((strSymbol[0] == '0') && (strSymbol[1] == '0')) {
			if ((strSymbol[2] == '0') || (strSymbol[2] == '2')) {
				return true;
			}
		}
	}
	return false;
}

bool IsChinaStock(const CVirtualStockPtr& pStock) {
	if (pStock == nullptr) return false;
	return typeid(*pStock) == typeid(CChinaStock);
}

CChinaStock::CChinaStock() {
	for (int i = 0; i < 5; i++) {
		m_lPBuy.at(i) = m_lPSell.at(i) = 0;
		m_lVBuy.at(i) = m_lVSell.at(i) = 0;
	}

	m_nHand = 100;

	m_lHighLimitFromTengxun = m_lLowLimitFromTengxun = 0;
	m_lHighLimit = m_lLowLimit = 0;
	for (int i = 0; i < 5; i++) {
		m_lPBuy.at(i) = m_lPSell.at(i) = 0;
		m_lVBuy.at(i) = m_lVSell.at(i) = 0;
	}
	m_dRealtimeRS = 0;
	m_dRealtimeRSIndex = 0;

	m_llLastSavedVolume = 0;

	m_fChosen = false;
	m_fUpdateChosenStockDB = false;

	m_fDayLineDBUpdated = false;

	m_fHaveFirstRTData = false; // 实时数据开始计算标识。第一个实时数据只能用来初始化系统，不能用于计算。从第二个数据开始计算才有效。
	m_fNeedProcessRTData = true;
	m_fRTDataCalculated = false;
	m_pLastRTData = nullptr;

	ClearRTDataDeque();
}

void CChinaStock::ClearRTDataDeque() {
	const auto lTotalNumber = GetRTDataQueueSize();
	for (size_t i = 0; i < lTotalNumber; i++) {
		CWebRTDataPtr pRTData = PopRTData();
	}
}

bool CChinaStock::HaveNewDayLineData() {
	if (m_dataDayLine.Size() <= 0) return false;
	if (m_dataDayLine.GetData(m_dataDayLine.Size() - 1)->GetDate() > GetDayLineEndDate()) return true;
	return false;
}

/////////////////////////////////////////////////////////////////////////////////////
//
// 此函数被工作线程ThreadCalculatingRTDataProc调用，尽量减少对全局变量的操作。
//
// 交易情况和挂单情况似乎分开计算比较好，思路容易清晰。目前来看计算能力尚够，但要考虑旧机器的速度了。
//
//
////////////////////////////////////////////////////////////////////////////////////
void CChinaStock::ProcessRTData() {
	size_t lTotalNumber = GetRTDataQueueSize(); //  缓存队列的长度。采用同步机制获取其数值.
	// 以下为计算挂单变化、股票活跃度、大单买卖情况
	for (size_t i = 0; i < lTotalNumber; i++) {
		const CWebRTDataPtr pRTData = PopRTData(); // 采用同步机制获取数据
		if (pRTData->IsActive()) {// 数据有效
			UpdateRTData(pRTData); // 更新股票现时状态。
			m_fRTDataCalculated = true;
		}
	}
}

void CChinaStock::ReportDayLineDownLoaded() {
	//string strTemp = GetSymbol();
	//strTemp += "日线下载完成.");
	//gl_systemMessage.PushDayLineInfoMessage(strTemp);
}

void CChinaStock::UpdateDayLineDB() {
	if (IsDayLineDuplicated()) {
		DeleteDuplicatedDayLine();
	}
	SaveDayLineDB();
	UpdateDayLineStartEndDate();
	UnloadDayLine();
}

bool CChinaStock::IsDayLineDuplicated() noexcept {
	if (m_dataDayLine.Empty()) return false;
	if (m_dataDayLine.GetData(0)->GetDate() > GetDayLineEndDate()) return false;
	return true;
}

void CChinaStock::DeleteDuplicatedDayLine() noexcept {
	ASSERT(!m_dataDayLine.Empty());
	using namespace StockMarket;
	const auto& t = ChinaStockDayline{};
	auto db = gl_dbStockMarket.get();
	auto tx = sqlpp::start_transaction(db);

	db(sqlpp::remove_from(t).where(t.Symbol == GetSymbol() && t.Date >= toFormattedDate(m_dataDayLine.GetData(0)->GetDate())));
	tx.commit();
}

void CChinaStock::UpdateCurrentHistoryCandle(const CVirtualHistoryCandlePtr& pBeUpdated) const {
	pBeUpdated->SetDate(floor<chrono::days>(gl_pChinaMarket->ToLocalTime(GetTimePoint())));
	pBeUpdated->SetExchange(m_strExchange);
	pBeUpdated->SetStockSymbol(m_strSymbol);
	pBeUpdated->SetLastClose(m_lLastClose);
	pBeUpdated->SetOpen(m_lOpen);
	pBeUpdated->SetHigh(m_lHigh);
	pBeUpdated->SetLow(m_lLow);
	pBeUpdated->SetClose(m_lNew);
	pBeUpdated->SetVolume(m_llVolume);
	pBeUpdated->SetAmount(m_llAmount);
	pBeUpdated->SetUpDown(static_cast<double>(m_lUpDown) / GetRatio());
	pBeUpdated->SetUpDownRate(m_dUpDownRate);
	if (m_llTotalValue != 0) {
		pBeUpdated->SetChangeHandRate(static_cast<double>(100) * m_llAmount / m_llTotalValue);
	}
	else {
		pBeUpdated->SetChangeHandRate(0.0);
	}
	pBeUpdated->SetTotalValue(m_llTotalValue);
	pBeUpdated->SetCurrentValue(m_llCurrentValue);
}

void CChinaStock::UpdateRTData(const CWebRTDataPtr& pRTData) {
	SetTimePoint(pRTData->GetTime());
	SetLastClose(pRTData->GetLastClose());
	SetNew(pRTData->GetNew());
	SetHigh(pRTData->GetHigh());
	SetLow(pRTData->GetLow());
	SetOpen(pRTData->GetOpen());
	SetVolume(pRTData->GetVolume());
	SetAmount(pRTData->GetAmount());
	SetUpDown(m_lNew - m_lLastClose);
	if (m_lLastClose > 0) SetUpDownRate(static_cast<double>(m_lUpDown) * 100 / m_lLastClose);
	else SetUpDownRate(0);
	// 新浪实时数据是没有总市值和流通市值这两项的，故而需要判断一下是否为零
	if (pRTData->GetTotalValue() > 0) SetTotalValue(pRTData->GetTotalValue());
	if (pRTData->GetCurrentValue() > 0) SetCurrentValue(pRTData->GetCurrentValue());
	if (m_llTotalValue != 0) m_dChangeHandRate = static_cast<double>(m_llAmount) * 100 / m_llTotalValue;
	else m_dChangeHandRate = 0;
	for (int i = 0; i < 5; i++) {
		SetPBuy(i, pRTData->GetPBuy(i));
		SetVBuy(i, pRTData->GetVBuy(i));
		SetPSell(i, pRTData->GetPSell(i));
		SetVSell(i, pRTData->GetVSell(i));
	}
}

void CChinaStock::UpdateStatus(const CWebRTDataPtr& pRTData) {
	SetActive(true);
	SetSymbol(pRTData->GetSymbol()); // 更新全局股票池信息（有时RTData不全，无法更新退市的股票信息）
	if (!pRTData->GetStockName().empty()) {
		SetDisplaySymbol(pRTData->GetStockName()); // 更新全局股票池信息（有时RTData不全，无法更新退市的股票信息）
	}
}

void CChinaStock::UpdateDayLineStartEndDate() {
	bool fUpdated = false;

	if (m_dataDayLine.Size() > 0) {
		if ((GetDayLineStartDate() == toLocalDays(CHINA_MARKET_BEGIN_DATE_)) || (m_dataDayLine.GetData(0)->GetDate() < GetDayLineStartDate())) {
			SetDayLineStartDate(m_dataDayLine.GetData(0)->GetDate());
			SetDayLineDBUpdated(true);
			fUpdated = true;
		}
		if (m_dataDayLine.GetData(m_dataDayLine.Size() - 1)->GetDate() > GetDayLineEndDate()) {
			SetDayLineEndDate(m_dataDayLine.GetData(m_dataDayLine.Size() - 1)->GetDate());
			auto day = GetDayLineEndDate();
			SetDayLineDBUpdated(true);
			fUpdated = true;
		}
	}
	if (fUpdated) {
		SetUpdateProfileDB(true);
	}
}

bool CChinaStock::LoadStockCode(const CStockSymbol& stockSymbol) {
	m_strDescription = stockSymbol.m_Description;
	m_strDisplaySymbol = stockSymbol.m_DisplaySymbol;
	m_strExchange = stockSymbol.m_Exchange;
	m_strSymbol = stockSymbol.m_Symbol;
	LoadUpdateDate(stockSymbol.m_UpdateDate);

	return true;
}

void CChinaStock::CheckNeedProcessRTData() {
	SetNeedProcessRTData(true);
	if (IsShanghaiExchange(GetSymbol())) {
		if (GetSymbol().substr(0, 6) <= "000999") {//沪市指数？
			SetNeedProcessRTData(false);
		}
	}
	else if ((GetSymbol().substr(0, 6) >= "399000")) {// 深市指数
		SetNeedProcessRTData(false);
	}
}

bool CChinaStock::CheckDayLineStatus() {
	// 不再更新日线数据比上个交易日要新的股票。其他所有的股票都查询一遍，以防止出现新股票或者老的股票重新活跃起来。
	if (gl_pChinaMarket->GetLastTradeDate() <= GetDayLineEndDate()) {// 最新日线数据为今日或者上一个交易日的数据。
		SetUpdateDayLine(false); // 日线数据不需要更新
	}
	return m_fUpdateDayLine;
}

void CChinaStock::CreateWeekLine() {
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

void CChinaStock::CreateMonthLine() {
	ASSERT(m_dataDayLine.IsDataLoaded());
	size_t index = 0;
	CMonthLine monthLine;
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

////////////////////////////////////////////////////////////////////////////////
//
// 判断股票数据是否有效。采用最高价、最低价、成交量和成交额来判断，如果都为零，则认为此股今日没有有效数据
// 不采用开盘价，因开盘价有可能不为零时，其他数据皆为零（停牌时即此状态）。
//
////////////////////////////////////////////////////////////////////////////////
bool CChinaStock::IsTodayDataActive() const {
	if (!m_fActive) return false;
	return IsTodayDataChanged();
}

////////////////////////////////////////////////////////////////////////////////
//
// 判断股票数据是否有效。采用最高价、最低价、成交量和成交额来判断，如果都为零，则认为此股今日没有有效数据
// 不采用开盘价，因开盘价有可能不为零时，其他数据皆为零（停牌时即此状态）。
//
////////////////////////////////////////////////////////////////////////////////
bool CChinaStock::IsTodayDataChanged() const {
	if ((GetHigh() != 0) || (GetLow() != 0) || (GetVolume() != 0) || (GetAmount() != 0)) {
		return true;
	}
	return false;
}
