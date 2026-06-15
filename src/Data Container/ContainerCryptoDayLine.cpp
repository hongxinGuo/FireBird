#include "pch.h"
#include "ContainerCryptoDayLine.h"

#include "dataBaseConnector.h"

namespace {
	CFinnhubCrypto s_stock;
}

CContainerCryptoDayLine::CContainerCryptoDayLine() {
	m_ratio = s_stock.GetRatio();
}

void CContainerCryptoDayLine::SaveDB(const string& strCryptoSymbol) {
	using namespace StockMarket;
	const auto& t = FinnhubCryptoDayline{};
	auto db = gl_dbStockMarket.get();
	auto tx = start_transaction(db);

	// Helper: insert a single candle into DB (ratio applied inside)
	auto multi_insert = insert_into(t).columns(t.Date, t.Exchange, t.Symbol,
	                                           t.LastClose, t.Open, t.High, t.Low, t.Close, t.Dividend, t.SplitFactor,
	                                           t.Volume, t.Amount, t.UpAndDown, t.UpDownRate, t.ChangeHandRate, t.TotalValue, t.CurrentValue);

	const size_t lSize = Size();
	int nValues = 0;
	// no old data: bulk insert all
	for (size_t i = 0; i < lSize; ++i) {
		auto pCandle = GetData(i);
		multi_insert.values.add(
			t.Date = static_cast<long>(toUnsignedDate(pCandle->GetDate())),
			t.Exchange = pCandle->GetExchange(),
			t.Symbol = pCandle->GetStockSymbol(),
			t.LastClose = static_cast<double>(pCandle->GetLastClose()) / m_ratio,
			t.Open = static_cast<double>(pCandle->GetOpen()) / m_ratio,
			t.High = static_cast<double>(pCandle->GetHigh()) / m_ratio,
			t.Low = static_cast<double>(pCandle->GetLow()) / m_ratio,
			t.Close = static_cast<double>(pCandle->GetClose()) / m_ratio,
			t.Dividend = pCandle->GetDividend(),
			t.SplitFactor = pCandle->GetSplitFactor(),
			t.Volume = static_cast<double>(pCandle->GetVolume()),
			t.Amount = static_cast<double>(pCandle->GetAmount()),
			t.UpAndDown = pCandle->GetUpDown(),
			t.UpDownRate = pCandle->GetUpDownRate(),
			t.ChangeHandRate = pCandle->GetChangeHandRate(),
			t.TotalValue = static_cast<double>(pCandle->GetTotalValue()),
			t.CurrentValue = static_cast<double>(pCandle->GetCurrentValue())
		);
		nValues++;
	}
	if (nValues > 0) db(multi_insert);
	tx.commit();
}

void CContainerCryptoDayLine::LoadDB(const string& strCryptoSymbol) {
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
}
