//////////////////////////////////////////////////////////////////////////////////////////
//
// CDataWorldStock类操作worldmarket数据库中的stock_profile表。
//
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "pch.h"

#include"globedef.h"

#include"thread.h"
#include"WorldMarket.h"

#include"SetWorldStock.h"
#include"CallableFunction.h"

#include "DataWorldStock.h"

CDataWorldStock::CDataWorldStock() {
	Reset();
}

CDataWorldStock::~CDataWorldStock() {
}

void CDataWorldStock::Reset(void) {
	m_vWorldStock.resize(0);
	m_mapWorldStock.clear();
	m_lLastTotalWorldStock = 0;
}

bool CDataWorldStock::SortStock(void)
{
	sort(m_vWorldStock.begin(), m_vWorldStock.end(), CompareWorldStock);
	m_mapWorldStock.clear();
	int j = 0;
	for (auto& pStock : m_vWorldStock) {
		m_mapWorldStock[pStock->GetSymbol()] = j++;
	}

	return true;
}

bool CDataWorldStock::IsStockProfileNeedUpdate(void) {
	const int iTotal = m_vWorldStock.size();
	for (int i = 0; i < iTotal; i++) {
		if (m_vWorldStock.at(i)->IsUpdateProfileDB()) return true;
	}
	return false;
}

bool CDataWorldStock::IsBasicFinancialNeedUpdate(void) {
	const int iTotal = m_vWorldStock.size();
	for (int i = 0; i < iTotal; i++) {
		if (m_vWorldStock.at(i)->IsUpdateBasicFinancialDB()) return true;
	}
	return false;
}

void CDataWorldStock::ResetEPSSurprise(void) {
	for (auto& p : m_vWorldStock) {
		p->SetLastEPSSurpriseUpdateDate(19800101);
		p->m_fEPSSurpriseUpdated = false;
	}
}

void CDataWorldStock::ResetPeer(void) {
	for (auto& pStock : m_vWorldStock) {
		if (pStock->GetPeerUpdateDate() != 19800101) {
			pStock->SetPeerUpdateDate(19800101);
			pStock->SetPeerUpdated(false);
			pStock->SetUpdateProfileDB(true);
		}
	}
}

void CDataWorldStock::ResetBasicFinancial(void) {
	for (auto& pStock : m_vWorldStock) {
		if (pStock->GetBasicFinancialUpdateDate() != 19800101) {
			pStock->SetBasicFinancialUpdateDate(19800101);
			pStock->SetBasicFinancialUpdated(false);
			pStock->SetUpdateProfileDB(true);
		}
	}
}

void CDataWorldStock::ResetDayLine(void) {
	for (auto& pStock : m_vWorldStock) {
		pStock->SetIPOStatus(__STOCK_NOT_CHECKED__);
		pStock->SetDayLineStartDate(29900101);
		pStock->SetDayLineEndDate(19800101);
		pStock->SetDayLineNeedUpdate(true);
		pStock->SetUpdateProfileDB(true);
	}
}

bool CDataWorldStock::Delete(CWorldStockPtr pStock) {
	if (pStock == nullptr) return false;
	if (!IsStock(pStock->GetSymbol())) return false;

	m_vWorldStock.erase(m_vWorldStock.begin() + m_mapWorldStock.at(pStock->GetSymbol()));
	m_mapWorldStock.erase(pStock->GetSymbol());

	return true;
}

void CDataWorldStock::Add(CWorldStockPtr pStock) {
	m_mapWorldStock[pStock->GetSymbol()] = m_vWorldStock.size();
	m_vWorldStock.push_back(pStock);
	pStock->SetTodayNewStock(true);
	pStock->SetUpdateProfileDB(true);
}

