#include "pch.h"
#include "ContainerTiingoStock.h"
#include "InfoReport.h"
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

void CContainerTiingoStock::UpdateDayLineDB() {
	for (size_t i = 0; i < Size(); i++) {
		const CTiingoStockPtr pStock = GetStock(i);
		pStock->UpdateDayLineDB();
		if (gl_systemConfiguration.IsExitingSystem()) break; // 如果程序正在退出，则停止存储。
	}
}

void CContainerTiingoStock::Update52WeekHighLowDB() {
	auto lSize = Size();
	for (size_t i = 0; i < lSize; i++) {
		const CTiingoStockPtr pStock = GetStock(i);
		pStock->Update52WeekHighLowDB();
		if (gl_systemConfiguration.IsExitingSystem()) break; // 如果程序正在退出，则停止存储。
	}
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

void CContainerTiingoStock::TaskProcessDayLine() {
	gl_systemMessage.PushInnerSystemInformationMessage(_T("开始处理Tiingo日线数据"));
	auto lSize = Size();
	size_t index = 0;
	while (index < lSize) {
		while (gl_ThreadStatus.GetNumberOfBackGroundWorkingThread() > 7) Sleep(100);
		if (gl_systemConfiguration.IsExitingSystem()) return;
		auto pStock = GetStock(index);
		if (IsEarlyThen(pStock->GetDayLineStartDate(), pStock->GetDayLineEndDate(), 500)) { // 只处理有两年以上日线的股票
			gl_runtime.thread_executor()->post([pStock] {
				gl_BackgroundWorkingThread.acquire(); // 最多八个线程
				gl_ThreadStatus.IncreaseBackGroundWorkingThread();
				if (gl_systemConfiguration.IsExitingSystem()) return;
				gl_systemMessage.PushDayLineInfoMessage(_T("start ") + pStock->GetSymbol());
				pStock->ProcessDayLine();
				gl_ThreadStatus.DecreaseBackGroundWorkingThread();
				gl_BackgroundWorkingThread.release();
			});
		}
		index++;
	}

	while (gl_ThreadStatus.GetNumberOfBackGroundWorkingThread() > 0) Sleep(1000);
	gl_systemMessage.PushInnerSystemInformationMessage(_T("Tiingo日线数据处理完毕"));
}
