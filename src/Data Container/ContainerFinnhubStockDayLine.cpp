#include "pch.h"

#include "ContainerFinnhubStockDayLine.h"

namespace {
	CFinnhubStock s_stockContainerFinnhubStockDayLine;
}

CContainerFinnhubStockDayLine::CContainerFinnhubStockDayLine() {
	m_ratio = s_stockContainerFinnhubStockDayLine.GetRatio();
}

void CContainerFinnhubStockDayLine::SaveDB(const string& strStockSymbol) {
	auto ratio = GetRatio();
	using namespace StockMarket;
	const auto& t = FinnhubStockDayline{};
	auto db = gl_dbStockMarket.get();
	auto tx = sqlpp::start_transaction(db);

	// Helper: insert a single candle into DB (ratio applied inside)
	auto multi_insert = insert_into(t).columns(t.Date, t.Exchange, t.Symbol,
	                                           t.LastClose, t.Open, t.High, t.Low, t.Close, t.Volume, t.Amount,
	                                           t.Dividend, t.SplitFactor, t.UpAndDown, t.UpDownRate, t.ChangeHandRate, t.TotalValue, t.CurrentValue);
	auto insertCandle = [&](const CVirtualHistoryCandle* pCandle) {
		multi_insert.values.add(
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
			t.UpAndDown = static_cast<double>(pCandle->GetUpDown()),
			t.UpDownRate = static_cast<double>(pCandle->GetUpDownRate()),
			t.ChangeHandRate = static_cast<double>(pCandle->GetChangeHandRate()),
			t.TotalValue = static_cast<double>(pCandle->GetTotalValue()),
			t.CurrentValue = static_cast<double>(pCandle->GetCurrentValue())
		);
	};

	auto lSize = Size();
	for (size_t i = 0; i < lSize; i++) {
		auto pCandle = GetData(i);
		insertCandle(pCandle);
	}
	if (lSize > 0) db(multi_insert);
	tx.commit();
}

void CContainerFinnhubStockDayLine::LoadDB(const string& strStockSymbol) {
	auto ratio = GetRatio();
	using namespace StockMarket;
	const auto& t = FinnhubStockDayline{};
	auto db = gl_dbStockMarket.get();
	auto tx = sqlpp::start_transaction(db);

	auto result = db(select(all_of(t)).from(t).where(t.Symbol == strStockSymbol).order_by(t.Date.asc()));
	size_t rows = result.size();
	Reserve(rows);
	for (const auto& row : result) {
		CVirtualHistoryCandle candle;
		candle.SetDate(row.Date);
		candle.SetExchange(row.Exchange);
		candle.SetStockSymbol(row.Symbol);
		candle.SetLastClose(row.LastClose * ratio);
		candle.SetOpen(row.Open * ratio);
		candle.SetHigh(row.High * ratio);
		candle.SetLow(row.Low * ratio);
		candle.SetClose(row.Close * ratio);
		candle.SetSplitFactor(row.SplitFactor);
		candle.SetDividend(row.Dividend);
		candle.SetUpDown(row.UpAndDown);
		candle.SetVolume(row.Volume);
		candle.SetAmount(row.Amount);
		candle.SetUpDownRate(row.UpDownRate);
		candle.SetChangeHandRate(row.ChangeHandRate);
		candle.SetTotalValue(row.TotalValue);
		candle.SetCurrentValue(row.CurrentValue);
		Add(candle);
	}
	tx.commit();

	m_fDataLoaded = true;
}
