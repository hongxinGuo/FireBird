#include "pch.h"

#include "ContainerTiingoStock.h"

#include "Thread.h"
#include "ThreadStatus.h"
#include "TimeConvert.h"
#include "WorldMarket.h"

#include "dataBaseConnector.h"

CContainerTiingoStock::CContainerTiingoStock() {
	CContainerTiingoStock::Reset();
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
	ASSERT(IsUpdateProfileDB());

	set<string> setExistingSymbols;

	using namespace StockMarket;
	const auto& t = TiingoStockProfile{};
	auto db = gl_dbStockMarket.get();
	auto tx = start_transaction(db);

	for (const auto& row : db(select(all_of(t)).from(t).unconditionally())) {
		setExistingSymbols.insert(row.Symbol);
	}

	for (size_t l = 0; l < m_vStock.size(); l++) {
		const CTiingoStockPtr pStock = GetStock(l);
		ASSERT(pStock != nullptr);
		if (pStock->IsUpdateProfileDB()) {
			int IsActive = pStock->IsActive() ? 1 : 0;
			int IsADR = pStock->IsADR() ? 1 : 0;
			if (setExistingSymbols.contains(pStock->GetSymbol())) { // 如果是原有的代码，则更新；如果是新代码，则插入。
				pStock->UpdateJsonUpdateDate();
				db(update(t).set(
					t.TiingoPermaTicker = pStock->GetTiingoPermaTicker(),
					t.Name = pStock->GetName(),
					t.IsActive = IsActive,
					t.IsADR = IsADR,
					t.SICCode = pStock->GetSicCode(),
					t.SICIndustry = pStock->GetSicIndustry(),
					t.SICSector = pStock->GetSicSector(),
					t.TiingoIndustry = pStock->GetTiingoIndustry(),
					t.TiingoSector = pStock->GetTiingoSector(),
					t.ReportingCurrency = pStock->GetReportingCurrency(),
					t.Location = pStock->GetLocation(),
					t.CompanyWebSite = pStock->GetCompanyWebSite(),
					t.SECFilingWebSite = pStock->GetSECFilingWebSite(),
					t.UpdateDate = pStock->GetJsonUpdateDate().dump()
				).where(t.Symbol == pStock->GetSymbol()));
			}
			else { // 新代码，插入。
				pStock->UpdateJsonUpdateDate();
				string sUpdateDate = pStock->GetJsonUpdateDate().dump();
				db(insert_into(t).set(
					t.Symbol = pStock->GetSymbol(),
					t.TiingoPermaTicker = pStock->GetTiingoPermaTicker(),
					t.Name = pStock->GetName(),
					t.IsActive = IsActive,
					t.IsADR = IsADR,
					t.SICCode = pStock->GetSicCode(),
					t.SICIndustry = pStock->GetSicIndustry(),
					t.SICSector = pStock->GetSicSector(),
					t.TiingoIndustry = pStock->GetTiingoIndustry(),
					t.TiingoSector = pStock->GetTiingoSector(),
					t.ReportingCurrency = pStock->GetReportingCurrency(),
					t.Location = pStock->GetLocation(),
					t.CompanyWebSite = pStock->GetCompanyWebSite(),
					t.SECFilingWebSite = pStock->GetSECFilingWebSite(),
					t.UpdateDate = pStock->GetJsonUpdateDate().dump()
				));
				pStock->SetNewStock(false);
			}
			pStock->SetUpdateProfileDB(false);
		}
	}
	tx.commit();
}

