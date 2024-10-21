#include "pch.h"
#include "ContainerTiingoStock.h"
#include "InfoReport.h"
#include "SetTiingoStockDayLine.h"
#include "TimeConvert.h"
#include "WorldMarket.h"

CContainerTiingoStock::CContainerTiingoStock() {
	CContainerTiingoStock::Reset();
}

CContainerTiingoStock::~CContainerTiingoStock() {
	//for (const auto& pStock : m_vStock) {
	//if (pStock->GetDayLineEndDate() < 19700101) pStock->SetDayLineEndDate(19800101);
	//	if (pStock->GetDayLineStartDate() == 29900101) {}
	//pStock->SetUpdateProfileDB(true);
	//}
	//UpdateDB();
}

void CContainerTiingoStock::Reset() {
	CContainerVirtualStock::Reset();
}

void CContainerTiingoStock::UpdateProfile(const CTiingoStockPtr& pStock) {
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
			CSetTiingoStock setWorldStock;
			setWorldStock.m_strSort = _T("[Ticker]");
			setWorldStock.Open();
			setWorldStock.m_pDatabase->BeginTrans();
			while (!setWorldStock.IsEOF()) {	//更新原有的代码集状态
				const CTiingoStockPtr pStock = GetStock(setWorldStock.m_Ticker);
				ASSERT(pStock != nullptr);
				if (pStock->IsUpdateProfileDB()) {
					pStock->Update(setWorldStock);
					pStock->SetUpdateProfileDB(false);
				}
				setWorldStock.MoveNext();
			}
			for (size_t l = 0; l < m_vStock.size(); l++) {
				const CTiingoStockPtr pStock = GetStock(l);
				ASSERT(pStock != nullptr);
				if (pStock->IsUpdateProfileDB()) {
					pStock->Append(setWorldStock);
					pStock->SetUpdateProfileDB(false);
					pStock->SetTodayNewStock(false);
				}
			}
			setWorldStock.m_pDatabase->CommitTrans();
			setWorldStock.Close();
		} catch (CException* e) {
			ReportInformationAndDeleteException(e);
		}
	}
}

bool CContainerTiingoStock::LoadDB() {
	CSetTiingoStock setTiingoStock;
	CString strSymbol = _T("");

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
			CString str = "发现重复代码：";
			str += setTiingoStock.m_Ticker;
			gl_systemMessage.PushInnerSystemInformationMessage(str);
			setTiingoStock.Delete();
		}
		setTiingoStock.MoveNext();
	}
	setTiingoStock.m_pDatabase->CommitTrans();
	setTiingoStock.Close();

	return true;
}

void CContainerTiingoStock::ResetDayLineStartEndDate() {
	for (int i = 0; i < Size(); i++) {
		auto pTiingoStock = GetStock(i);
		pTiingoStock->SetDayLineStartDate(29900101);
		pTiingoStock->SetDayLineEndDate(19800101);
	}
}

void CContainerTiingoStock::BuildDayLine(long lDate) {
	DeleteDayLine(lDate);

	// 存储该日的数据
	auto lSize = Size();
	time_t tMarketCloseTime = ConvertToTTime(lDate, 0, 170000);
	CSetTiingoStockDayLine setDayLine;
	setDayLine.Open();
	setDayLine.m_pDatabase->BeginTrans();
	for (size_t i = 0; i < lSize; i++) {
		auto pTiingoStock = GetStock(i);
		if (pTiingoStock->GetTransactionTime() >= tMarketCloseTime) {
			pTiingoStock->AddDayLine(setDayLine, lDate);
		}
	}
	setDayLine.m_pDatabase->CommitTrans();
	setDayLine.Close();
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
	for (int i = 0; i < lSize; i++) {
		if (GetStock(i)->m_fIsActive) iCount++;
	}
	return iCount;
}

bool CContainerTiingoStock::IsUpdateFinancialStateDB() noexcept {
	return std::ranges::any_of(m_vStock, [](const CVirtualStockPtr& pStock) { return dynamic_pointer_cast<CTiingoStock>(pStock)->IsUpdateFinancialStateDB(); });
}

void CContainerTiingoStock::UpdateFinancialStateDB() const {
	for (auto& pStock : m_vStock) {
		if (dynamic_pointer_cast<CTiingoStock>(pStock)->IsUpdateFinancialStateDB()) {
			dynamic_pointer_cast<CTiingoStock>(pStock)->UpdateFinancialStateDB();
			dynamic_pointer_cast<CTiingoStock>(pStock)->SetUpdateFinancialStateDB(false);
		}
	}
}

bool CContainerTiingoStock::IsUpdateDayLineDB() {
	return std::ranges::any_of(m_vStock, [](const CVirtualStockPtr& pStock) { return dynamic_pointer_cast<CTiingoStock>(pStock)->IsUpdateDayLineDB(); });
}

void CContainerTiingoStock::UpdateDayLineDB() {
	for (long i = 0; i < Size(); i++) {
		const CTiingoStockPtr pStock = GetStock(i);
		pStock->UpdateDayLineDB();
		if (gl_systemConfiguration.IsExitingSystem()) break; // 如果程序正在退出，则停止存储。
	}
}