bool CDataWorldStock::LoadDB(void) {
	CSetWorldStock setWorldStock;
	CWorldStockPtr pWorldStock = nullptr;
	CString str;
	long lSymbolLength = 0;

	setWorldStock.m_strSort = _T("[Symbol]");
	setWorldStock.Open();
	setWorldStock.m_pDatabase->BeginTrans();
	while (!setWorldStock.IsEOF()) {
		pWorldStock = make_shared<CWorldStock>();
		pWorldStock->Load(setWorldStock);
		if (!IsStock(pWorldStock->GetSymbol())) {
			pWorldStock->CheckProfileUpdateStatus(gl_pWorldMarket->GetMarketDate());
			pWorldStock->CheckBasicFinancialUpdateStatus(gl_pWorldMarket->GetMarketDate());
			pWorldStock->CheckDayLineUpdateStatus(gl_pWorldMarket->GetMarketDate(), gl_pWorldMarket->GetLastTradeDate(), gl_pWorldMarket->GetMarketTime(), gl_pWorldMarket->GetDayOfWeek());
			pWorldStock->CheckEPSSurpriseStatus(gl_pWorldMarket->GetMarketDate());
			pWorldStock->CheckPeerStatus(gl_pWorldMarket->GetMarketDate());
			pWorldStock->CheckInsiderTransactionStatus(gl_pWorldMarket->GetMarketDate());
			m_mapWorldStock[pWorldStock->GetSymbol()] = m_vWorldStock.size();
			m_vWorldStock.push_back(pWorldStock);
			if (pWorldStock->GetCurrency().GetLength() > lSymbolLength) {
				lSymbolLength = pWorldStock->GetCurrency().GetLength();
			}
		}
		else {
			str = _T("发现重复代码：");
			str += pWorldStock->GetSymbol();
			gl_systemMessage.PushInnerSystemInformationMessage(str);
			setWorldStock.Delete(); // 删除此重复代码
		}
		setWorldStock.MoveNext();

		//CheckStockSymbol(pWorldStock);
	}
	setWorldStock.m_pDatabase->CommitTrans();
	setWorldStock.Close();
	SortStock();
	m_lLastTotalWorldStock = m_vWorldStock.size();
	TRACE("共装入%d Finnhub Symbol\n", m_lLastTotalWorldStock);

	char buffer[100];
	sprintf_s(buffer, _T("%d"), lSymbolLength);
	str = _T("WorldMarket股票代码最长长度为");
	str += buffer;
	//gl_systemMessage.PushInnerSystemInformationMessage(str);

	return true;
}

/// <summary>
/// 这种查询方式比较晦涩，但结果正确。目前使用此函数。(可能出现存储多个相同代码的问题，研究之）
/// </summary>
/// <param name=""></param>
/// <returns></returns>
bool CDataWorldStock::UpdateProfileDB(void) {
	static bool sm_fInProcess = false;
	CWorldStockPtr pStock = nullptr;
	CSetWorldStock setWorldStock;
	int iStockNeedUpdate = 0;
	int iCurrentUpdated = 0;
	time_t tt = GetTickCount64();

	if (sm_fInProcess) {
		gl_systemMessage.PushErrorMessage(_T("UpdateStockProfileDB任务用时超过五分钟"));
		return false;
	}
	else {
		sm_fInProcess = true;
	}

	//更新原有的代码集状态
	if (IsStockProfileNeedUpdate()) {
		for (auto& pStock2 : m_vWorldStock) {
			if (pStock2->IsUpdateProfileDB()) iStockNeedUpdate++;
		}
		setWorldStock.m_strSort = _T("[Symbol]");
		setWorldStock.Open();
		setWorldStock.m_pDatabase->BeginTrans();
		while (iCurrentUpdated < iStockNeedUpdate) {
			if (setWorldStock.IsEOF()) break;
			pStock = GetStock(setWorldStock.m_Symbol);
			if (pStock->IsUpdateProfileDB()) {
				iCurrentUpdated++;
				pStock->Update(setWorldStock);
				pStock->SetUpdateProfileDB(false);
			}
			setWorldStock.MoveNext();
		}
		if (iCurrentUpdated < iStockNeedUpdate) {
			if (!setWorldStock.IsEOF()) setWorldStock.MoveLast();
			if (!setWorldStock.IsEOF()) setWorldStock.MoveNext();
			for (auto& pStock3 : m_vWorldStock) {
				if (pStock3->IsUpdateProfileDB()) {
					//ASSERT(pStock3->IsTodayNewStock()); // 所有的新股票，都是今天新生成的
					iCurrentUpdated++;
					pStock3->Append(setWorldStock);
					pStock3->SetTodayNewStock(false);
					TRACE("存储股票：%s\n", pStock3->GetSymbol().GetBuffer());
				}
				if (iCurrentUpdated >= iStockNeedUpdate) break;
			}
		}
		setWorldStock.m_pDatabase->CommitTrans();
		setWorldStock.Close();
		m_lLastTotalWorldStock = m_vWorldStock.size();
	}
	ASSERT(iCurrentUpdated == iStockNeedUpdate);
	tt = GetTickCount64() - tt;
	char buffer3[30];
	sprintf_s(buffer3, _T("%lld"), tt);
	CString strMessage = _T("更新WorldMarket stock用时");
	strMessage += buffer3;
	strMessage += _T("毫秒,共更新了");
	sprintf_s(buffer3, _T("%d"), iCurrentUpdated);
	strMessage += buffer3;
	strMessage += _T("个股票");
	//gl_systemMessage.PushInnerSystemInformationMessage(strMessage);

	sm_fInProcess = false;
	return true;
}

