#include "pch.h"
#include "ContainerCryptoDayLine.h"

#include "dataBaseConnector.h"

namespace {
	CFinnhubCrypto s_stock;
}

CContainerCryptoDayLine::CContainerCryptoDayLine() {
	m_ratio = s_stock.GetRatio();
}

bool CContainerCryptoDayLine::SaveDB(const string& strCryptoSymbol) {
	using namespace StockMarket;
	const auto& t = FinnhubCryptoDayline{};
	auto db = gl_dbStockMarket.get();
	auto tx = start_transaction(db);

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

	const size_t lSize = Size();

	// no old data: bulk insert all
	for (size_t i = 0; i < lSize; ++i) {
		auto pCandle = GetData(i);
		insertCandle(pCandle);
	}
	tx.commit();

	return true;
}

bool CContainerCryptoDayLine::LoadDB(const string& strCryptoSymbol) {
	using namespace StockMarket;
	const auto& t = FinnhubCryptoDayline{};
	auto db = gl_dbStockMarket.get();
	auto tx = start_transaction(db);

	Reset();
	auto result = db(select(all_of(t)).from(t).where(t.Symbol == strCryptoSymbol).order_by(t.Date.asc()));
	size_t rows = result.size();
	Reserve(rows);

	for (const auto& row : result) {
		// 装入DayLine数据
		CVirtualHistoryCandle historyCandle;
		historyCandle.Reset();
		historyCandle.SetRatio(m_ratio);
		historyCandle.SetDate(row.Date);
		historyCandle.SetStockSymbol(row.Symbol);
		historyCandle.SetLastClose(row.LastClose * m_ratio);
		historyCandle.SetOpen(row.Open * m_ratio);
		historyCandle.SetHigh(row.High * m_ratio);
		historyCandle.SetLow(row.Low * m_ratio);
		historyCandle.SetClose(row.Close * m_ratio);
		historyCandle.SetVolume(row.Volume);
		historyCandle.SetAmount(row.Amount);
		Add(historyCandle);
	}
	tx.commit();
	m_fDataLoaded = true;

	return true;
}
