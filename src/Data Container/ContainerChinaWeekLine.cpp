#include"pch.h"

#include "ContainerChinaWeekLine.h"

#include "dataBaseConnector.h"

namespace {
	CChinaStock s_stock;
}

CContainerChinaWeekLine::CContainerChinaWeekLine() {
	m_ratio = s_stock.GetRatio();
}

bool CContainerChinaWeekLine::SaveDB(const string& strStockSymbol) {
	if (Size() == 0) {
		return false;
	}

	using namespace StockMarket;
	const auto& t = ChinaStockWeekline{};
	auto db = gl_dbStockMarket.get();
	auto tx = start_transaction(db);

	auto multi_insert = insert_into(t).columns(t.Date, t.Exchange, t.Symbol,
	                                           t.LastClose, t.Open, t.High, t.Low, t.Close, t.Dividend, t.SplitFactor,
	                                           t.Volume, t.Amount, t.UpAndDown, t.UpDownRate, t.ChangeHandRate, t.TotalValue, t.CurrentValue);

	const size_t lSize = Size();
	int nValues = 0;
	try {
		// no old data: bulk insert all
		for (size_t i = 0; i < lSize; ++i) {
			auto pCandle = GetData(i);
			multi_insert.values.add(t.Date = pCandle->GetDate(),
			                        t.Exchange = pCandle->GetExchange(),
			                        t.Symbol = pCandle->GetStockSymbol(),
			                        t.LastClose = static_cast<double>(pCandle->GetLastClose()) / m_ratio,
			                        t.Open = static_cast<double>(pCandle->GetOpen()) / m_ratio,
			                        t.High = static_cast<double>(pCandle->GetHigh()) / m_ratio,
			                        t.Low = static_cast<double>(pCandle->GetLow()) / m_ratio,
			                        t.Close = static_cast<double>(pCandle->GetClose()) / m_ratio,
			                        t.Dividend = static_cast<double>(pCandle->GetDividend()),
			                        t.SplitFactor = static_cast<double>(pCandle->GetSplitFactor()),
			                        t.Volume = static_cast<double>(pCandle->GetVolume()),
			                        t.Amount = static_cast<double>(pCandle->GetAmount()),
			                        t.UpAndDown = static_cast<double>(pCandle->GetUpDown()),
			                        t.UpDownRate = static_cast<double>(pCandle->GetUpDownRate()),
			                        t.ChangeHandRate = pCandle->GetChangeHandRate(),
			                        t.TotalValue = static_cast<double>(pCandle->GetTotalValue()),
			                        t.CurrentValue = static_cast<double>(pCandle->GetCurrentValue())
			);
			nValues++;
		}
		if (nValues > 0) db(multi_insert);
		tx.commit();
	} catch (...) {
		tx.rollback();
		throw;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////
///
/// Note: 需要使用大宗插入模式，否则出错
///
///
///
/////////////////////////////////////////////////////////////////////////////////////////
void CContainerChinaWeekLine::SaveCurrentWeekLine() const {
	using namespace StockMarket;
	const auto& t = ChinaCurrentWeekline{};
	auto db = gl_dbStockMarket.get();
	auto tx = sqlpp::start_transaction(db);
	auto ratio = GetRatio();

	auto multi_insert = insert_into(t).columns(t.Date, t.Exchange, t.Symbol,
	                                           t.LastClose, t.Open, t.High, t.Low, t.Close, t.Volume, t.Amount,
	                                           t.Dividend, t.SplitFactor, t.UpAndDown, t.UpDownRate, t.ChangeHandRate, t.TotalValue, t.CurrentValue);
	int nValues = 0;
	for (const auto& data : m_vHistoryData) {
		multi_insert.values.add(
			t.Date = (int)data.GetDate(),
			t.Exchange = data.GetExchange(),
			t.Symbol = data.GetStockSymbol(),
			t.LastClose = static_cast<double>(data.GetLastClose()) / ratio,
			t.Open = static_cast<double>(data.GetOpen()) / ratio,
			t.High = static_cast<double>(data.GetHigh()) / ratio,
			t.Low = static_cast<double>(data.GetLow()) / ratio,
			t.Close = static_cast<double>(data.GetClose()) / ratio,
			t.Volume = static_cast<double>(data.GetVolume()),
			t.Amount = static_cast<double>(data.GetAmount()),
			t.Dividend = data.GetDividend(),
			t.SplitFactor = data.GetSplitFactor(),
			t.UpAndDown = static_cast<double>(data.GetUpDown()) / ratio,
			t.UpDownRate = data.GetUpDownRate() / ratio,
			t.ChangeHandRate = 0.0,
			t.TotalValue = static_cast<double>(data.GetTotalValue()),
			t.CurrentValue = static_cast<double>(data.GetCurrentValue())
		);
		nValues++;
	}
	if (nValues > 0) db(multi_insert);
	tx.commit();

	TRACE(_T("存储了%d个当前周周线数据\n"), m_vHistoryData.size());
}

bool CContainerChinaWeekLine::LoadDB(const string& strStockCode) {
	using namespace StockMarket;
	const auto& t = ChinaStockWeekline{};
	auto db = gl_dbStockMarket.get();
	auto tx = start_transaction(db);

	Reset();
	auto result = db(select(all_of(t)).from(t).where(t.Symbol == strStockCode).order_by(t.Date.asc()));
	size_t rows = result.size();
	Reserve(rows);

	for (const auto& row : result) {
		CVirtualHistoryCandle historyCandle;
		int ratio = GetRatio();
		historyCandle.Reset();
		historyCandle.SetRatio(ratio);
		historyCandle.SetDate(row.Date);
		historyCandle.SetExchange(row.Exchange);
		historyCandle.SetStockSymbol(row.Symbol);
		historyCandle.SetLastClose(row.LastClose * ratio);
		historyCandle.SetOpen(row.Open * ratio);
		historyCandle.SetHigh(row.High * ratio);
		historyCandle.SetLow(row.Low * ratio);
		historyCandle.SetClose(row.Close * ratio);
		historyCandle.SetSplitFactor(row.SplitFactor);
		historyCandle.SetDividend(row.Dividend);
		historyCandle.SetUpDown(row.UpAndDown);
		historyCandle.SetVolume(row.Volume);
		historyCandle.SetAmount(row.Amount);
		historyCandle.SetUpDownRate(row.UpDownRate);
		historyCandle.SetChangeHandRate(row.ChangeHandRate);
		historyCandle.SetTotalValue(row.TotalValue);
		historyCandle.SetCurrentValue(row.CurrentValue);
		historyCandle.SetRatio(ratio);
		Add(historyCandle);
	}
	tx.commit();

	m_fDataLoaded = true;
	return true;
}

bool CContainerChinaWeekLine::LoadCurrentWeekLine() {
	using namespace StockMarket;
	const auto& t = ChinaCurrentWeekline{};
	auto db = gl_dbStockMarket.get();
	auto tx = start_transaction(db);

	auto result = db(select(all_of(t)).from(t).unconditionally());
	for (const auto& row : result) {
		CVirtualHistoryCandle historyCandle;
		int ratio = GetRatio();
		historyCandle.Reset();
		historyCandle.SetRatio(ratio);
		historyCandle.SetDate(row.Date);
		historyCandle.SetExchange(row.Exchange);
		historyCandle.SetStockSymbol(row.Symbol);
		historyCandle.SetLastClose(row.LastClose * ratio);
		historyCandle.SetOpen(row.Open * ratio);
		historyCandle.SetHigh(row.High * ratio);
		historyCandle.SetLow(row.Low * ratio);
		historyCandle.SetClose(row.Close * ratio);
		historyCandle.SetSplitFactor(row.SplitFactor);
		historyCandle.SetDividend(row.Dividend);
		historyCandle.SetUpDown(row.UpAndDown);
		historyCandle.SetVolume(row.Volume);
		historyCandle.SetAmount(row.Amount);
		historyCandle.SetUpDownRate(row.UpDownRate);
		historyCandle.SetChangeHandRate(row.ChangeHandRate);
		historyCandle.SetTotalValue(row.TotalValue);
		historyCandle.SetCurrentValue(row.CurrentValue);
		historyCandle.SetRatio(ratio);
		Add(historyCandle);
	}
	tx.commit();

	return true;
}

void CContainerChinaWeekLine::StoreVectorData(const vector<CWeekLine>& vWeekLine) {
	for (const auto& weekLine : vWeekLine) {
		Add(weekLine);
	}
	SetDataLoaded(true);
}

/////////////////////////////////////////////////////////////////////////////////////
//
// 更新日线容器。
//
/////////////////////////////////////////////////////////////////////////////////////
void CContainerChinaWeekLine::UpdateData(const vector<CWeekLine>& vTempWeekLine) {
	Unload(); // 清除已载入的周线数据（如果有的话）
	// 将日线数据以时间为正序存入
	for (const auto& pWeekLine : vTempWeekLine) {
		Add(pWeekLine);
	}
	SetDataLoaded(true);
}

void CContainerChinaWeekLine::UpdateData(const CVirtualHistoryCandle* pHistoryCandle) {
	for (size_t index = 0; index < m_vHistoryData.size(); ++index) {
		auto pWeekLine = GetData(index);
		auto str = pWeekLine->GetStockSymbol();
		if (str == pHistoryCandle->GetStockSymbol()) {
			pWeekLine->UpdateWeekLine(pHistoryCandle);
			break;
		}
	}
}
