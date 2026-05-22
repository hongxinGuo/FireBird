#include "pch.h"

#include "ContainerTiingoStockDayLine.h"

#include "InfoReport.h"
#include"SetTiingoStockDayLine.h"

#include"dataBaseConnector.h"
#include"StockMarketSQLTable.h"

namespace {
	CTiingoStock s_stock;
}

CContainerTiingoStockDayLine::CContainerTiingoStockDayLine() {
	m_ratio = s_stock.GetRatio();
}

bool CContainerTiingoStockDayLine::SaveDB(const string& strStockSymbol) {
	try {
		CSetTiingoStockDayLine setDayLine;
		UpdateDB(&setDayLine, strStockSymbol);
	} catch (CException& e) {
		ReportInformation(e);
	}

	return true;
}

bool CContainerTiingoStockDayLine::LoadDB2(const string& strStockSymbol) {
	CSetTiingoStockDayLine setDayLineBasic;

	Unload(); // 卸载之前的日线
	Reserve(3000);

	setDayLineBasic.m_strFilter = "[Symbol] = '";
	setDayLineBasic.m_strFilter += strStockSymbol.c_str();
	setDayLineBasic.m_strFilter += "'";
	setDayLineBasic.m_strSort = "[Date]";
	setDayLineBasic.Open();
	// 装入DayLine数据
	while (!setDayLineBasic.IsEOF()) {
		CTiingoCandleLine candle;
		candle.LoadBasicData(&setDayLineBasic);
		Add(candle);
		setDayLineBasic.MoveNext();
	}
	m_fDataLoaded = true;

	SplitAdjust();
	setDayLineBasic.Close();

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

/////////////////////////////////////////////////////////////////////////////////////////
//
//	将日线历史数据存入数据库．默认数据库为空。
//  此函数被工作线程调用，需要注意数据同步问题。
//  当存在旧日线历史数据时，本函数只是更新。
//
//  具体操作的数据表由第一个参数传入，
//  自动删除旧数据中的重复数据。
//
//Note  更新完后，本container中的日线数据也自动更新为最新数据(以备以后的处理日线数据）
//
//////////////////////////////////////////////////////////////////////////////////////////
void CContainerTiingoStockDayLine::UpdateDB(CSetTiingoStockDayLine* pSetTiingoStockDayLine, const string& strStockSymbol) {
	vector<CTiingoCandleLine> vOldHistoryCandle;
	vOldHistoryCandle.reserve(3000);

	const CTiingoCandleLine* pHistoryCandle = nullptr;
	long lSizeOfOldDayLine = 0;

	const size_t lSize = Size();
	long lLastDate = 0;
	pSetTiingoStockDayLine->m_strFilter = "[Symbol] = '";
	pSetTiingoStockDayLine->m_strFilter += strStockSymbol.c_str();
	pSetTiingoStockDayLine->m_strFilter += "'";
	pSetTiingoStockDayLine->m_strSort = "[Date]";

	pSetTiingoStockDayLine->Open();
	pSetTiingoStockDayLine->m_pDatabase->BeginTrans();
	while (!pSetTiingoStockDayLine->IsEOF()) {
		if (pSetTiingoStockDayLine->m_Date > lLastDate) {
			lLastDate = pSetTiingoStockDayLine->m_Date;
			CTiingoCandleLine candle;
			candle.Reset();
			candle.LoadBasicData(pSetTiingoStockDayLine);

			vOldHistoryCandle.push_back(candle);
			lSizeOfOldDayLine++;
		}
		else {
			pSetTiingoStockDayLine->Delete(); //删除日期重复的数据
		}
		pSetTiingoStockDayLine->MoveNext();
	}

	if (lSizeOfOldDayLine > 0) {// 有旧数据
		long lCurrentPos = 0;
		for (size_t i = 0; i < lSize; i++) {	// 数据是正序存储的，需要从头部开始存储
			pHistoryCandle = GetData(i);
			auto a = pHistoryCandle->GetRatio();

			if (pHistoryCandle->GetDate() < vOldHistoryCandle.at(0).GetDate()) {	// 有更早的新数据？
				pHistoryCandle->AppendBasicData(pSetTiingoStockDayLine);
			}
			else {
				while ((lCurrentPos < lSizeOfOldDayLine) && (vOldHistoryCandle.at(lCurrentPos).GetDate() < pHistoryCandle->GetDate())) lCurrentPos++;
				if (lCurrentPos < lSizeOfOldDayLine) {
					if (vOldHistoryCandle.at(lCurrentPos).GetDate() > pHistoryCandle->GetDate()) { // 前数据集中有遗漏的日期
						pHistoryCandle->AppendBasicData(pSetTiingoStockDayLine);
					}
				}
				else {
					pHistoryCandle->AppendBasicData(pSetTiingoStockDayLine);
				}
			}
		}
	}
	else {// 没有旧数据
		for (size_t i = 0; i < lSize; i++) {	// 数据是正序存储的，需要从头部开始存储
			pHistoryCandle = GetData(i);
			pHistoryCandle->AppendBasicData(pSetTiingoStockDayLine);
		}
	}
	pSetTiingoStockDayLine->m_pDatabase->CommitTrans();
	pSetTiingoStockDayLine->Close();
}

void CContainerTiingoStockDayLine::UpdateDB2(const string& strStockSymbol) {
	vector<CTiingoCandleLine> vOldHistoryCandle;

	using namespace StockMarket;
	const auto& t = TiingoStockDayline{};

	auto db = gl_dbStockMarket.get();
	auto tx = start_transaction(db);

	// 读取数据库中已有的日线（按日期正序），并删除重复日期的记录（保留第一个遇到的）
	long lLastDate = 0;
	auto result = db(select(all_of(t)).from(t).where(t.Symbol == strStockSymbol).order_by(t.Date.asc()));
	vOldHistoryCandle.reserve(result.size());
	for (const auto& row : result) {
		if (row.Date > lLastDate) {
			lLastDate = row.Date;
			CTiingoCandleLine candle;
			candle.Reset();
			candle.SetDate(row.Date);
			candle.SetExchange(row.Exchange);
			candle.SetStockSymbol(row.Symbol);
			candle.SetLastClose(row.LastClose);
			candle.SetOpen(row.Open);
			candle.SetHigh(row.High);
			candle.SetLow(row.Low);
			candle.SetClose(row.Close);
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
	auto insertCandle = [&](const CTiingoCandleLine* pC) {
		db(insert_into(t).set(
			t.Date = pC->GetDate(),
			t.Exchange = pC->GetExchange(),
			t.Symbol = pC->GetStockSymbol(),
			t.LastClose = pC->GetLastClose(),
			t.Open = pC->GetOpen(),
			t.High = pC->GetHigh(),
			t.Low = pC->GetLow(),
			t.Close = pC->GetClose(),
			t.SplitFactor = pC->GetSplitFactor(),
			t.Dividend = pC->GetDividend(),
			t.UpAndDown = pC->GetUpDown(), // note: field name in DB was UpAndDown in original; match whatever sqlpp table defines
			t.Volume = pC->GetVolume(),
			t.Amount = pC->GetAmount(),
			t.UpDownRate = pC->GetUpDownRate(),
			t.ChangeHandRate = pC->GetChangeHandRate(),
			t.TotalValue = pC->GetTotalValue(),
			t.CurrentValue = pC->GetCurrentValue()
		));
	};

	const size_t lSize = Size();

	if (!vOldHistoryCandle.empty()) { // 有旧数据 -> 只插入缺失或更早的条目
		size_t lCurrentPos = 0;
		for (size_t i = 0; i < lSize; ++i) {
			const CTiingoCandleLine* pHistoryCandle = GetData(i);

			if (pHistoryCandle->GetDate() < vOldHistoryCandle.front().GetDate()) {
				// 整体更早的新数据：直接插入
				insertCandle(pHistoryCandle);
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
					}
					// 否则数据库已有该日期，不插入
				}
				else {
					// 已越过旧数据末尾，插入剩余所有新数据
					insertCandle(pHistoryCandle);
				}
			}
		}
	}
	else { // 没有旧数据 -> 直接把所有当前 container 的日线插入
		for (size_t i = 0; i < lSize; ++i) {
			const CTiingoCandleLine* pHistoryCandle = GetData(i);
			insertCandle(pHistoryCandle);
		}
	}

	tx.commit();
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
