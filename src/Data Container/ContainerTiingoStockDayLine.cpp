#include "pch.h"

#include "ContainerTiingoStockDayLine.h"

#include"dataBaseConnector.h"

namespace {
	CTiingoStock s_stock;
}

CContainerTiingoStockDayLine::CContainerTiingoStockDayLine() {
	m_ratio = s_stock.GetRatio();
}

bool CContainerTiingoStockDayLine::SaveDB(const string& strStockSymbol) {
	auto ratio = GetRatio();

	using namespace StockMarket;
	const auto& t = TiingoStockDayline{};

	auto db = gl_dbStockMarket.get();
	auto tx = start_transaction(db);
	auto multi_insert = insert_into(t).columns(t.Date, t.Exchange, t.Symbol,
	                                           t.LastClose, t.Open, t.High, t.Low, t.Close, t.Dividend, t.SplitFactor,
	                                           t.Volume, t.Amount, t.UpAndDown, t.UpDownRate, t.ChangeHandRate, t.TotalValue, t.CurrentValue);

	// helper to insert one CTiingoCandleLine into DB via sqlpp11
	auto insertCandle = [&](const CTiingoCandleLine* pC) {
		multi_insert.values.add(
			t.Date = pC->GetDate(),
			t.Exchange = pC->GetExchange(),
			t.Symbol = pC->GetStockSymbol(),
			t.LastClose = static_cast<double>(pC->GetLastClose()) / ratio,
			t.Open = static_cast<double>(pC->GetOpen()) / ratio,
			t.High = static_cast<double>(pC->GetHigh()) / ratio,
			t.Low = static_cast<double>(pC->GetLow()) / ratio,
			t.Close = static_cast<double>(pC->GetClose()) / ratio,
			t.Dividend = pC->GetDividend(),
			t.SplitFactor = pC->GetSplitFactor(),
			t.Volume = static_cast<double>(pC->GetVolume()),
			t.Amount = static_cast<double>(pC->GetAmount()),
			t.UpAndDown = pC->GetUpDown(), // note: field name in DB was UpAndDown in original; match whatever sqlpp table defines
			t.UpDownRate = pC->GetUpDownRate(),
			t.ChangeHandRate = pC->GetChangeHandRate(),
			t.TotalValue = static_cast<double>(pC->GetTotalValue()),
			t.CurrentValue = static_cast<double>(pC->GetCurrentValue())
		);
	};

	auto lSize = Size();
	for (size_t i = 0; i < lSize; ++i) {
		const CTiingoCandleLine* pHistoryCandle = GetData(i);
		insertCandle(pHistoryCandle);
	}
	if (lSize > 0) db(multi_insert);
	tx.commit();

	return true;
}

bool CContainerTiingoStockDayLine::LoadDB(const string& strStockSymbol) {
	Unload(); // 卸载之前的日线

	using namespace StockMarket;
	const auto& t = TiingoStockDayline{};

	auto db = gl_dbStockMarket.get();
	auto tx = start_transaction(db);
	auto result = db(select(all_of(t)).from(t).where(t.Symbol == strStockSymbol).order_by(t.Date.asc()));
	Reserve(result.size() + 2);
	for (const auto& row : result) {
		CTiingoCandleLine candle;
		auto ratio = GetRatio();

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
	SplitAdjust();

	return true;
}

void CContainerTiingoStockDayLine::UpdateData(const CTiingoCandleLinesPtr& pvTempDayLine) {
	Unload(); // 清除已载入的日线数据（如果有的话）
	Reserve(pvTempDayLine->size());
	// 将日线数据以时间为正序存入
	for (auto& dayLine : *pvTempDayLine) {
		if (dayLine.IsActive()) {
			// 清除掉不再交易（停牌或退市后出现的）的股票日线
			Add(dayLine);
		}
	}
	m_fDataLoaded = true;
}

struct CSplitFactor {
	long date;
	double factor;
};

void CContainerTiingoStockDayLine::SplitAdjust() {
	ASSERT(IsDataLoaded());
	// 按拆分因子调整日线数据
	vector<shared_ptr<CSplitFactor>> vpSplitFactor;
	double dTotalFactor = 1.0;
	// 找出所有的拆分因子，并计算累计拆分因子。注意，拆分因子是从后向前计算的。
	for (long i = m_vHistoryData.size() - 1; i >= 0; i--) {
		auto data = m_vHistoryData.at(i);
		if (std::abs(data.GetSplitFactor() - 1.0) > EPSILON) {
			dTotalFactor *= data.GetSplitFactor();
			auto p = make_shared<CSplitFactor>();
			p->date = data.GetDate();
			p->factor = dTotalFactor;
			vpSplitFactor.push_back(p);
		}
	}
	if (vpSplitFactor.empty()) return; // 没有拆分因子，直接返回

	// 从后向前调整日线数据
	long j = 0;
	long prevDate;
	int i = m_vHistoryData.size() - 1;
	double currentFactor = 1.0;
	auto currentData = GetData(i);
	auto currentSpiltDate = vpSplitFactor.at(j)->date;
	while (currentData->GetDate() > currentSpiltDate) currentData = GetData(--i); // 找到拆分日的日线数据
	do {
		const double prevFactor = currentFactor;
		currentSpiltDate = vpSplitFactor.at(j)->date;
		currentFactor = vpSplitFactor.at(j)->factor;
		if (j < vpSplitFactor.size() - 1) prevDate = vpSplitFactor.at(j + 1)->date;
		else prevDate = 0;

		ASSERT(currentData->GetDate() == currentSpiltDate);
		currentData->SetLastClose(currentData->GetLastClose() * prevFactor / currentFactor); // 拆分日只有前收盘价需要调整，其他价格不调整。

		while (currentData->GetDate() > prevDate && i > 0) { // 调整拆分日之前的日线数据，直到下一个拆分日（如果有的话）
			currentData = GetData(--i);
			currentData->AdjustByFactor(currentFactor);
		}
		j++;
	} while (i > 0);
}
