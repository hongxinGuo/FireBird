#include "pch.h"
#include "ContainerForexDayLine.h"

#include "dataBaseConnector.h"
#include "InfoReport.h"

namespace {
	CFinnhubForex s_stock;
}

CContainerForexDayLine::CContainerForexDayLine() {
	m_ratio = s_stock.GetRatio();
}

bool CContainerForexDayLine::SaveDB(const string& strForexSymbol) {
	vector<CTiingoCandleLine> vOldHistoryCandle;
	auto ratio = GetRatio();

	using namespace StockMarket;
	const auto& t = FinnhubForexDayline{};

	auto db = gl_dbStockMarket.get();
	auto tx = start_transaction(db);

	// 读取数据库中已有的日线（按日期正序），并删除重复日期的记录（保留第一个遇到的）
	long lLastDate = 0;
	auto result = db(select(all_of(t)).from(t).where(t.Symbol == strForexSymbol).order_by(t.Date.asc()));
	vOldHistoryCandle.reserve(result.size());
	for (const auto& row : result) {
		if (row.Date > lLastDate) {
			lLastDate = row.Date;
			CTiingoCandleLine candle;
			candle.Reset();
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
			vOldHistoryCandle.push_back(candle);
		}
		else {
			// 删除与该重复行完全匹配的那一条记录（尽量精确匹配以保证只删除重复项）
			db(remove_from(t).where(
				t.Symbol == row.Symbol &&
				t.Date == row.Date &&
				t.Exchange == row.Exchange &&
				t.LastClose == row.LastClose &&
				t.Open == row.Open &&
				t.High == row.High &&
				t.Low == row.Low &&
				t.Close == row.Close &&
				t.SplitFactor == row.SplitFactor &&
				t.Dividend == row.Dividend &&
				t.UpAndDown == row.UpAndDown &&
				t.Volume == row.Volume &&
				t.Amount == row.Amount &&
				t.UpDownRate == row.UpDownRate &&
				t.ChangeHandRate == row.ChangeHandRate &&
				t.TotalValue == row.TotalValue &&
				t.CurrentValue == row.CurrentValue
			));
		}
	}

	// helper to insert one CTiingoCandleLine into DB via sqlpp11
	auto multi_insert = insert_into(t).columns(t.Date, t.Exchange, t.Symbol, t.LastClose, t.Open,
	                                           t.High, t.Low, t.Close, t.Volume, t.Amount, t.Dividend, t.SplitFactor,
	                                           t.UpAndDown, t.UpDownRate, t.ChangeHandRate, t.CurrentValue, t.TotalValue);
	auto insertCandle = [&](const CVirtualHistoryCandle* pC) {
		multi_insert.values.add(
			t.Date = pC->GetDate(),
			t.Exchange = pC->GetExchange(),
			t.Symbol = pC->GetStockSymbol(),
			t.LastClose = static_cast<double>(pC->GetLastClose()) / ratio,
			t.Open = static_cast<double>(pC->GetOpen()) / ratio,
			t.High = static_cast<double>(pC->GetHigh()) / ratio,
			t.Low = static_cast<double>(pC->GetLow()) / ratio,
			t.Close = static_cast<double>(pC->GetClose()) / ratio,
			t.Volume = static_cast<double>(pC->GetVolume()),
			t.Amount = static_cast<double>(pC->GetAmount()),
			t.Dividend = pC->GetDividend(),
			t.SplitFactor = pC->GetSplitFactor(),
			t.UpAndDown = pC->GetUpDown(), // note: field name in DB was UpAndDown in original; match whatever sqlpp table defines
			t.UpDownRate = pC->GetUpDownRate(),
			t.ChangeHandRate = pC->GetChangeHandRate(),
			t.CurrentValue = static_cast<double>(pC->GetCurrentValue()),
			t.TotalValue = static_cast<double>(pC->GetTotalValue())
		);
	};

	const size_t lSize = Size();
	int nValues = 0;
	if (!vOldHistoryCandle.empty()) { // 有旧数据 -> 只插入缺失或更早的条目
		size_t lCurrentPos = 0;
		for (size_t i = 0; i < lSize; ++i) {
			const CVirtualHistoryCandle* pHistoryCandle = GetData(i);

			if (pHistoryCandle->GetDate() < vOldHistoryCandle.front().GetDate()) {
				// 整体更早的新数据：直接插入
				insertCandle(pHistoryCandle);
				nValues++;
			}
			else {
				// 找到在旧数据中的位置（或越过）
				while (lCurrentPos < vOldHistoryCandle.size() &&
					(vOldHistoryCandle.at(lCurrentPos).GetDate() < pHistoryCandle->GetDate())) {
					++lCurrentPos;
				}

				if (lCurrentPos < vOldHistoryCandle.size()) {
					// 如果当前旧数据的日期大于待插入的日期，说明旧数据集中缺失该日期 -> 插入
					if (vOldHistoryCandle.at(lCurrentPos).GetDate() > pHistoryCandle->GetDate()) {
						insertCandle(pHistoryCandle);
						nValues++;
					}
					// 否则数据库已有该日期，不插入
				}
				else {
					// 已越过旧数据末尾，插入剩余所有新数据
					insertCandle(pHistoryCandle);
					nValues++;
				}
			}
		}
	}
	else { // 没有旧数据 -> 直接把所有当前 container 的日线插入
		for (size_t i = 0; i < lSize; ++i) {
			const CVirtualHistoryCandle* pHistoryCandle = GetData(i);
			insertCandle(pHistoryCandle);
			nValues++;
		}
	}
	if (nValues > 0) db(multi_insert);
	tx.commit();

	return true;
}

bool CContainerForexDayLine::LoadDB(const string& strCryptoSymbol) {
	using namespace StockMarket;
	const auto& t = FinnhubForexDayline{};

	Reset();
	auto db = gl_dbStockMarket.get();
	auto tx = start_transaction(db);
	auto result = db(select(all_of(t)).from(t).where(t.Symbol == strCryptoSymbol).order_by(t.Date.asc()));
	Reserve(result.size() + 2);
	for (const auto& row : result) {
		CVirtualHistoryCandle candle;
		auto ratio = GetRatio();

		candle.SetRatio(ratio);
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