//////////////////////////////////////////////////////////////////////////////////////////////
///
/// 使用sqlpp11的类型化查询接口来加载数据，sqlpp11的查询接口比传统的方式快10倍。
/// 
///
//////////////////////////////////////////////////////////////////////////////////////////////
bool CContainerTiingoStock::LoadProfileDB() {
	// Use sqlpp11 typed query API to load profile data
	try {
		using namespace StockMarket;
		const auto& t = TiingoStockProfile{};

		Reset();
		auto db = gl_dbStockMarket.get();
		auto tx = start_transaction(db);
		auto result = db(select(all_of(t)).from(t).unconditionally().order_by(t.ID.asc()));
		auto rowCount = result.size();
		Reserve(rowCount + 100); // 预留一些空间，避免后续添加新股票时频繁扩容
		for (const auto& row : result) {
			//for (const auto& row : result) {
			const std::string symbol = row.Symbol;
			if (!IsSymbol(symbol)) {
				const auto pTiingoStock = make_shared<CTiingoStock>();
				pTiingoStock->SetTiingoPermaTicker(row.TiingoPermaTicker);
				pTiingoStock->SetSymbol(row.Symbol);
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
				pTiingoStock->LoadUpdateDate(row.UpdateDate);
				pTiingoStock->CheckUpdateStatus(gl_pWorldMarket->GetMarketDate());
				Add(pTiingoStock);
			}
			else {
				db(sqlpp::remove_from(t).where(t.ID == row.ID)); // 如果数据库中存在重复的股票代码，则删除重复的记录。
			}
		}
		tx.commit();
		Sort();
	} catch (const std::exception& ex) {
		gl_systemMessage.PushErrorMessage(std::format("LoadDB(sqlpp11) failed: {}", ex.what()));
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
///
/// 删除数据库中重复的股票代码。由于之前的代码逻辑问题，可能会在数据库中存储多个相同代码的股票简介，此函数用来删除这些重复的股票代码。
/// Note: MySQL数据库不允许在同一张表中使用DELETE语句删除重复的记录，因此需要使用JOIN的方式来删除重复的记录。
/// 具体来说，使用INNER JOIN将表自身连接起来，找到重复的记录，并删除其中一个记录。
///
///
//////////////////////////////////////////////////////////////////////////////////////////////////
void CContainerTiingoStock::DeleteDuplicatedSymbolFromDB() {
	auto db = gl_dbStockMarket.get();
	// Use execute(string) to run raw SQL text (operator() requires a sqlpp statement)
	db.execute("DELETE t1 FROM tiingo_stock_profile t1 INNER JOIN tiingo_stock_profile t2 ON t1.Symbol = t2.Symbol AND t1.ID > t2.ID");
	db.execute("COMMIT");
}

void CContainerTiingoStock::ResetDayLineStartEndDate() {
	for (size_t i = 0; i < Size(); i++) {
		auto pTiingoStock = GetStock(i);
		pTiingoStock->SetDayLineStartDate(toLocalDays(29900101));
		pTiingoStock->SetDayLineEndDate(toLocalDays(19800101));
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// 存储该日的数据
///Note：只存储该日日线数据，但不更新各种标识。
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CContainerTiingoStock::BuildDayLine(chrono::local_days date) {
	auto lSize = Size();
	chrono::sys_seconds st = gl_pWorldMarket->ToSysTime(toLocalDateTime(date, chrono::local_seconds(chrono::seconds(0)))); // 使用当日数据，无论是否是闭市后的数据。

	// 删除当天旧数据（与原逻辑保持一致）
	DeleteDayLine(date);

	try {
		using namespace StockMarket;
		const auto& t = TiingoStockDayline{};

		auto db = gl_dbStockMarket.get();
		auto tx = start_transaction(db);
		auto multi_insert = insert_into(t).columns(t.Date, t.Exchange, t.Symbol, t.LastClose, t.Open,
		                                           t.High, t.Low, t.Close, t.Volume, t.Amount, t.Dividend, t.SplitFactor,
		                                           t.UpAndDown, t.UpDownRate, t.ChangeHandRate, t.CurrentValue, t.TotalValue);

		int nValues = 0;
		for (size_t i = 0; i < lSize; i++) {
			auto pTiingoStock = GetStock(i);
			//if (pTiingoStock->GetTransactionTime() >= tMarketCloseTime) {
			if (pTiingoStock->GetTimePoint() >= st) {
				// 将内部整数/单位值转换为数据库存储的浮点值（与 LoadDayLine 中的乘比率相反）
				const double ratio = static_cast<double>(pTiingoStock->GetRatio());
				//ASSERT(pTiingoStock->GetLastClose() != 0);
				//ASSERT(pTiingoStock->GetNew() != 0);
				multi_insert.values.add(
					t.Date = toFormattedDate(date),
					t.Exchange = pTiingoStock->GetExchange(),
					t.Symbol = pTiingoStock->GetSymbol(),
					t.LastClose = static_cast<double>(pTiingoStock->GetLastClose()) / ratio,
					t.Open = static_cast<double>(pTiingoStock->GetOpen()) / ratio,
					t.High = static_cast<double>(pTiingoStock->GetHigh()) / ratio,
					t.Low = static_cast<double>(pTiingoStock->GetLow()) / ratio,
					t.Close = static_cast<double>(pTiingoStock->GetNew()) / ratio,
					t.Volume = static_cast<double>(pTiingoStock->GetVolume()),
					t.Amount = static_cast<double>(pTiingoStock->GetAmount()),
					t.Dividend = static_cast<double>(pTiingoStock->GetDividend()),
					t.SplitFactor = static_cast<double>(pTiingoStock->GetSplitFactor()),
					t.UpAndDown = static_cast<double>(pTiingoStock->GetUpDown()) / ratio,
					t.UpDownRate = static_cast<double>(pTiingoStock->GetUpDownRate()),
					t.ChangeHandRate = static_cast<double>(pTiingoStock->GetChangeHandRate()),
					t.CurrentValue = static_cast<double>(pTiingoStock->GetCurrentValue()),
					t.TotalValue = static_cast<double>(pTiingoStock->GetTotalValue())
				);
				nValues++;
			}
		}
		if (nValues > 0) db(multi_insert);
		tx.commit();
	} catch (const std::exception& ex) {
		gl_systemMessage.PushErrorMessage(std::format("BuildDayLine(sqlpp11) failed: {}", ex.what()));
		return;
	}

	gl_systemMessage.PushDayLineInfoMessage("Tiingo IEX book of day saved");
	gl_systemMessage.PushInnerSystemInformationMessage("Tiingo IEX book of day saved");
	gl_systemConfiguration.SetTiingoIEXTopOfBookUpdateDate(date);
}

void CContainerTiingoStock::LoadDayLine(chrono::local_days date) {
	try {
		using namespace StockMarket;
		const auto& t = TiingoStockDayline{};

		auto st = gl_pWorldMarket->ToSysTime(toLocalDateTime(date, toLocalTime(170000)));

		auto db = gl_dbStockMarket.get();
		auto tx = start_transaction(db);

		// select rows for the given trade date
		auto result = db(select(all_of(t)).from(t).where(t.Date == toFormattedDate(date)).order_by(t.Symbol.asc()));

		for (const auto& row : result) {
			const std::string symbol = row.Symbol;
			if (IsSymbol(symbol)) {
				auto pStock = GetStock(symbol);
				if (pStock == nullptr) continue;

				pStock->SetTimePoint(st);

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
		gl_systemMessage.PushErrorMessage(std::format("LoadDayLine(sqlpp11) failed: {}", ex.what()));
	}
}

void CContainerTiingoStock::DeleteDayLine(chrono::local_days date) {
	using namespace StockMarket;
	const auto& t = TiingoStockDayline{};

	auto db = gl_dbStockMarket.get();
	auto tx = start_transaction(db);

	// Delete all rows for the given trade date in one statement
	db(remove_from(t).where(t.Date == toFormattedDate(date)));
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
	Delete52WeekHighDB();
	auto lSize = Size();

	using namespace StockMarket;
	const auto& t = TiingoStock52WeekHigh{};
	auto db = gl_dbStockMarket.get();
	auto tx = start_transaction(db);

	for (size_t i = 0; i < lSize; i++) {
		if (gl_systemConfiguration.IsExitingSystem()) break; // 如果程序正在退出，则停止存储。
		const CTiingoStockPtr pStock = GetStock(i);
		if (pStock->IsUpdate52WeekHighLowDB()) {
			auto Size = pStock->Get52WeekHighSize();
			for (size_t index = 0; index < Size; index++) {
				db(insert_into(t).set(
					t.Symbol = pStock->GetSymbol(),
					t.Exchange = pStock->GetExchange(),
					t.Date = toFormattedDate(pStock->Get52WeekHighDate(index))
				));
			}
		}
	}
	tx.commit();

	gl_systemConfiguration.SetTiingoStock52WeekHighLowUpdateDate(gl_pWorldMarket->GetCurrentTradeDate());
	gl_systemMessage.PushInnerSystemInformationMessage("tiingo 52 week high calculated");
}

void CContainerTiingoStock::TaskUpdate52WeekLowDB() {
	gl_systemMessage.PushInnerSystemInformationMessage("52 week low");
	Delete52WeekLowDB();
	auto lSize = Size();

	using namespace StockMarket;
	const auto& t = TiingoStock52WeekLow{};
	auto db = gl_dbStockMarket.get();
	auto tx = start_transaction(db);
	auto multi_insert = insert_into(t).columns(t.Symbol, t.Exchange, t.Date);

	int Values = 0;
	for (size_t i = 0; i < lSize; i++) {
		if (gl_systemConfiguration.IsExitingSystem()) break; // 如果程序正在退出，则停止存储。
		const CTiingoStockPtr pStock = GetStock(i);
		if (pStock->IsUpdate52WeekHighLowDB()) {
			auto Size = pStock->Get52WeekLowSize();
			for (size_t index = 0; index < Size; index++) {
				multi_insert.values.add(
					t.Symbol = pStock->GetSymbol(),
					t.Exchange = pStock->GetExchange(),
					t.Date = toFormattedDate(pStock->Get52WeekLowDate(index))
				);
				Values++;
			}
		}
	}
	if (Values > 0) db(multi_insert);
	tx.commit();

	gl_systemConfiguration.SetTiingoStock52WeekHighLowUpdateDate(gl_pWorldMarket->GetCurrentTradeDate());
	gl_systemMessage.PushInnerSystemInformationMessage("tiingo 52 week low calculated");
}

void CContainerTiingoStock::TaskCalculate() {
	gl_systemMessage.PushInnerSystemInformationMessage("calculating 52 week low");

	vector<result<int>> results;
	vector<int> vPos;
	auto lSize = Size();

	for (size_t index = 0; index < lSize; index++) {
		bool fFound = false;
		auto pStock = GetStock(index);
		pStock->Load52WeekLowDB();
		if (pStock->IsEnough52WeekLow()) {
			fFound = true;
		}
		pStock->m_v52WeekLowDate.clear(); //Note 直到这里才清空
		if (fFound) vPos.push_back(index); // vPos中存储找到的位置。
	}

	using namespace StockMarket;
	const auto& t = TiingoStockCurrentTrace{};
	auto db = gl_dbStockMarket.get();
	auto tx = start_transaction(db);
	auto multi_insert = insert_into(t).columns(t.Date, t.Symbol, t.SICCode);

	db(remove_from(t).where(t.Date == toFormattedDate(gl_pWorldMarket->GetMarketDate()))); // 先删除原有数据

	for (size_t index = 0; index < vPos.size(); index++) {
		auto pStock = GetStock(vPos.at(index));
		multi_insert.values.add(
			t.Date = toFormattedDate(gl_pWorldMarket->GetMarketDate()),
			t.Symbol = pStock->GetSymbol(),
			t.SICCode = pStock->GetSicCode()
		);
	}
	if (!vPos.empty()) db(multi_insert);
	tx.commit();

	gl_systemMessage.PushInnerSystemInformationMessage("52 week low Calculated");
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
	for (auto index = 0; index < lSize; index++) {
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

	using namespace StockMarket;
	const auto& t = TiingoStockCurrentTrace{};
	auto db = gl_dbStockMarket.get();
	auto tx = start_transaction(db);
	db(remove_from(t).where(t.Date == toFormattedDate(gl_pWorldMarket->GetMarketDate()))); // 先删除原有数据

	for (size_t index = 0; index < vPos.size(); index++) {
		auto pStock = GetStock(vPos.at(index));
		db(sqlpp::insert_into(t).set(
			t.Date = toFormattedDate(gl_pWorldMarket->GetMarketDate()),
			t.Symbol = pStock->GetSymbol(),
			t.SICCode = pStock->GetSicCode()
		));
	}
	tx.commit();

	gl_systemMessage.PushInnerSystemInformationMessage("52 week low Calculated");
}

void CContainerTiingoStock::Delete52WeekHighDB() {
	using namespace StockMarket;
	const auto& t = TiingoStock52WeekHigh{};
	auto db = gl_dbStockMarket.get();
	auto tx = start_transaction(db);

	db(sqlpp::remove_from(t).unconditionally());
	tx.commit();
}

void CContainerTiingoStock::Delete52WeekLowDB() {
	using namespace StockMarket;
	const auto& t = TiingoStock52WeekLow{};
	auto db = gl_dbStockMarket.get();
	auto tx = start_transaction(db);
	db(sqlpp::remove_from(t).unconditionally());
	tx.commit();
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
/// 计算52weekHigh、52weekLow
/// 需要控制住线程数量，以利于其他后台线程能够顺利执行。
///
///
/////////////////////////////////////////////////////////////////////////////////////////////
void CContainerTiingoStock::TaskProcessTodayDayLine() {
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
					pStock->ProcessDayLine();
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
	auto s = std::format("3月内再创新高数:{:d}, 3月内未再次新高数:{:d}, 比率:{:.2f}", gl_pWorldMarket->GetNewHighHigher(), gl_pWorldMarket->GetNoNewHighHigher(),
	                     static_cast<double>(gl_pWorldMarket->GetNewHighHigher()) / gl_pWorldMarket->GetNoNewHighHigher());
	gl_systemMessage.PushStockMarketInformationMessage(s);
}
