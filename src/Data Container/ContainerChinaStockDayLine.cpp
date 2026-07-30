module;
#include <afx.h>

#include"StockMarketSQLTable.h"
#include"sqlpp23/sqlpp23.h"
module Container.HistoryCandle.ChinaStockDayLine;

import TimeConvert;
import HistoryCandle;
import Stock.ChinaStock;
import HistoryCandle.WeekLine;
import DatabaseConnector;

import std;
using std::vector;

namespace {
	CChinaStock s_stockContainerChinaDayLine;
}

CContainerChinaStockDayLine::CContainerChinaStockDayLine() {
	m_ratio = s_stockContainerChinaDayLine.GetRatio();
}

void CContainerChinaStockDayLine::SaveDB(const string& strStockSymbol) {
	using namespace StockMarket;
	const auto& t = ChinaStockDayline{};
	auto db = gl_dbStockMarket.get();
	auto tx = start_transaction(db);
	auto multi_insert = insert_into(t).columns(t.Date, t.Exchange, t.Symbol,
	                                           t.LastClose, t.Open, t.High, t.Low, t.Close, t.Volume, t.Amount, t.Dividend, t.SplitFactor,
	                                           t.UpAndDown, t.UpDownRate, t.ChangeHandRate, t.TotalValue, t.CurrentValue);

	// Helper: insert a single candle into DB (ratio applied inside)
	auto insertCandle = [&](const CVirtualHistoryCandle* pCandle) {
		multi_insert.add_values(
			t.Date = toFormattedDate(pCandle->GetDate()),
			t.Exchange = pCandle->GetExchange(),
			t.Symbol = pCandle->GetStockSymbol(),
			t.LastClose = static_cast<double>(pCandle->GetLastClose()) / m_ratio,
			t.Open = static_cast<double>(pCandle->GetOpen()) / m_ratio,
			t.High = static_cast<double>(pCandle->GetHigh()) / m_ratio,
			t.Low = static_cast<double>(pCandle->GetLow()) / m_ratio,
			t.Close = static_cast<double>(pCandle->GetClose()) / m_ratio,
			t.Volume = static_cast<double>(pCandle->GetVolume()),
			t.Amount = static_cast<double>(pCandle->GetAmount()),
			t.Dividend = pCandle->GetDividend(),
			t.SplitFactor = pCandle->GetSplitFactor(),
			t.UpAndDown = pCandle->GetUpDown(),
			t.UpDownRate = pCandle->GetUpDownRate(),
			t.ChangeHandRate = pCandle->GetChangeHandRate(),
			t.TotalValue = static_cast<double>(pCandle->GetTotalValue()),
			t.CurrentValue = static_cast<double>(pCandle->GetCurrentValue())
		);
	};

	size_t lSize = Size();
	for (size_t i = 0; i < lSize; ++i) {
		auto pCandle = GetData(i);
		insertCandle(pCandle);
	}
	if (lSize > 0) db(multi_insert);
	tx.commit();
}

void CContainerChinaStockDayLine::LoadDB(const string& strStockSymbol) {
	using namespace StockMarket;
	const auto& t = ChinaStockDayline{};
	auto db = gl_dbStockMarket.get();
	auto tx = sqlpp::start_transaction(db);

	auto result = db(select(all_of(t)).from(t).where(t.Symbol == strStockSymbol).order_by(t.Date.asc()));
	size_t rows = result.size();
	Reserve(rows + 10);

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
}

void CContainerChinaStockDayLine::LoadDB(const string& strStockSymbol, long lStartDate) {
	using namespace StockMarket;
	const auto& t = ChinaStockDayline{};
	auto db = gl_dbStockMarket.get();
	auto tx = sqlpp::start_transaction(db);

	auto result = db(select(all_of(t)).from(t).where(t.Symbol == strStockSymbol && t.Date >= static_cast<int>(lStartDate)).order_by(t.Date.asc()));
	//size_t rows = result.size();
	Reserve(3000);

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
}

bool CContainerChinaStockDayLine::BuildWeekLine(vector<CWeekLine>& vWeekLine) {
	ASSERT(IsDataLoaded());
	ASSERT(Size() > 0);
	size_t currentDayLinePos = 0;

	vWeekLine.clear();
	do {
		CWeekLine pWeekLine = CreateNewWeekLine(currentDayLinePos);
		vWeekLine.push_back(pWeekLine);
	} while (currentDayLinePos < Size());

	return true;
}

CWeekLine CContainerChinaStockDayLine::CreateNewWeekLine(size_t& currentDayLinePos) {
	ASSERT(Size() > 0);
	ASSERT(currentDayLinePos < Size());
	const auto lNextMonday = GetNextMonday(GetData(currentDayLinePos)->GetDate());
	const auto lNewestDay = GetData(Size() - 1)->GetDate();
	CWeekLine weekLine;
	if (lNextMonday < lNewestDay) {
		// 中间数据
		while (GetData(currentDayLinePos)->GetDate() < lNextMonday) {
			weekLine.UpdateWeekLine(GetData(currentDayLinePos++));
		}
	}
	else {
		// 最后一组数据
		while (currentDayLinePos <= (Size() - 1)) {
			weekLine.UpdateWeekLine(GetData(currentDayLinePos++));
		}
	}

	if (weekLine.GetLastClose() > 0) {
		weekLine.SetUpDownRate(weekLine.GetUpDown() * 100 * 1000 / weekLine.GetLastClose());
	}
	else {
		weekLine.SetUpDownRate(weekLine.GetUpDown() * 100 * 1000 / weekLine.GetOpen());
	}

	return weekLine;
}
