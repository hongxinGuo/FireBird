#include "pch.h"

#include "ContainerTiingoStock.h"

#include "CharSetTransfer.h"
#include "ConvertToString.h"
#include "SetTiingoStockCurrentTrace.h"
#include "SetTiingoStockDayLine.h"
#include "Thread.h"
#include "ThreadStatus.h"
#include "TimeConvert.h"
#include "WorldMarket.h"

#undef min
#undef max
#include "dataBaseConnector.h"
#include <sqlpp11/sqlpp11.h>

#include "StockMarketSQLTable.h"

CContainerTiingoStock::CContainerTiingoStock() {
	CContainerTiingoStock::Reset();
}

CContainerTiingoStock::~CContainerTiingoStock() {
}

void CContainerTiingoStock::Reset() {
	CContainerVirtualStock::Reset();
}

void CContainerTiingoStock::UpdateProfile(const CTiingoStockPtr& pStock) {
	ASSERT(gl_systemConfiguration.IsPaidTypeTiingoAccount()); // 调用此函数时，必须保证是付费账户。
	ASSERT(IsSymbol(pStock->GetSymbol()));

	auto pTiingoStock = GetStock(pStock->GetSymbol());
	pTiingoStock->UpdateProfile(pStock);
}

/// <summary>
/// 
/// 这种查询方式比较晦涩，但结果正确。目前使用此函数。(可能出现存储多个相同代码的问题，研究之）
///
/// 免费账户和付费账户更新的方式不同。
/// 免费账户只存储新证券即可；付费账户需要更新所有新接收到的数据。
///
/// </summary>
void CContainerTiingoStock::UpdateProfileDB() {
	if (IsUpdateProfileDB()) {
		CSetTiingoStock setTiingoStock;
		setTiingoStock.m_strSort = "[Ticker]";
		setTiingoStock.Open();
		setTiingoStock.m_pDatabase->BeginTrans();
		while (!setTiingoStock.IsEOF()) {	//更新原有的代码集状态
			if (IsSymbol(T2Utf8(setTiingoStock.m_Ticker))) {
				const CTiingoStockPtr pStock = GetStock(T2Utf8(setTiingoStock.m_Ticker));
				ASSERT(pStock != nullptr);
				if (pStock->IsUpdateProfileDB()) {
					pStock->Update(setTiingoStock);
					pStock->SetUpdateProfileDB(false);
				}
			}
			else {
				setTiingoStock.Delete(); // 删除已不存在的代码。
			}
			setTiingoStock.MoveNext();
		}
		for (size_t l = 0; l < m_vStock.size(); l++) {
			const CTiingoStockPtr pStock = GetStock(l);
			ASSERT(pStock != nullptr);
			if (pStock->IsUpdateProfileDB()) {
				pStock->Append(setTiingoStock);
				pStock->SetUpdateProfileDB(false);
				pStock->SetTodayNewStock(false);
			}
		}
		setTiingoStock.m_pDatabase->CommitTrans();
		setTiingoStock.Close();
	}
}

/*
 *void CContainerTiingoStock::UpdateProfileDB() {
	if (IsUpdateProfileDB()) {
		CSetTiingoStock setTiingoStock;
		setTiingoStock.m_strSort = "[Ticker]";
		setTiingoStock.Open();
		setTiingoStock.m_pDatabase->BeginTrans();
		while (!setTiingoStock.IsEOF()) {	//更新原有的代码集状态
			if (IsSymbol(T2Utf8(setTiingoStock.m_Ticker))) {
				const CTiingoStockPtr pStock = GetStock(T2Utf8(setTiingoStock.m_Ticker));
				ASSERT(pStock != nullptr);
				if (pStock->IsUpdateProfileDB()) {
					pStock->Update(setTiingoStock);
					pStock->SetUpdateProfileDB(false);
				}
			}
			else {
				setTiingoStock.Delete(); // 删除已不存在的代码。
			}
			setTiingoStock.MoveNext();
		}
		for (size_t l = 0; l < m_vStock.size(); l++) {
			const CTiingoStockPtr pStock = GetStock(l);
			ASSERT(pStock != nullptr);
			if (pStock->IsUpdateProfileDB()) {
				pStock->Append(setTiingoStock);
				pStock->SetUpdateProfileDB(false);
				pStock->SetTodayNewStock(false);
			}
		}
		setTiingoStock.m_pDatabase->CommitTrans();
		setTiingoStock.Close();
	}
}
 */