bool CDataWorldStock::UpdateBasicFinancialDB(void) {
	static bool sm_fInProcess = false;
	vector<CWorldStockPtr> vStock;
	CWorldStockPtr pStock = nullptr;

	if (sm_fInProcess) {
		gl_systemMessage.PushErrorMessage(_T("UpdateBasicFinancialDB任务用时超过五分钟"));
		return false;
	}
	else {
		sm_fInProcess = true;
	}
	for (auto& pStock4 : m_vWorldStock) {
		if (pStock4->IsUpdateBasicFinancialDB()) {
			vStock.push_back(pStock4);
		}
	}

	UpdateBasicFinancialAnnualDB(vStock);
	UpdateBasicFinancialQuarterDB(vStock);
	UpdateBasicFinancialMetricDB(vStock);

	for (auto& pStock5 : vStock) {
		pStock5->SetUpdateBasicFinancialDB(false);
	}

	sm_fInProcess = false;
	return true;
}

bool CDataWorldStock::UpdateBasicFinancialMetricDB(vector<CWorldStockPtr> vStock) {
	CWorldStockPtr pStock = nullptr;
	CSetFinnhubStockBasicFinancialMetric setBasicFinancialMetric;
	int iBasicFinancialNeedUpdate = 0;
	int iCurrentUpdated = 0;

	//更新原有的基本财务信息
	if (IsBasicFinancialNeedUpdate()) {
		iBasicFinancialNeedUpdate = vStock.size();
		setBasicFinancialMetric.m_strSort = _T("[Symbol]");
		setBasicFinancialMetric.Open();
		setBasicFinancialMetric.m_pDatabase->BeginTrans();
		while (iCurrentUpdated < iBasicFinancialNeedUpdate) {
			if (setBasicFinancialMetric.IsEOF()) break;
			pStock = GetStock(setBasicFinancialMetric.m_symbol);
			if (vStock.end() != find(vStock.begin(), vStock.end(), pStock)) {
				iCurrentUpdated++;
				pStock->UpdateBasicFinancialMetric(setBasicFinancialMetric);
			}
			setBasicFinancialMetric.MoveNext();
		}
		if (iCurrentUpdated < iBasicFinancialNeedUpdate) {
			ASSERT(setBasicFinancialMetric.IsEOF());
			if (!setBasicFinancialMetric.IsEOF()) setBasicFinancialMetric.MoveLast();
			if (!setBasicFinancialMetric.IsEOF()) setBasicFinancialMetric.MoveNext();
			for (int i = iCurrentUpdated; i < iBasicFinancialNeedUpdate; i++) {
				auto& pStock3 = vStock.at(i);
				ASSERT(pStock3->IsUpdateBasicFinancialDB());
				iCurrentUpdated++;
				pStock3->AppendBasicFinancialMetric(setBasicFinancialMetric);
				ASSERT(iCurrentUpdated <= iBasicFinancialNeedUpdate);
			}
		}
		setBasicFinancialMetric.m_pDatabase->CommitTrans();
		setBasicFinancialMetric.Close();
	}

	return true;
}

bool CDataWorldStock::UpdateBasicFinancialQuarterDB(vector<CWorldStockPtr> vStock) {
	for (auto& pStock : vStock) {
		thread thread1(ThreadUpdateBasicFinancialQuarterlyDB, pStock.get());
		thread1.detach();
	}
	return true;
}

bool CDataWorldStock::UpdateBasicFinancialAnnualDB(vector<CWorldStockPtr> vStock) {
	for (auto& pStock : vStock) {
		thread thread1(ThreadUpdateBasicFinancialAnnualDB, pStock.get());
		thread1.detach();
	}
	return true;
}

bool CDataWorldStock::CheckStockSymbol(CWorldStockPtr pStock) {
	CString strSymbol = pStock->GetSymbol();
	CString strExhangeCode = pStock->GetExchangeCode();

	if (strExhangeCode.Compare(_T("US")) == 0) return true;
	int pos = strSymbol.Find(_T(".") + strExhangeCode);
	if ((pos + 1) < (strSymbol.GetLength() - strExhangeCode.GetLength())) {
		gl_systemMessage.PushErrorMessage(_T("stock sysmbol Error: ") + strSymbol);
		return false;
	}
	return true;
}

bool CDataWorldStock::IsNeedSaveDayLine(void) {
	for (auto& pStock : m_vWorldStock) {
		if (pStock->IsDayLineNeedSaving()) return true;
	}
	return false;
}

bool CDataWorldStock::IsNeedSaveInsiderTransaction(void) {
	for (auto& pStock : m_vWorldStock) {
		if (pStock->IsInsiderTransactionNeedSave()) return true;
	}
	return false;
}