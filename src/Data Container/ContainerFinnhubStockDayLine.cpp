#include "pch.h"

#include "ContainerFinnhubStockDayLine.h"

#include "dataBaseConnector.h"
#include "InfoReport.h"
#include"SetFinnhubStockDayLine.h"

namespace {
	CFinnhubStock s_stock;
}

CContainerFinnhubStockDayLine::CContainerFinnhubStockDayLine() {
	m_ratio = s_stock.GetRatio();
}

bool CContainerFinnhubStockDayLine::SaveDB(const string& strStockSymbol) {
	auto ratio = GetRatio();
	using namespace StockMarket;
	const auto& t = FinnhubStockDayline{};
	auto db = gl_dbStockMarket.get();
	auto tx = sqlpp::start_transaction(db);

	// Helper: insert a single candle into DB (ratio applied inside)
	auto insertCandle = [&](const CVirtualHistoryCandle* pCandle) {
		db(insert_into(t).set(
			t.Date = pCandle->GetDate(),
			t.Exchange = pCandle->GetExchange(),
			t.Symbol = pCandle->GetStockSymbol(),
			t.LastClose = static_cast<double>(pCandle->GetLastClose()) / m_ratio,
			t.High = static_cast<double>(pCandle->GetHigh()) / m_ratio,
			t.Low = static_cast<double>(pCandle->GetLow()) / m_ratio,
			t.Open = static_cast<double>(pCandle->GetOpen()) / m_ratio,
			t.Close = static_cast<double>(pCandle->GetClose()) / m_ratio,
			t.Dividend = pCandle->GetDividend(),
			t.SplitFactor = pCandle->GetSplitFactor(),
			t.Volume = pCandle->GetVolume(),
			t.Amount = pCandle->GetAmount(),
			t.UpAndDown = pCandle->GetUpDown(),
			t.UpDownRate = pCandle->GetUpDownRate(),
			t.ChangeHandRate = pCandle->GetChangeHandRate(),
			t.TotalValue = pCandle->GetTotalValue(),
			t.CurrentValue = pCandle->GetCurrentValue()
		));
	};

	auto lSize = Size();
	for (size_t i = 0; i < lSize; i++) {
		auto pCandle = GetData(i);
		insertCandle(pCandle);
	}
	tx.commit();

	return true;
}

bool CContainerFinnhubStockDayLine::LoadDB(const string& strStockSymbol) {
	auto ratio = GetRatio();
	using namespace StockMarket;
	const auto& t = FinnhubStockDayline{};
	auto db = gl_dbStockMarket.get();
	auto tx = sqlpp::start_transaction(db);

	Reset();
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
	return true;
}
