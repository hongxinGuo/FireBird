//////////////////////////////////////////////////////////////////////////////////////////
//
// CContainerWorldStock类操作worldMarket数据库中的stock_profile表。
//
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "pch.h"

#include"thread.h"
#include"WorldMarket.h"

#include"SetWorldStock.h"

#include "ContainerWorldStock.h"

using namespace std;

CContainerWorldStock::CContainerWorldStock() {
	CContainerWorldStock::Reset();
}

void CContainerWorldStock::Reset(void) {
	CContainerVirtualStock::Reset();
}

void CContainerWorldStock::ResetEPSSurprise(void) {
	for (size_t l = 0; l < m_vStock.size(); l++) {
		const CWorldStockPtr pStock = GetStock(l);
		pStock->SetLastEPSSurpriseUpdateDate(19800101);
		pStock->m_fEPSSurpriseUpdated = false;
	}
}

void CContainerWorldStock::ResetPeer(void) {
	for (size_t l = 0; l < m_vStock.size(); l++) {
		const CWorldStockPtr pStock = GetStock(l);
		if (pStock->GetPeerUpdateDate() != 19800101) {
			pStock->SetPeerUpdateDate(19800101);
			pStock->SetUpdatePeer(true);
			pStock->SetUpdateProfileDB(true);
		}
	}
}

void CContainerWorldStock::ResetBasicFinancial(void) {
	for (size_t l = 0; l < m_vStock.size(); l++) {
		const CWorldStockPtr pStock = GetStock(l);
		if (pStock->GetBasicFinancialUpdateDate() != 19800101) {
			pStock->SetBasicFinancialUpdateDate(19800101);
			pStock->SetUpdateBasicFinancial(true);
			pStock->SetUpdateProfileDB(true);
		}
	}
}

void CContainerWorldStock::ResetDayLine(void) {
	for (size_t l = 0; l < m_vStock.size(); l++) {
		const CWorldStockPtr pStock = GetStock(l);
		pStock->SetIPOStatus(_STOCK_NOT_CHECKED_);
		pStock->SetDayLineStartDate(29900101);
		pStock->SetDayLineEndDate(19800101);
		pStock->SetDayLineNeedUpdate(true);
		pStock->SetUpdateProfileDB(true);
	}
}

bool CContainerWorldStock::LoadDB(void) {
	CSetWorldStock setWorldStock;
	CWorldStockPtr pWorldStock = nullptr;
	CString str;
	long lMaxSymbolLength = 0;

	setWorldStock.m_strSort = _T("[Symbol]");
	setWorldStock.Open();
	setWorldStock.m_pDatabase->BeginTrans();
	while (!setWorldStock.IsEOF()) {
		pWorldStock = make_shared<CWorldStock>();
		pWorldStock->Load(setWorldStock);
		if (!IsSymbol(pWorldStock->GetSymbol())) {
			pWorldStock->CheckUpdateStatus(gl_pWorldMarket->GetMarketDate());
			Add(pWorldStock);
			if (pWorldStock->GetSymbol().GetLength() > lMaxSymbolLength) {
				lMaxSymbolLength = pWorldStock->GetSymbol().GetLength();
			}
		}
		else {
			str = _T("发现重复代码：");
			str += pWorldStock->GetSymbol();
			gl_systemMessage.PushInnerSystemInformationMessage(str);
			setWorldStock.Delete(); // 删除此重复代码
		}
		setWorldStock.MoveNext();

		//ValidateStockSymbol(pWorldStock);
	}
	setWorldStock.m_pDatabase->CommitTrans();
	setWorldStock.Close();
	Sort();

	ASSERT(lMaxSymbolLength < 20); // 目前WorldMarket数据库的股票代码长度限制为20个字符
	char buffer[100];
	sprintf_s(buffer, _T("%d"), lMaxSymbolLength);
	str = _T("WorldMarket股票代码最长长度为");
	str += buffer;
#ifdef _DEBUG
	gl_systemMessage.PushInnerSystemInformationMessage(str);
#endif

	return true;
}

