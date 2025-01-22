#include "pch.h"

#include "ContainerTiingoStock.h"
#include "InfoReport.h"
#include "SetTiingoStockCurrentTrace.h"
#include "SetTiingoStockDayLine.h"
#include "Thread.h"
#include "ThreadStatus.h"
#include "TimeConvert.h"
#include "WorldMarket.h"

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
void CContainerTiingoStock::UpdateDB() {
	if (IsUpdateProfileDB()) {
		try {
			CSetTiingoStock setFinnhubStock;
			setFinnhubStock.m_strSort = _T("[Ticker]");
			setFinnhubStock.Open();
			setFinnhubStock.m_pDatabase->BeginTrans();
			while (!setFinnhubStock.IsEOF()) {	//更新原有的代码集状态
				if (IsSymbol(setFinnhubStock.m_Ticker)) {
					const CTiingoStockPtr pStock = GetStock(setFinnhubStock.m_Ticker);
					ASSERT(pStock != nullptr);
					if (pStock->IsUpdateProfileDB()) {
						pStock->Update(setFinnhubStock);
						pStock->SetUpdateProfileDB(false);
					}
				}
				else {
					setFinnhubStock.Delete(); // 删除已不存在的代码。
				}
				setFinnhubStock.MoveNext();
			}
			for (size_t l = 0; l < m_vStock.size(); l++) {
				const CTiingoStockPtr pStock = GetStock(l);
				ASSERT(pStock != nullptr);
				if (pStock->IsUpdateProfileDB()) {
					pStock->Append(setFinnhubStock);
					pStock->SetUpdateProfileDB(false);
					pStock->SetTodayNewStock(false);
				}
			}
			setFinnhubStock.m_pDatabase->CommitTrans();
			setFinnhubStock.Close();
		} catch (CException* e) {
			ReportInformationAndDeleteException(e);
		}
	}
}