bool CContainerTiingoStock::LoadProfileDB2() {
	CSetTiingoStock setTiingoStock;

	Reset();
	setTiingoStock.m_strSort = "[Ticker]";
	setTiingoStock.Open();
	setTiingoStock.m_pDatabase->BeginTrans();
	while (!setTiingoStock.IsEOF()) {
		if (!IsSymbol(T2Utf8(setTiingoStock.m_Ticker))) {
			const auto pTiingoStock = make_shared<CTiingoStock>();
			pTiingoStock->Load(setTiingoStock);
			Add(pTiingoStock);
		}
		else {
			setTiingoStock.Delete(); // 删除重复代码
		}
		setTiingoStock.MoveNext();
	}
	setTiingoStock.m_pDatabase->CommitTrans();
	setTiingoStock.Close();

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////
///
/// 使用sqlpp11的类型化查询接口来加载数据，sqlpp11的查询接口比传统的方式快10倍。
/// 
///
//////////////////////////////////////////////////////////////////////////////////////////////
bool CContainerTiingoStock::LoadProfileDB() {
	// Use sqlpp11 typed query API to load profile data
	bool fFindDuplicatedCode = false;
	try {
		using namespace StockMarket;
		const auto& t = TiingoStockFundamental{};

		Reset();
		auto db = gl_dbStockMarket.get();
		auto tx = start_transaction(db);
		auto result = db(select(t.TiingoPermaTicker, t.Ticker, t.Name, t.IsActive, t.IsADR, t.SICCode, t.SICIndustry, t.SICSector, t.TiingoIndustry, t.TiingoSector, t.ReportingCurrency, t.Location, t.CompanyWebSite, t.SECFilingWebSite, t.IPOStatus, t.UpdateDate).from(t).unconditionally().order_by(t.Ticker.asc()));
		auto rowCount = result.size();
		Reserve(rowCount + 100); // 预留一些空间，避免后续添加新股票时频繁扩容
		for (const auto& row : result) {
			//for (const auto& row : result) {
			const std::string symbol = row.Ticker;
			if (!IsSymbol(symbol)) {
				const auto pTiingoStock = make_shared<CTiingoStock>();

				pTiingoStock->SetTiingoPermaTicker(row.TiingoPermaTicker);
				pTiingoStock->SetSymbol(row.Ticker);
				pTiingoStock->SetName(row.Name);
				pTiingoStock->SetActive(row.IsActive);
				pTiingoStock->SetIsADR(row.IsADR);
				pTiingoStock->SetSicCode(row.SICCode);
				pTiingoStock->SetSicIndustry(row.SICIndustry);
				pTiingoStock->SetSicSector(row.SICSector);
				pTiingoStock->SetTiingoIndustry(row.TiingoIndustry);
				pTiingoStock->SetTiingoSector(row.TiingoSector);
				pTiingoStock->SetReportingCurrency(row.ReportingCurrency);
				pTiingoStock->SetLocation(row.Location);
				pTiingoStock->SetCompanyWebSite(row.CompanyWebSite);
				pTiingoStock->SetSECFilingWebSite(row.SECFilingWebSite);
				pTiingoStock->SetIPOStatus(row.IPOStatus);
				pTiingoStock->LoadUpdateDate(row.UpdateDate);
				pTiingoStock->CheckUpdateStatus(gl_pWorldMarket->GetMarketDate());
				Add(pTiingoStock);
			}
			else {
				fFindDuplicatedCode = true;
			}
		}
		tx.commit();
	} catch (const std::exception& ex) {
		gl_systemMessage.PushErrorMessage(fmt::format("LoadDB(sqlpp11) failed: {}", ex.what()));
		return false;
	}
	if (fFindDuplicatedCode) {
		DeleteDuplicatedStockDB();
	}
	return true;
}

void CContainerTiingoStock::DeleteDuplicatedStockDB() {
	auto db = gl_dbStockMarket.get();
	// Use execute(string) to run raw SQL text (operator() requires a sqlpp statement)
	db.execute("DELETE t1 FROM tiingo_stock_fundamental t1 INNER JOIN tiingo_stock_fundamental t2 ON t1.Ticker = t2.Ticker AND t1.ID > t2.ID");
	db.execute("COMMIT");
}

void CContainerTiingoStock::ResetDayLineStartEndDate() {
	for (size_t i = 0; i < Size(); i++) {
		auto pTiingoStock = GetStock(i);
		pTiingoStock->SetDayLineStartDate(29900101);
		pTiingoStock->SetDayLineEndDate(19800101);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 存储该日的数据
// 只有存储的股票才更新其日线结束日期。
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContainerTiingoStock::BuildDayLine(long lDate) {
	// 使用 sqlpp11 批量插入日线数据
	auto lSize = Size();
	time_t tMarketCloseTime = gl_pWorldMarket->TransferToUTCTime(lDate, 0); // 使用当日数据，无论是否是闭市后的数据。

	// 先载入并删除当天旧数据（与原逻辑保持一致）
	LoadDayLine(lDate);
	DeleteDayLine(lDate);

	try {
		using namespace StockMarket;
		const auto& t = TiingoStockDayline{};

		auto db = gl_dbStockMarket.get();
		auto tx = start_transaction(db);

		for (size_t i = 0; i < lSize; i++) {
			auto pTiingoStock = GetStock(i);
			if (pTiingoStock->GetTransactionTime() >= tMarketCloseTime) {
				// 将内部整数/单位值转换为数据库存储的浮点值（与 LoadDayLine 中的乘比率相反）
				const double ratio = static_cast<double>(pTiingoStock->GetRatio());

				db(insert_into(t).set(
					t.Date = lDate,
					t.Exchange = pTiingoStock->GetExchangeCode(),
					t.Symbol = pTiingoStock->GetSymbol(),
					t.LastClose = static_cast<double>(pTiingoStock->GetLastClose()) / ratio,
					t.Open = static_cast<double>(pTiingoStock->GetOpen()) / ratio,
					t.High = static_cast<double>(pTiingoStock->GetHigh()) / ratio,
					t.Low = static_cast<double>(pTiingoStock->GetLow()) / ratio,
					t.Close = static_cast<double>(pTiingoStock->GetNew()) / ratio,
					t.Volume = pTiingoStock->GetVolume(),
					t.Amount = pTiingoStock->GetAmount(),
					t.Dividend = pTiingoStock->GetDividend(),
					t.SplitFactor = pTiingoStock->GetSplitFactor(),
					t.UpAndDown = static_cast<double>(pTiingoStock->GetUpDown()) / ratio,
					t.UpDownRate = pTiingoStock->GetUpDownRate(),
					t.ChangeHandRate = pTiingoStock->GetChangeHandRate(),
					t.TotalValue = pTiingoStock->GetTotalValue(),
					t.CurrentValue = pTiingoStock->GetCurrentValue()
				));

				// 保持原有对象状态更新
				pTiingoStock->SetDayLineEndDate(lDate);
				pTiingoStock->SetUpdateProfileDB(true);
			}
		}

		tx.commit();
	} catch (const std::exception& ex) {
		gl_systemMessage.PushErrorMessage(fmt::format("BuildDayLine(sqlpp11) failed: {}", ex.what()));
		return;
	}

	gl_systemConfiguration.SetTiingoIEXTopOfBookUpdateDate(lDate);
}

void CContainerTiingoStock::LoadDayLine(long lDate) {
	try {
		using namespace StockMarket;
		const auto& t = TiingoStockDayline{};

		time_t ttTradeDay = ConvertToTTime(lDate, gl_pWorldMarket->GetTimeZone(), 170000); // 美股下午4点收市

		auto db = gl_dbStockMarket.get();
		auto tx = start_transaction(db);

		// select rows for the given trade date
		auto result = db(select(all_of(t)).from(t).where(t.Date == lDate).order_by(t.Symbol.asc()));

		for (const auto& row : result) {
			const std::string symbol = row.Symbol;
			if (IsSymbol(symbol)) {
				auto pStock = GetStock(symbol);
				if (pStock == nullptr) continue;

				pStock->SetTransactionTime(ttTradeDay);

				const double ratio = pStock->GetRatio();

				// numeric fields from sqlpp11 are used directly
				pStock->SetHigh(row.High * ratio);
				pStock->SetLow(row.Low * ratio);
				pStock->SetOpen(row.Open * ratio);
				pStock->SetNew(row.Close * ratio);
				pStock->SetLastClose(row.LastClose * ratio);

				pStock->SetVolume(row.Volume);
				pStock->SetDividend(row.Dividend);
				pStock->SetSplitFactor(row.SplitFactor);
			}
		}
		tx.commit();
	} catch (const std::exception& ex) {
		gl_systemMessage.PushErrorMessage(fmt::format("LoadDayLine(sqlpp11) failed: {}", ex.what()));
	}
}

void CContainerTiingoStock::DeleteDayLine(long lDate) {
	using namespace StockMarket;
	const auto& t = TiingoStockDayline{};

	auto db = gl_dbStockMarket.get();
	auto tx = start_transaction(db);

	// Delete all rows for the given trade date in one statement
	db(remove_from(t).where(t.Date == lDate));
	tx.commit();
}

long CContainerTiingoStock::GetTotalActiveStocks() {
	int iCount = 0;
	auto lSize = Size();
	for (size_t i = 0; i < lSize; i++) {
		if (GetStock(i)->IsActive()) iCount++;
	}
	return iCount;
}

void CContainerTiingoStock::UpdateFinancialStateDB() {
	for (size_t i = 0; i < Size(); i++) {
		auto pStock = GetStock(i);
		if (pStock->IsUpdateFinancialStateDB()) {
			pStock->UpdateFinancialStateDB();
			pStock->SetUpdateFinancialStateDB(false);
		}
	}
}

void CContainerTiingoStock::TaskUpdate52WeekHighDB() {
	gl_systemMessage.PushInnerSystemInformationMessage("52 week high");
	Delete52WeekHighData();
	auto lSize = Size();

	CSetTiingoStock52WeekHigh set52WeekHigh;
	set52WeekHigh.m_strFilter = "[ID] = 1";
	set52WeekHigh.Open();
	set52WeekHigh.m_pDatabase->BeginTrans();

	for (size_t i = 0; i < lSize; i++) {
		if (gl_systemConfiguration.IsExitingSystem()) break; // 如果程序正在退出，则停止存储。
		const CTiingoStockPtr pStock = GetStock(i);
		if (pStock->IsUpdate52WeekHighLowDB()) {
			pStock->Update52WeekHighDB(set52WeekHigh);
		}
	}
	set52WeekHigh.m_pDatabase->CommitTrans();
	set52WeekHigh.Close();

	gl_systemConfiguration.SetTiingoStock52WeekHighLowUpdateDate(gl_pWorldMarket->GetCurrentTradeDate());
	gl_systemMessage.PushInnerSystemInformationMessage("tiingo 52 week high calculated");
}

void CContainerTiingoStock::TaskUpdate52WeekLowDB() {
	gl_systemMessage.PushInnerSystemInformationMessage("52 week low");
	Delete52WeekLowData();
	auto lSize = Size();

	CSetTiingoStock52WeekLow set52WeekLow;
	set52WeekLow.m_strFilter = "[ID] = 1";
	set52WeekLow.Open();
	set52WeekLow.m_pDatabase->BeginTrans();

	for (size_t i = 0; i < lSize; i++) {
		if (gl_systemConfiguration.IsExitingSystem()) break; // 如果程序正在退出，则停止存储。
		const CTiingoStockPtr pStock = GetStock(i);
		if (pStock->IsUpdate52WeekHighLowDB()) {
			pStock->Update52WeekLowDB(set52WeekLow);
		}
	}

	set52WeekLow.m_pDatabase->CommitTrans();
	set52WeekLow.Close();

	gl_systemConfiguration.SetTiingoStock52WeekHighLowUpdateDate(gl_pWorldMarket->GetCurrentTradeDate());
	gl_systemMessage.PushInnerSystemInformationMessage("tiingo 52 week low calculated");
}

void CContainerTiingoStock::TaskCalculate() {
	gl_systemMessage.PushInnerSystemInformationMessage("calculating 52 week low");
	auto lSize = Size();
	CSetTiingoStockCurrentTrace setCurrentTrace;
	setCurrentTrace.m_strFilter = "[ID] = 1";
	setCurrentTrace.Open();
	setCurrentTrace.m_pDatabase->BeginTrans();
	for (size_t index = 0; index < lSize; index++) {
		if (gl_systemConfiguration.IsExitingSystem()) break; // 如果程序正在退出，则停止存储。
		auto pStock = GetStock(index);
		pStock->Load52WeekLowDB();
		if (pStock->IsEnough52WeekLow()) {
		}
		pStock->m_v52WeekLowDate.clear(); //Note 直到这里才清空
	}
	setCurrentTrace.m_pDatabase->CommitTrans();
	setCurrentTrace.Close();
	gl_systemMessage.PushInnerSystemInformationMessage("52 week low Calculated");
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// 使用并行工作协程，加速运行
//
///////////////////////////////////////////////////////////////////////////////////////////
void CContainerTiingoStock::TaskCalculate2() {
	vector<result<int>> results;
	vector<int> vPos;
	gl_systemMessage.PushInnerSystemInformationMessage("calculating 52 week low");
	auto lSize = Size();
	for (int index = 0; index < lSize; index++) {
		gl_BackgroundWorkingThread.acquire();
		auto result = gl_runtime.thread_executor()->submit([this, index] {
			if (gl_systemConfiguration.IsExitingSystem()) return -1;
			gl_ThreadStatus.IncreaseBackGroundWorkingThread();
			bool fFound = false;
			auto pStock = this->GetStock(index);
			pStock->Load52WeekLowDB();
			if (pStock->IsEnough52WeekLow()) {
				fFound = true;
			}
			pStock->m_v52WeekLowDate.clear(); //Note 直到这里才清空
			gl_ThreadStatus.DecreaseBackGroundWorkingThread();
			gl_BackgroundWorkingThread.release();
			if (fFound) return index;
			return -1;
		});
		results.emplace_back(std::move(result));
	}

	for (auto& result2 : results) {
		auto pos = result2.get(); // 在此等待所有工作线程完成
		if (pos != -1) {
			vPos.push_back(pos); // vPos中存储找到的位置。
		}
	}

	CSetTiingoStockCurrentTrace setCurrentTrace1;
	setCurrentTrace1.m_strFilter = fmt::format("[Date] = {:8Ld}", gl_pWorldMarket->GetMarketDate()).c_str();
	setCurrentTrace1.Open();
	setCurrentTrace1.m_pDatabase->BeginTrans();
	while (!setCurrentTrace1.IsEOF()) {
		setCurrentTrace1.Delete();
		setCurrentTrace1.MoveNext();
	}
	setCurrentTrace1.m_pDatabase->CommitTrans();
	setCurrentTrace1.Close();

	CSetTiingoStockCurrentTrace setCurrentTrace;
	setCurrentTrace.m_strFilter = "[ID] = 1";
	setCurrentTrace.Open();
	setCurrentTrace.m_pDatabase->BeginTrans();
	for (auto index = 0; index < vPos.size(); index++) {
		auto pStock = GetStock(vPos.at(index));
		setCurrentTrace.AddNew();
		setCurrentTrace.m_Date = gl_pWorldMarket->GetMarketDate();
		setCurrentTrace.m_Symbol = pStock->GetSymbol().c_str();
		setCurrentTrace.m_SICCode = pStock->GetSicCode();
		setCurrentTrace.Update();
	}
	setCurrentTrace.m_pDatabase->CommitTrans();
	setCurrentTrace.Close();
	gl_systemMessage.PushInnerSystemInformationMessage("52 week low Calculated");
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
// 改正tiingo日线数据中的错误数据。
//
//////////////////////////////////////////////////////////////////////////////////////////////
void CContainerTiingoStock::TaskFixDayLine() {
	for (size_t i = 0; i < m_vStock.size(); i++) {
		auto pStock = GetStock(i);

		CSetTiingoStockDayLine setDayLineBasic;
		long lLastClose = 0;
		auto ratio = pStock->GetRatio();

		// 装入DayLine数据
		setDayLineBasic.m_strFilter = "[Symbol] = '";
		setDayLineBasic.m_strFilter += pStock->GetSymbol().c_str();
		setDayLineBasic.m_strFilter += "'";
		setDayLineBasic.m_strSort = "[Date]";
		setDayLineBasic.Open();
		setDayLineBasic.m_pDatabase->BeginTrans();
		while (!setDayLineBasic.IsEOF()) {
			long currentLastClose = _tstof(setDayLineBasic.m_LastClose) * ratio;
			// 如果本交易日的上一个交易日收盘价为零的话，使用上一个交易日的收盘价修复之。
			if (currentLastClose == 0) {
				setDayLineBasic.Edit();
				setDayLineBasic.m_LastClose = ConvertValueToCString(lLastClose, ratio);
				setDayLineBasic.Update();
			}
			lLastClose = _tstof(setDayLineBasic.m_Close) * ratio;
			setDayLineBasic.MoveNext();
		}
		setDayLineBasic.m_pDatabase->CommitTrans();
		setDayLineBasic.Close();
	}
}

void CContainerTiingoStock::Delete52WeekHighData() {
	CDatabase database;

	database.Open(_T("stock_market"), FALSE, FALSE, _T("ODBC;UID=FireBird;PASSWORD=firebird;charset=utf8mb4"));
	database.BeginTrans();
	database.ExecuteSQL(_T("TRUNCATE `stock_market`.`tiingo_stock_52week_high`;"));
	database.CommitTrans();
	database.Close();
}

void CContainerTiingoStock::Delete52WeekLowData() {
	CDatabase database;

	database.Open(_T("stock_market"), FALSE, FALSE, _T("ODBC;UID=FireBird;PASSWORD=firebird;charset=utf8mb4"));
	database.BeginTrans();
	database.ExecuteSQL(_T("TRUNCATE `stock_market`.`tiingo_stock_52week_low`;"));
	database.CommitTrans();
	database.Close();
}

bool CContainerTiingoStock::IsUpdateFinancialStateDB() noexcept {
	return std::ranges::any_of(m_vStock, [](const CVirtualStockPtr& pStock) { return dynamic_pointer_cast<CTiingoStock>(pStock)->IsUpdateFinancialStateDB(); });
}

bool CContainerTiingoStock::IsUpdate52WeekHighLowDB() noexcept {
	return std::ranges::any_of(m_vStock, [](const CVirtualStockPtr& pStock) { return dynamic_pointer_cast<CTiingoStock>(pStock)->IsUpdate52WeekHighLowDB(); });
}

void CContainerTiingoStock::SetUpdateFinancialState(bool fFlag) {
	for (size_t index = 0; index < m_vStock.size(); index++) {
		auto pStock = GetStock(index);
		pStock->SetUpdateFinancialState(fFlag);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
///
/// 需要控制住线程数量，以利于其他后台线程能够顺利执行。
///
///
/////////////////////////////////////////////////////////////////////////////////////////////
void CContainerTiingoStock::TaskProcessDayLine() {
	gl_systemMessage.PushInnerSystemInformationMessage("开始处理Tiingo日线数据");
	gl_pWorldMarket->ResetNewHighHigher();

	auto lSize = Size();
	auto lastCalculatedDate = gl_systemConfiguration.GetTiingoStockDayLineProcessedDate();
	vector<result<void>> vResults;
	for (size_t index = 0; index < lSize; index++) {
		auto pStock = GetStock(index);
		if (IsEarlyThen(pStock->GetDayLineStartDate(), pStock->GetDayLineEndDate(), 500)) { // 只处理有两年以上日线的股票
			gl_BackgroundWorkingThread.acquire();
			auto result = gl_runtime.thread_executor()->submit([pStock, lastCalculatedDate] {
				gl_ThreadStatus.IncreaseBackGroundWorkingThread();
				if (!gl_systemConfiguration.IsExitingSystem()) {
					pStock->ProcessDayLine(lastCalculatedDate);
				}
				gl_ThreadStatus.DecreaseBackGroundWorkingThread();
				gl_BackgroundWorkingThread.release();
			});
			vResults.emplace_back(std::move(result));
		}
	}
	for (auto& result2 : vResults) {
		result2.get(); // 在这里等待所有的线程执行完毕
	}
	gl_systemConfiguration.SetTiingoStockDayLineProcessedDate(gl_pWorldMarket->GetMarketDate());
	gl_systemMessage.PushInnerSystemInformationMessage("结束处理Tiingo日线任务，开始存储");

	auto result2 = gl_runtime.thread_executor()->submit([] {
		gl_dataContainerTiingoStock.TaskUpdate52WeekHighDB();
	});
	auto result3 = gl_runtime.thread_executor()->submit([] {
		gl_dataContainerTiingoStock.TaskUpdate52WeekLowDB();
	});
	result2.get();
	result3.get();

	for (size_t i = 0; i < gl_dataContainerTiingoStock.Size(); i++) {
		auto pStock = gl_dataContainerTiingoStock.GetStock(i);
		pStock->SetUpdate52WeekHighLowDB(false);
	}

	auto result5 = gl_runtime.thread_executor()->submit([] {
		gl_dataContainerTiingoStock.TaskCalculate();
	});
	result5.get();

	ReportHighHigherRate();
	gl_systemMessage.PushInnerSystemInformationMessage("Tiingo日线数据处理完毕");
}

void CContainerTiingoStock::ReportHighHigherRate() {
	auto s = fmt::format("3月内再创新高数:{:d}, 3月内未再次新高数:{:d}, 比率:{:.2f}", gl_pWorldMarket->GetNewHighHigher(), gl_pWorldMarket->GetNoNewHighHigher(),
	                     static_cast<double>(gl_pWorldMarket->GetNewHighHigher()) / gl_pWorldMarket->GetNoNewHighHigher());
	gl_systemMessage.PushStockMarketInformationMessage(s);
}