/// <summary>
/// 这种查询方式比较晦涩，但结果正确。目前使用此函数。(可能出现存储多个相同代码的问题，研究之）
/// </summary>
void CContainerWorldStock::UpdateProfileDB(void) {
	//更新原有的代码集状态
	if (IsUpdateProfileDB()) {
		CSetWorldStock setWorldStock;
		int iCurrentUpdated = 0;
		int iStockNeedUpdate = 0;
		for (const auto& pStock : m_vStock) {
			if (pStock->IsUpdateProfileDB()) iStockNeedUpdate++;
		}
		if (iStockNeedUpdate > 500) iStockNeedUpdate = 500; // 每次更新500个,保证此任务不占用过多时间（500个大致需要2-5秒钟）。每秒申请一次的话，五分钟最多300个。
		setWorldStock.m_strSort = _T("[Symbol]");
		setWorldStock.Open();
		setWorldStock.m_pDatabase->BeginTrans();
		while (iCurrentUpdated < iStockNeedUpdate) {
			if (setWorldStock.IsEOF()) break;
			const CWorldStockPtr pStock = GetStock(setWorldStock.m_Symbol);
			ASSERT(pStock != nullptr);
			if (pStock->IsUpdateProfileDB()) {
				iCurrentUpdated++;
				pStock->Update(setWorldStock);
				pStock->SetUpdateProfileDB(false);
			}
			setWorldStock.MoveNext();
		}
		if (iCurrentUpdated < iStockNeedUpdate) {
			if (!setWorldStock.IsEOF()) {
				setWorldStock.MoveLast();
				setWorldStock.MoveNext();
			}
			for (size_t l = 0; l < m_vStock.size(); l++) {
				const CWorldStockPtr pStock = GetStock(l);
				ASSERT(pStock != nullptr);
				if (pStock->IsUpdateProfileDB()) {
					iCurrentUpdated++;
					pStock->Append(setWorldStock);
					pStock->SetUpdateProfileDB(false);
					pStock->SetTodayNewStock(false);
				}
				if (iCurrentUpdated >= iStockNeedUpdate) break;
			}
		}
		setWorldStock.m_pDatabase->CommitTrans();
		setWorldStock.Close();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
//
// 更新基本金融信息
//
//
////////////////////////////////////////////////////////////////////////////////////////////
bool CContainerWorldStock::UpdateBasicFinancialDB(void) {
	static bool s_fInProcess = false;
	vector<CWorldStockPtr> s_vStock{};

	if (s_fInProcess) {
		gl_systemMessage.PushErrorMessage(_T("UpdateBasicFinancialDB任务用时超过五分钟"));
		return false;
	}
	s_fInProcess = true;

	s_vStock.clear();
	for (size_t l = 0; l < m_vStock.size(); l++) {
		const CWorldStockPtr pStock = GetStock(l);
		if (pStock->IsUpdateBasicFinancialDB()) {
			s_vStock.push_back(pStock);
		}
	}

	UpdateBasicFinancialAnnualDB(s_vStock);
	UpdateBasicFinancialQuarterDB(s_vStock);
	UpdateBasicFinancialMetricDB(s_vStock);

	ClearUpdateBasicFinancialFlag(s_vStock);

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
bool CContainerWorldStock::UpdateBasicFinancialQuarterDB(vector<CWorldStockPtr> vStock) {
	for (const auto& pStock : vStock) {
		if (gl_systemStatus.IsExitingSystem()) break;
		pStock->AppendBasicFinancialQuarter();
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
// 这个函数比较占用CPU的计算能力，故而当使用较慢的CPU时，不使用工作线程做并行处理
//
// 发现使用并行处理方式时，数据库偶尔会出现故障，估计是MySQL数据库的同步问题，目前不容易找到问题之所在。
// 决定还是使用串行方式，不再生成线程。--20221101
//
//////////////////////////////////////////////////////////////////////////////////////////////////
bool CContainerWorldStock::UpdateBasicFinancialAnnualDB(vector<CWorldStockPtr> vStock) {
	for (const auto& pStock : vStock) {
		if (gl_systemStatus.IsExitingSystem()) break;
		pStock->AppendBasicFinancialAnnual();
	}
	return true;
}

bool CContainerWorldStock::UpdateBasicFinancialMetricDB(vector<CWorldStockPtr> vStock) {
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
		CWorldStockPtr pStockNeedUpdate = GetStock(setBasicFinancialMetric.m_symbol);
		if (vStock.end() != ranges::find(vStock.begin(), vStock.end(), pStockNeedUpdate)) {
			iCurrentUpdated++;
			pStockNeedUpdate->UpdateBasicFinancialMetric(setBasicFinancialMetric);
			pStockNeedUpdate->SetUpdateBasicFinancialDB(false);
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

	return true;
}

void CContainerWorldStock::ClearUpdateBasicFinancialFlag(vector<CWorldStockPtr> vStock) {
	for (const auto& pStock : vStock) {
		if (pStock->IsUpdateBasicFinancialDB()) {
			CString strMessage = _T("found stock:") + pStock->GetSymbol() + _T(" need update basic financial data");
			TRACE(strMessage);
			gl_systemMessage.PushErrorMessage(strMessage);
		}
		pStock->SetUpdateBasicFinancialDB(false);
	}
}

bool CContainerWorldStock::ValidateStockSymbol(CWorldStockPtr pStock) {
	const CString strSymbol = pStock->GetSymbol();
	const CString strExchangeCode = pStock->GetExchangeCode();

	if (strExchangeCode.Compare(_T("US")) == 0) return true;
	const int pos = strSymbol.Find(_T(".") + strExchangeCode);
	if ((pos + 1) < (strSymbol.GetLength() - strExchangeCode.GetLength())) {
		return false;
	}
	return true;
}

bool CContainerWorldStock::IsUpdateCompanyNewsDB(void) {
	return ranges::any_of(m_vStock, [](const CVirtualStockPtr& pStock) { return dynamic_pointer_cast<CWorldStock>(pStock)->IsUpdateCompanyNewsDB(); });
}

bool CContainerWorldStock::IsUpdateBasicFinancialDB(void) {
	return ranges::any_of(m_vStock, [](const CVirtualStockPtr& pStock) { return dynamic_pointer_cast<CWorldStock>(pStock)->IsUpdateBasicFinancialDB(); });
}

bool CContainerWorldStock::IsSaveInsiderTransaction(void) {
	return ranges::any_of(m_vStock, [](CVirtualStockPtr& P) { return dynamic_pointer_cast<CWorldStock>(P)->IsSaveInsiderTransaction(); });
}

bool CContainerWorldStock::IsSaveInsiderSentiment(void) {
	return ranges::any_of(m_vStock, [](CVirtualStockPtr& p) { return dynamic_pointer_cast<CWorldStock>(p)->IsSaveInsiderSentiment(); });
}