bool CContainerTiingoStock::LoadDB() {
	CSetTiingoStock setTiingoStock;
	CString strSymbol = _T("");

	setTiingoStock.m_strSort = _T("[Ticker]");
	setTiingoStock.Open();
	setTiingoStock.m_pDatabase->BeginTrans();
	while (!setTiingoStock.IsEOF()) {
		if (!IsSymbol(setTiingoStock.m_Ticker)) {
			const auto pTiingoStock = make_shared<CTiingoStock>();
			pTiingoStock->Load(setTiingoStock);
			pTiingoStock->CheckUpdateStatus(gl_pWorldMarket->GetMarketDate());
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

void CContainerTiingoStock::ResetDayLineStartEndDate() {
	for (size_t i = 0; i < Size(); i++) {
		auto pTiingoStock = GetStock(i);
		pTiingoStock->SetDayLineStartDate(29900101);
		pTiingoStock->SetDayLineEndDate(19800101);
	}
}

// 存储该日的数据
// 
void CContainerTiingoStock::BuildDayLine(long lDate) {
	CSetTiingoStockDayLine setDayLine;
	auto lSize = Size();
	time_t tMarketCloseTime = ConvertToTTime(lDate, 0, 160000);

	DeleteDayLine(lDate);

	setDayLine.m_strFilter = _T("[ID] = 1"); // 这里必须设定一个限定项，否则当数据表很大时，打开时间会非常长
	setDayLine.Open();
	setDayLine.m_pDatabase->BeginTrans();
	for (size_t i = 0; i < lSize; i++) {
		auto pTiingoStock = GetStock(i);
		if (pTiingoStock->GetTransactionTime() >= tMarketCloseTime) {
			pTiingoStock->SaveCurrentDataToDayLineDB(setDayLine, lDate);
		}
	}
	setDayLine.m_pDatabase->CommitTrans();
	setDayLine.Close();

	gl_systemConfiguration.SetTiingoIEXTopOfBookUpdateDate(lDate);
}

void CContainerTiingoStock::DeleteDayLine(long lDate) {
	char buffer[20]{ 0x000 };
	CSetTiingoStockDayLine setDayLine;

	_ltoa_s(lDate, buffer, 10);
	const CString strDate = buffer;
	setDayLine.m_strFilter = _T("[Date] =");
	setDayLine.m_strFilter += strDate;
	setDayLine.Open();
	setDayLine.m_pDatabase->BeginTrans();
	while (!setDayLine.IsEOF()) {
		setDayLine.Delete();
		setDayLine.MoveNext();
	}
	setDayLine.m_pDatabase->CommitTrans();
	setDayLine.Close();
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
	set52WeekHigh.m_strFilter = _T("[ID] = 1");
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
	gl_systemMessage.PushInnerSystemInformationMessage(_T("tiingo 52 week high calculated"));
}

void CContainerTiingoStock::TaskUpdate52WeekLowDB() {
	gl_systemMessage.PushInnerSystemInformationMessage("52 week low");
	Delete52WeekLowData();
	auto lSize = Size();

	CSetTiingoStock52WeekLow set52WeekLow;
	set52WeekLow.m_strFilter = _T("[ID] = 1");
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
	gl_systemMessage.PushInnerSystemInformationMessage(_T("tiingo 52 week low calculated"));
}

void CContainerTiingoStock::TaskCalculate() {
	gl_systemMessage.PushInnerSystemInformationMessage("calculating 52 week low");
	auto lSize = Size();
	CSetTiingoStockCurrentTrace setCurrentTrace;
	setCurrentTrace.m_strFilter = _T("[ID] = 1");
	setCurrentTrace.Open();
	setCurrentTrace.m_pDatabase->BeginTrans();
	for (size_t index = 0; index < lSize; index++) {
		if (gl_systemConfiguration.IsExitingSystem()) break; // 如果程序正在退出，则停止存储。
		auto pStock = GetStock(index);
		pStock->Load52WeekLow();
		if (pStock->IsEnough52WeekLow()) {
			setCurrentTrace.AddNew();
			setCurrentTrace.m_Date = gl_pWorldMarket->GetMarketDate();
			setCurrentTrace.m_Symbol = pStock->GetSymbol();
			setCurrentTrace.m_SICCode = pStock->m_iSicCode;
			setCurrentTrace.Update();
		}
		pStock->m_v52WeekLow.clear(); //Note 直到这里才清空
	}
	setCurrentTrace.m_pDatabase->CommitTrans();
	setCurrentTrace.Close();
	gl_systemMessage.PushInnerSystemInformationMessage("52 week low Calculated");
}

void CContainerTiingoStock::Delete52WeekHighData() {
	CDatabase database;

	database.Open(_T("WorldMarket"), FALSE, FALSE, _T("ODBC;UID=FireBird;PASSWORD=firebird;charset=utf8mb4"));
	database.BeginTrans();
	database.ExecuteSQL(_T("TRUNCATE `worldmarket`.`tiingo_stock_52week_high`;"));
	database.CommitTrans();
	database.Close();
}

void CContainerTiingoStock::Delete52WeekLowData() {
	CDatabase database;

	database.Open(_T("WorldMarket"), FALSE, FALSE, _T("ODBC;UID=FireBird;PASSWORD=firebird;charset=utf8mb4"));
	database.BeginTrans();
	database.ExecuteSQL(_T("TRUNCATE `worldmarket`.`tiingo_stock_52week_low`;"));
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
	gl_systemMessage.PushInnerSystemInformationMessage(_T("开始处理Tiingo日线数据"));
	auto lSize = Size();
	vector<result<void>> vResults;
	for (size_t index = 0; index < lSize; index++) {
		auto pStock = GetStock(index);
		if (IsEarlyThen(pStock->GetDayLineStartDate(), pStock->GetDayLineEndDate(), 500)) { // 只处理有两年以上日线的股票
			gl_BackgroundWorkingThread.acquire();
			auto result = gl_runtime.thread_executor()->submit([pStock] {
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

	auto result2 = gl_runtime.thread_executor()->submit([] {
		gl_dataContainerTiingoStock.TaskUpdate52WeekHighDB();
	});
	auto result3 = gl_runtime.thread_executor()->submit([] {
		gl_dataContainerTiingoStock.TaskUpdate52WeekLowDB();
	});
	result2.get();
	result3.get();

	for (int i = 0; i < gl_dataContainerTiingoStock.Size(); i++) {
		auto pStock = gl_dataContainerTiingoStock.GetStock(i);
		pStock->SetUpdate52WeekHighLowDB(false);
	}

	auto result5 = gl_runtime.thread_executor()->submit([] {
		gl_dataContainerTiingoStock.TaskCalculate();
	});
	result5.get();
	gl_systemMessage.PushInnerSystemInformationMessage(_T("Tiingo日线数据处理完毕"));
}
