//////////////////////////////////////////////////////////////////////////////////////////
//
// CContainerFinnhubStock类操作worldMarket数据库中的finnhub_stock_profile表。
//
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "pch.h"

#include "InfoReport.h"
#include"WorldMarket.h"

#include"SetFinnhubStock.h"

#include "ContainerFinnhubStock.h"

CContainerFinnhubStock::CContainerFinnhubStock() {
	CContainerFinnhubStock::Reset();
}

CContainerFinnhubStock::~CContainerFinnhubStock() {
	//for (const auto& pStock : m_vStock) {
	//pStock->SetUpdateProfileDB(true);
	//}
	//UpdateProfileDB();
}

void CContainerFinnhubStock::Reset() {
	CContainerVirtualStock::Reset();
}

void CContainerFinnhubStock::ResetEPSSurprise() {
	for (size_t l = 0; l < m_vStock.size(); l++) {
		const CFinnhubStockPtr pStock = GetStock(l);
		pStock->SetLastEPSSurpriseUpdateDate(19800101);
		pStock->m_fUpdateEPSSurprise = true;
	}
}

void CContainerFinnhubStock::ResetPeer() {
	for (size_t l = 0; l < m_vStock.size(); l++) {
		const CFinnhubStockPtr pStock = GetStock(l);
		if (pStock->GetPeerUpdateDate() != 19800101) {
			pStock->SetPeerUpdateDate(19800101);
			pStock->SetUpdatePeer(true);
			pStock->SetUpdateProfileDB(true);
		}
	}
}

void CContainerFinnhubStock::ResetBasicFinancial() {
	for (size_t l = 0; l < m_vStock.size(); l++) {
		const CFinnhubStockPtr pStock = GetStock(l);
		if (pStock->GetBasicFinancialUpdateDate() != 19800101) {
			pStock->SetBasicFinancialUpdateDate(19800101);
			pStock->SetUpdateBasicFinancial(true);
			pStock->SetUpdateProfileDB(true);
		}
	}
}

void CContainerFinnhubStock::ResetDayLine() {
	for (size_t l = 0; l < m_vStock.size(); l++) {
		const CFinnhubStockPtr pStock = GetStock(l);
		pStock->SetIPOStatus(_STOCK_NOT_CHECKED_);
		pStock->SetDayLineStartDate(29900101);
		pStock->SetDayLineEndDate(19800101);
		pStock->SetUpdateDayLine(true);
		pStock->SetUpdateProfileDB(true);
	}
}

bool CContainerFinnhubStock::LoadDB() {
	CSetFinnhubStock setFinnhubStock;
	CFinnhubStockPtr pFinnhubStock = nullptr;
	long lMaxSymbolLength = 0;

	setFinnhubStock.m_strSort = _T("[Symbol]");
	setFinnhubStock.Open();
	setFinnhubStock.m_pDatabase->BeginTrans();
	while (!setFinnhubStock.IsEOF()) {
		pFinnhubStock = make_shared<CFinnhubStock>();
		pFinnhubStock->Load(setFinnhubStock);
		if (!IsSymbol(pFinnhubStock->GetSymbol())) {
			pFinnhubStock->CheckUpdateStatus(gl_pWorldMarket->GetMarketDate());
			Add(pFinnhubStock);
			if (pFinnhubStock->GetSymbol().GetLength() > lMaxSymbolLength) {
				lMaxSymbolLength = pFinnhubStock->GetSymbol().GetLength();
			}
		}
		else {
			setFinnhubStock.Delete(); // 删除此重复代码
		}
		setFinnhubStock.MoveNext();

		//ValidateStockSymbol(pFinnhubStock);
	}
	setFinnhubStock.m_pDatabase->CommitTrans();
	setFinnhubStock.Close();
	Sort();

	ASSERT(lMaxSymbolLength < 20); // 目前WorldMarket数据库的股票代码长度限制为20个字符
	string s = fmt::format("WorldMarket股票代码最长长度为{:Ld}", lMaxSymbolLength);
#ifdef _DEBUG
	gl_systemMessage.PushInnerSystemInformationMessage(s.c_str());
#endif

	return true;
}

