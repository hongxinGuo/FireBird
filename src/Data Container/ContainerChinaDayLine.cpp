#include"pch.h"

#include"TimeConvert.h"

#include "ContainerChinaDayLine.h"

#include "dataBaseConnector.h"
#include"SetChinaMarketDayLineInfo.h"

#include "InfoReport.h"

namespace {
	CChinaStock s_stock;
}

CContainerChinaDayLine::CContainerChinaDayLine() {
	m_ratio = s_stock.GetRatio();
}

bool CContainerChinaDayLine::SaveDB(const string& strStockSymbol) {
	using namespace StockMarket;
	const auto& t = ChinaStockDayline{};
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

	size_t lSize = Size();
	for (size_t i = 0; i < lSize; ++i) {
		auto pCandle = GetData(i);
		insertCandle(pCandle);
	}
	tx.commit();

	return true;
}

bool CContainerChinaDayLine::LoadDB(const string& strStockSymbol) {
	using namespace StockMarket;
	const auto& t = ChinaStockDayline{};
	auto db = gl_dbStockMarket.get();
	auto tx = sqlpp::start_transaction(db);

	Reset();
	auto result = db(select(all_of(t)).from(t).where(t.Symbol == strStockSymbol).order_by(t.Date.asc()));
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

bool CContainerChinaDayLine::UpdateDB(const string& strStockSymbol) {
	// NOTE:
	// - This branch shows a sqlpp11-based implementation sketch.
	// - It is guarded by USE_SQLPP11 so builds don't break until you
	//   provide a real sqlpp11 connection accessor on CVirtualSetHistoryCandle
	//   (for example: GetSqlppConnection()) and table wrapper (GetSqlppTable()).
	// - Replace the placeholder calls below with your project's actual sqlpp11 types.
	//
	// Example integration points you must provide:
	// - sqlpp::connection object accessible from pSetHistoryCandle (or a global DB manager).
	// - a sqlpp11 table struct that maps to the DayLine/HistoryCandle table with columns:
	//   symbol, date, open, high, low, close, volume, ... (names used below are illustrative).
	//
	// The implementation below:
	// 1. queries existing rows for the symbol ordered by date
	// 2. deduplicates same-date rows
	// 3. inserts any missing dates from this virtual container
	// 4. returns true if any new rows were appended (fNeedUpdate)
	//

	bool fNeedUpdate = false;

	if (Size() == 0) {
		return false;
	}

	using namespace StockMarket;
	const auto& t = ChinaStockDayline{};
	auto db = gl_dbStockMarket.get();
	auto tx = start_transaction(db);

	// Helper: load old history from DB for the symbol into vector<CVirtualHistoryCandle>
	vector<CVirtualHistoryCandle> vOldHistoryCandle;
	auto loadOldHistory = [&](const string& symbol) {
		if (symbol.empty()) return;
		auto result = db(select(all_of(t)).from(t).where(t.Symbol == symbol).order_by(t.Date.asc()));
		size_t rows = result.size();
		vOldHistoryCandle.reserve(rows);
		for (const auto& row : result) {
			CVirtualHistoryCandle historyCandle;
			historyCandle.SetRatio(m_ratio);
			historyCandle.SetDate(row.Date);
			historyCandle.SetExchange(row.Exchange);
			historyCandle.SetStockSymbol(row.Symbol);
			historyCandle.SetLastClose(row.LastClose * m_ratio);
			historyCandle.SetOpen(row.Open * m_ratio);
			historyCandle.SetHigh(row.High * m_ratio);
			historyCandle.SetLow(row.Low * m_ratio);
			historyCandle.SetClose(row.Close * m_ratio);
			historyCandle.SetSplitFactor(row.SplitFactor);
			historyCandle.SetDividend(row.Dividend);
			historyCandle.SetUpDown(row.UpAndDown);
			historyCandle.SetVolume(row.Volume);
			historyCandle.SetAmount(row.Amount);
			historyCandle.SetUpDownRate(row.UpDownRate);
			historyCandle.SetChangeHandRate(row.ChangeHandRate);
			historyCandle.SetTotalValue(row.TotalValue);
			historyCandle.SetCurrentValue(row.CurrentValue);
			// Deduplicate by date on the fly if necessary (SQL could also deduplicate)
			if (vOldHistoryCandle.empty() || vOldHistoryCandle.back().GetDate() < historyCandle.GetDate()) {
				vOldHistoryCandle.push_back(historyCandle);
			}
			else {
				//delete duplicate rows via db(remove_from(table).where(table.id == row.id))
				db(sqlpp::remove_from(t).where(t.ID == row.ID));
			}
		}
	};

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

	if (!strStockSymbol.empty()) {
		loadOldHistory(strStockSymbol);
	}

	// Insert missing/new data.
	long lSizeOfOldDayLine = static_cast<long>(vOldHistoryCandle.size());
	const size_t lSize = Size();

	try {
		if (lSizeOfOldDayLine > 0) {
			long lCurrentPos = 0;
			for (size_t i = 0; i < lSize; ++i) {
				CVirtualHistoryCandle* pCandle = GetData(i);
				const auto newDate = pCandle->GetDate();
				if (newDate < vOldHistoryCandle.at(0).GetDate()) {
					// insert full record (this is before existing DB range)
					insertCandle(pCandle);
					// note: original logic didn't set fNeedUpdate here; keep same behavior
				}
				else {
					while ((lCurrentPos < lSizeOfOldDayLine) && (vOldHistoryCandle.at(lCurrentPos).GetDate() < newDate)) ++lCurrentPos;
					if (lCurrentPos < lSizeOfOldDayLine) {
						if (vOldHistoryCandle.at(lCurrentPos).GetDate() > newDate) {
							insertCandle(pCandle);
							fNeedUpdate = true;
						}
						// else dates equal -> skip (duplicate)
					}
					else {
						// past end of old data -> insert
						insertCandle(pCandle);
						fNeedUpdate = true;
					}
				}
			}
		}
		else {
			// no old data: bulk insert all
			for (size_t i = 0; i < lSize; ++i) {
				auto pCandle = GetData(i);
				insertCandle(pCandle);
				fNeedUpdate = true;
			}
		}
		tx.commit();
	} catch (...) {
		tx.rollback();
		throw;
	}

	return fNeedUpdate;
}

bool CContainerChinaDayLine::BuildWeekLine(vector<CWeekLine>& vWeekLine) {
	ASSERT(IsDataLoaded());
	ASSERT(Size() > 0);
	long lCurrentDayLinePos = 0;
	CWeekLine pWeekLine;

	vWeekLine.clear();
	do {
		pWeekLine = CreateNewWeekLine(lCurrentDayLinePos);
		vWeekLine.push_back(pWeekLine);
	} while (lCurrentDayLinePos < Size());

	return true;
}

CWeekLine CContainerChinaDayLine::CreateNewWeekLine(long& lCurrentDayLinePos) {
	ASSERT(Size() > 0);
	ASSERT(lCurrentDayLinePos < Size());
	auto data = GetData(lCurrentDayLinePos);
	const long lNextMonday = GetNextMonday(GetData(lCurrentDayLinePos)->GetDate());
	const long lNewestDay = GetData(Size() - 1)->GetDate();
	CWeekLine weekLine;
	if (lNextMonday < lNewestDay) {
		// 中间数据
		while (GetData(lCurrentDayLinePos)->GetDate() < lNextMonday) {
			weekLine.UpdateWeekLine(GetData(lCurrentDayLinePos++));
		}
	}
	else {
		// 最后一组数据
		while (lCurrentDayLinePos <= (Size() - 1)) {
			weekLine.UpdateWeekLine(GetData(lCurrentDayLinePos++));
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
