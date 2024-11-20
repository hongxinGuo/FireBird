#include "ContainerTiingoStock.h"
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
	ASSERT(gl_systemConfiguration.IsPaidTypeTiingoAccount()); // ���ô˺���ʱ�����뱣֤�Ǹ����˻���
	ASSERT(IsSymbol(pStock->GetSymbol()));

	auto pTiingoStock = GetStock(pStock->GetSymbol());
	pTiingoStock->UpdateProfile(pStock);
}

/// <summary>
/// 
/// ���ֲ�ѯ��ʽ�Ƚϻ�ɬ���������ȷ��Ŀǰʹ�ô˺�����(���ܳ��ִ洢�����ͬ��������⣬�о�֮��
///
/// ����˻��͸����˻����µķ�ʽ��ͬ��
/// ����˻�ֻ�洢��֤ȯ���ɣ������˻���Ҫ���������½��յ������ݡ�
///
/// </summary>
void CContainerTiingoStock::UpdateDB() {
	if (IsUpdateProfileDB()) {
		try {
			CSetTiingoStock setFinnhubStock;
			setFinnhubStock.m_strSort = _T("[Ticker]");
			setFinnhubStock.Open();
			setFinnhubStock.m_pDatabase->BeginTrans();
			while (!setFinnhubStock.IsEOF()) {	//����ԭ�еĴ��뼯״̬
				if (IsSymbol(setFinnhubStock.m_Ticker)) {
					const CTiingoStockPtr pStock = GetStock(setFinnhubStock.m_Ticker);
					ASSERT(pStock != nullptr);
					if (pStock->IsUpdateProfileDB()) {
						pStock->Update(setFinnhubStock);
						pStock->SetUpdateProfileDB(false);
					}
				}
				else {
					setFinnhubStock.Delete(); // ɾ���Ѳ����ڵĴ��롣
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
			setTiingoStock.Delete(); // ɾ���ظ�����
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

// �洢���յ�����
// 
void CContainerTiingoStock::BuildDayLine(long lDate) {
	CSetTiingoStockDayLine setDayLine;
	auto lSize = Size();
	time_t tMarketCloseTime = ConvertToTTime(lDate, 0, 160000);

	DeleteDayLine(lDate);

	setDayLine.m_strFilter = _T("[ID] = 1"); // ��������趨һ���޶���������ݱ�ܴ�ʱ����ʱ���ǳ���
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
		if (gl_systemConfiguration.IsExitingSystem()) break; // ������������˳�����ֹͣ�洢��
	}
}

void CContainerTiingoStock::TaskUpdate52WeekHighLowDB() {
	gl_systemMessage.PushInnerSystemInformationMessage("52 week high and low");
	Delete52WeekHighData();
	Delete52WeekLowData();
	auto lSize = Size();
	vector<result<void>> results;
	for (size_t i = 0; i < lSize; i++) {
		const CTiingoStockPtr pStock = GetStock(i);
		if (pStock->IsUpdate52WeekHighLowDB()) {
			gl_BackgroundWorkingThread.acquire();
			auto result = gl_runtime.thread_executor()->submit([pStock] {
				gl_ThreadStatus.IncreaseBackGroundWorkingThread();
				if (!gl_systemConfiguration.IsExitingSystem()) { // ������������˳�����ֹͣ�洢��
					pStock->Update52WeekHighLowDB();
				}
				gl_ThreadStatus.DecreaseBackGroundWorkingThread();
				gl_BackgroundWorkingThread.release();
			});
			results.emplace_back(std::move(result));
		}
	}
	for (auto& result : results) {
		result.get();
	}
	gl_systemConfiguration.SetTiingoStock52WeekHighLowUpdateDate(gl_pWorldMarket->GetCurrentTradeDate());
	gl_pWorldMarket->AddTask(WORLD_MARKET_TIINGO_CALCULATE__, GetNextTime(gl_pWorldMarket->GetMarketTime(), 0, 1, 0)); // һ���Ӻ����
	gl_systemMessage.PushInnerSystemInformationMessage(_T("tiingo 52 week calculated"));
}

void CContainerTiingoStock::TaskCalculate() {
	auto lSize = Size();
	CSetTiingoStockCurrentTrace setCurrentTrace;
	setCurrentTrace.m_strFilter = _T("[ID] = 1");
	setCurrentTrace.Open();
	setCurrentTrace.m_pDatabase->BeginTrans();
	for (size_t index = 0; index < lSize; index++) {
		auto pStock = GetStock(index);
		pStock->Load52WeekLow();
		if (pStock->IsEnough52WeekLow()) {
			setCurrentTrace.AddNew();
			setCurrentTrace.m_Date = gl_pWorldMarket->GetMarketDate();
			setCurrentTrace.m_Symbol = pStock->GetSymbol();
			setCurrentTrace.m_SICCode = pStock->m_iSicCode;
			setCurrentTrace.Update();
		}
		pStock->m_v52WeekLow.clear(); //Note ֱ����������
	}
	setCurrentTrace.m_pDatabase->CommitTrans();
	setCurrentTrace.Close();
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
/// ��Ҫ����ס�߳�������������������̨�߳��ܹ�˳��ִ�С�
///
///
/////////////////////////////////////////////////////////////////////////////////////////////
void CContainerTiingoStock::TaskProcessDayLine() {
	gl_systemMessage.PushInnerSystemInformationMessage(_T("��ʼ����Tiingo��������"));
	auto lSize = Size();
	vector<result<void>> vResults;
	for (size_t index = 0; index < lSize; index++) {
		auto pStock = GetStock(index);
		if (IsEarlyThen(pStock->GetDayLineStartDate(), pStock->GetDayLineEndDate(), 500)) { // ֻ�����������������ߵĹ�Ʊ
			gl_BackgroundWorkingThread.acquire();
			auto result = gl_runtime.thread_executor()->submit([pStock] {
				gl_ThreadStatus.IncreaseBackGroundWorkingThread();
				if (!gl_systemConfiguration.IsExitingSystem()) {
					//gl_systemMessage.PushDayLineInfoMessage(pStock->GetSymbol());
					pStock->ProcessDayLine();
				}
				gl_ThreadStatus.DecreaseBackGroundWorkingThread();
				gl_BackgroundWorkingThread.release();
			});
			vResults.emplace_back(std::move(result));
		}
	}
	for (auto& result2 : vResults) {
		result2.get(); // ������ȴ����е��߳�ִ�����
	}
	gl_systemConfiguration.SetTiingoStockDayLineProcessedDate(gl_pWorldMarket->GetMarketDate());
	gl_pWorldMarket->AddTask(WORLD_MARKET_TIINGO_UPDATE_52WEEK_HIGH_LOW__, GetNextTime(gl_pWorldMarket->GetMarketTime(), 0, 1, 0)); // һ���Ӻ����
	gl_systemMessage.PushInnerSystemInformationMessage(_T("Tiingo�������ݴ������"));
}

///////////////////////////////////////////////////////////////////////
//
// ���д���ĿǰRelease����ʱ��Ϊ13���ӡ�
//
//
///////////////////////////////////////////////////////////////////////
void CContainerTiingoStock::TaskProcessDayLine2() {
	gl_systemMessage.PushInnerSystemInformationMessage(_T("��ʼ����Tiingo��������"));
	auto lSize = Size();
	vector<result<int>> vResults;
	for (size_t index = 0; index < lSize; index++) {
		auto pStock = GetStock(index);
		if (IsEarlyThen(pStock->GetDayLineStartDate(), pStock->GetDayLineEndDate(), 500)) { // ֻ�����������������ߵĹ�Ʊ
			gl_systemMessage.PushDayLineInfoMessage(pStock->GetSymbol());
			pStock->ProcessDayLine();
		}
		if (gl_systemConfiguration.IsExitingSystem()) break;
	}
	gl_pWorldMarket->AddTask(WORLD_MARKET_TIINGO_CALCULATE__, GetNextTime(gl_pWorldMarket->GetMarketTime(), 0, 1, 0)); // һ���Ӻ����
	gl_systemMessage.PushInnerSystemInformationMessage(_T("Tiingo�������ݴ������"));
}
