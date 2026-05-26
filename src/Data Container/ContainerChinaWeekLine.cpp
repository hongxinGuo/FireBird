#include"pch.h"

#include "ContainerChinaWeekLine.h"

#include "dataBaseConnector.h"
#include"SetWeekLineInfo.h"

#include "InfoReport.h"

namespace {
	CChinaStock s_stock;
}

CContainerChinaWeekLine::CContainerChinaWeekLine() {
	m_ratio = s_stock.GetRatio();
}

bool CContainerChinaWeekLine::SaveDB(const string& strStockSymbol) {
	try {
		CSetWeekLineInfo setWeekLineBasic;
		UpdateBasicDB(&setWeekLineBasic, strStockSymbol);
	} catch (CException& e) {
		ReportInformation(e);
	}

	return true;
}

void CContainerChinaWeekLine::SaveCurrentWeekLine() const {
	using namespace StockMarket;
	const auto& t = ChinaCurrentWeekline{};
	auto db = gl_dbStockMarket.get();
	auto tx = sqlpp::start_transaction(db);
	auto ratio = GetRatio();

	for (const auto& data : m_vHistoryData) {
		db(sqlpp::insert_into(t).set(
			t.Date = data.GetDate(),
			t.Exchange = data.GetExchange(),
			t.Symbol = data.GetStockSymbol(),
			t.LastClose = data.GetLastClose() / ratio,
			t.High = data.GetHigh() / ratio,
			t.Low = data.GetLow() / ratio,
			t.Open = data.GetOpen() / ratio,
			t.Close = data.GetClose() / ratio,
			t.Dividend = data.GetDividend(),
			t.SplitFactor = data.GetSplitFactor(),
			t.Volume = data.GetVolume(),
			t.Amount = data.GetAmount(),
			t.UpAndDown = data.GetUpDown(),
			t.UpDownRate = data.GetUpDownRate(),
			t.ChangeHandRate = data.GetChangeHandRate(),
			t.TotalValue = data.GetTotalValue(),
			t.CurrentValue = data.GetCurrentValue()
		));
	}
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