/// <summary>
/// 这种查询方式比较晦涩，但结果正确。目前使用此函数。(可能出现存储多个相同代码的问题，研究之）
/// </summary>
void CContainerFinnhubStock::UpdateProfileDB() {
	if (IsUpdateProfileDB()) {
		try {
			CSetFinnhubStock setFinnhubStock;
			int iCurrentUpdated = 0;
			int iStockNeedUpdate = 0;
			for (const auto& pStock : m_vStock) {
				if (pStock->IsUpdateProfileDB()) iStockNeedUpdate++;
			}
			setFinnhubStock.m_strSort = _T("[Symbol]");
			setFinnhubStock.Open();
			setFinnhubStock.m_pDatabase->BeginTrans();
			while (iCurrentUpdated < iStockNeedUpdate) {	//更新原有的代码集状态
				if (setFinnhubStock.IsEOF()) break;
				const CFinnhubStockPtr pStock = GetStock(setFinnhubStock.m_Symbol);
				ASSERT(pStock != nullptr);
				if (pStock->IsUpdateProfileDB()) {
					iCurrentUpdated++;
					pStock->Update(setFinnhubStock);
					pStock->SetUpdateProfileDB(false);
				}
				setFinnhubStock.MoveNext();
			}
			if (iCurrentUpdated < iStockNeedUpdate) { // 添加新的股票简介
				for (size_t l = 0; l < m_vStock.size(); l++) {
					const CFinnhubStockPtr pStock = GetStock(l);
					ASSERT(pStock != nullptr);
					if (pStock->IsUpdateProfileDB()) {
						iCurrentUpdated++;
						pStock->Append(setFinnhubStock);
						pStock->SetUpdateProfileDB(false);
						pStock->SetTodayNewStock(false);
					}
					if (iCurrentUpdated >= iStockNeedUpdate) break;
				}
			}
			setFinnhubStock.m_pDatabase->CommitTrans();
			setFinnhubStock.Close();
		} catch (CException* e) {
			ReportInformationAndDeleteException(e);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
//
// 更新基本金融信息
//
// Note 更新300条信息大致需要45秒。
//
//
////////////////////////////////////////////////////////////////////////////////////////////
bool CContainerFinnhubStock::UpdateBasicFinancialDB() {
	static bool s_fInProcess = false;
	vector<CFinnhubStockPtr> vStock{};

	if (s_fInProcess) {
		gl_systemMessage.PushErrorMessage(_T("UpdateBasicFinancialDB任务用时超过五分钟"));
		return false;
	}
	s_fInProcess = true;

	vStock.clear();
	for (size_t l = 0; l < m_vStock.size(); l++) {
		const CFinnhubStockPtr pStock = GetStock(l);
		if (pStock->IsUpdateBasicFinancialDB()) {
			vStock.push_back(pStock);
		}
	}

	UpdateBasicFinancialAnnualDB(vStock);
	UpdateBasicFinancialQuarterDB(vStock);
	UpdateBasicFinancialMetricDB(vStock);

	ClearUpdateBasicFinancialFlag(vStock);

	s_fInProcess = false;
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
// 这个函数比较占用CPU的计算能力，故而当使用较慢的CPU时，不使用工作线程做并行处理
//
// 发现使用并行处理方式时，数据库偶尔会出现故障，估计是MySQL数据库的同步问题，目前不容易找到问题之所在。
// 决定还是使用串行方式，不再生成线程。--20221101
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////
void CContainerFinnhubStock::UpdateBasicFinancialQuarterDB(const vector<CFinnhubStockPtr>& vStock) {
	for (const auto& pStock : vStock) {
		if (gl_systemConfiguration.IsExitingSystem()) break;
		pStock->AppendBasicFinancialQuarter();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
// 这个函数比较占用CPU的计算能力，故而当使用较慢的CPU时，不使用工作线程做并行处理
//
// 发现使用并行处理方式时，数据库偶尔会出现故障，估计是MySQL数据库的同步问题，目前不容易找到问题之所在。
// 决定还是使用串行方式，不再生成线程。--20221101
//
//////////////////////////////////////////////////////////////////////////////////////////////////
void CContainerFinnhubStock::UpdateBasicFinancialAnnualDB(const vector<CFinnhubStockPtr>& vStock) {
	for (const auto& pStock : vStock) {
		if (gl_systemConfiguration.IsExitingSystem()) break;
		pStock->AppendBasicFinancialAnnual();
	}
}

void CContainerFinnhubStock::UpdateBasicFinancialMetricDB(const vector<CFinnhubStockPtr>& vStock) {
	try {
		CSetFinnhubStockBasicFinancialMetric setBasicFinancialMetric;
		const auto iBasicFinancialNeedUpdate = vStock.size();
		size_t iCurrentUpdated = 0;

		ASSERT(IsUpdateBasicFinancialDB());
		setBasicFinancialMetric.m_strSort = _T("[Symbol]");
		setBasicFinancialMetric.Open();
		setBasicFinancialMetric.m_pDatabase->BeginTrans();
		//更新原有的基本财务信息
		while (iCurrentUpdated < iBasicFinancialNeedUpdate) {
			if (setBasicFinancialMetric.IsEOF()) break;
			if (IsSymbol(setBasicFinancialMetric.m_symbol)) {
				CFinnhubStockPtr pStockNeedUpdate = GetStock(setBasicFinancialMetric.m_symbol);
				if (vStock.end() != std::ranges::find(vStock.begin(), vStock.end(), pStockNeedUpdate)) {
					iCurrentUpdated++;
					pStockNeedUpdate->UpdateBasicFinancialMetric(setBasicFinancialMetric);
					pStockNeedUpdate->SetUpdateBasicFinancialDB(false);
				}
			}
			else {
				setBasicFinancialMetric.Delete(); //Note 自动删除代码已不存在的数据。
			}
			setBasicFinancialMetric.MoveNext();
		}
		// 添加新的基本财务数据
		if (iCurrentUpdated < iBasicFinancialNeedUpdate) {
			ASSERT(setBasicFinancialMetric.IsEOF());
			for (int i = 0; i < iBasicFinancialNeedUpdate; i++) {
				const auto& pStockNeedAppend = vStock.at(i);
				if (pStockNeedAppend->IsUpdateBasicFinancialDB()) {
					iCurrentUpdated++;
					pStockNeedAppend->AppendBasicFinancialMetric(setBasicFinancialMetric);
					pStockNeedAppend->SetUpdateBasicFinancialDB(false);
					ASSERT(iCurrentUpdated <= iBasicFinancialNeedUpdate);
				}
			}
		}
		setBasicFinancialMetric.m_pDatabase->CommitTrans();
		setBasicFinancialMetric.Close();
		ASSERT(iCurrentUpdated == iBasicFinancialNeedUpdate);
	} catch (CException* e) {
		ReportInformationAndDeleteException(e);
	}
}

void CContainerFinnhubStock::ClearUpdateBasicFinancialFlag(const vector<CFinnhubStockPtr>& vStock) {
	for (const auto& pStock : vStock) {
		if (pStock->IsUpdateBasicFinancialDB()) {
			pStock->SetUpdateBasicFinancialDB(false);
		}
	}
}

bool CContainerFinnhubStock::ValidateStockSymbol(const CFinnhubStockPtr& pStock) {
	const CString strSymbol = pStock->GetSymbol();
	const CString strExchangeCode = pStock->GetExchangeCode();

	if (strExchangeCode.Compare(_T("US")) == 0) return true;
	const int pos = strSymbol.Find(_T(".") + strExchangeCode);
	if ((pos + 1) < (strSymbol.GetLength() - strExchangeCode.GetLength())) {
		return false;
	}
	return true;
}

bool CContainerFinnhubStock::IsUpdateCompanyNewsDB() {
	return std::ranges::any_of(m_vStock, [](const CVirtualStockPtr& pStock) { return dynamic_pointer_cast<CFinnhubStock>(pStock)->IsUpdateCompanyNewsDB(); });
}

bool CContainerFinnhubStock::IsUpdateBasicFinancialDB() {
	return std::ranges::any_of(m_vStock, [](const CVirtualStockPtr& pStock) { return dynamic_pointer_cast<CFinnhubStock>(pStock)->IsUpdateBasicFinancialDB(); });
}

bool CContainerFinnhubStock::IsUpdateInsiderTransactionDB() {
	return std::ranges::any_of(m_vStock, [](const CVirtualStockPtr& P) { return dynamic_pointer_cast<CFinnhubStock>(P)->IsUpdateInsiderTransactionDB(); });
}

bool CContainerFinnhubStock::IsUpdateInsiderSentimentDB() {
	return std::ranges::any_of(m_vStock, [](const CVirtualStockPtr& p) { return dynamic_pointer_cast<CFinnhubStock>(p)->IsUpdateInsiderSentimentDB(); });
}

bool CContainerFinnhubStock::IsUpdateEPSSurpriseDB() {
	return std::ranges::any_of(m_vStock, [](const CVirtualStockPtr& p) { return dynamic_pointer_cast<CFinnhubStock>(p)->IsUpdateEPSSurpriseDB(); });
}

bool CContainerFinnhubStock::IsUpdateSECFilingsDB() {
	return std::ranges::any_of(m_vStock, [](const CVirtualStockPtr& p) { return dynamic_pointer_cast<CFinnhubStock>(p)->IsUpdateSECFilingsDB(); });
}
