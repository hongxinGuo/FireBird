//////////////////////////////////////////////////////////////////////////////////////////
//
// CDataWorldStock�����worldmarket���ݿ��е�stock_profile��
//
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "pch.h"

#include"thread.h"
#include"WorldMarket.h"

#include"SetWorldStock.h"

#include "DataWorldStock.h"

using namespace std;

CDataWorldStock::CDataWorldStock() {
	CDataWorldStock::Reset();
}

void CDataWorldStock::Reset(void) {
	CContainerVirtualStock::Reset();
	m_lLastTotalWorldStock = 0;
}

bool CDataWorldStock::IsCompanyNewsNeedUpdate(void) {
	return ranges::any_of(m_vStock, [](const CVirtualStockPtr& pStock) { return dynamic_pointer_cast<CWorldStock>(pStock)->IsUpdateCompanyNewsDB(); });
}

bool CDataWorldStock::IsBasicFinancialNeedUpdate(void) {
	return ranges::any_of(m_vStock, [](const CVirtualStockPtr& pStock) { return dynamic_pointer_cast<CWorldStock>(pStock)->IsUpdateBasicFinancialDB(); });
}

void CDataWorldStock::ResetEPSSurprise(void) {
	for (auto& p : m_vStock) {
		dynamic_pointer_cast<CWorldStock>(p)->SetLastEPSSurpriseUpdateDate(19800101);
		dynamic_pointer_cast<CWorldStock>(p)->m_fEPSSurpriseUpdated = false;
	}
}

void CDataWorldStock::ResetPeer(void) {
	for (const auto& pStock : m_vStock) {
		if (dynamic_pointer_cast<CWorldStock>(pStock)->GetPeerUpdateDate() != 19800101) {
			dynamic_pointer_cast<CWorldStock>(pStock)->SetPeerUpdateDate(19800101);
			dynamic_pointer_cast<CWorldStock>(pStock)->SetPeerUpdated(false);
			dynamic_pointer_cast<CWorldStock>(pStock)->SetUpdateProfileDB(true);
		}
	}
}

void CDataWorldStock::ResetBasicFinancial(void) {
	for (const auto& pStock : m_vStock) {
		if (dynamic_pointer_cast<CWorldStock>(pStock)->GetBasicFinancialUpdateDate() != 19800101) {
			dynamic_pointer_cast<CWorldStock>(pStock)->SetBasicFinancialUpdateDate(19800101);
			dynamic_pointer_cast<CWorldStock>(pStock)->SetBasicFinancialUpdated(false);
			dynamic_pointer_cast<CWorldStock>(pStock)->SetUpdateProfileDB(true);
		}
	}
}

void CDataWorldStock::ResetDayLine(void) {
	for (const auto& pStock : m_vStock) {
		dynamic_pointer_cast<CWorldStock>(pStock)->SetIPOStatus(_STOCK_NOT_CHECKED_);
		dynamic_pointer_cast<CWorldStock>(pStock)->SetDayLineStartDate(29900101);
		dynamic_pointer_cast<CWorldStock>(pStock)->SetDayLineEndDate(19800101);
		dynamic_pointer_cast<CWorldStock>(pStock)->SetDayLineNeedUpdate(true);
		dynamic_pointer_cast<CWorldStock>(pStock)->SetUpdateProfileDB(true);
	}
}

bool CDataWorldStock::LoadDB(void) {
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
			str = _T("�����ظ����룺");
			str += pWorldStock->GetSymbol();
			gl_systemMessage.PushInnerSystemInformationMessage(str);
			setWorldStock.Delete(); // ɾ�����ظ�����
		}
		setWorldStock.MoveNext();

		//CheckStockSymbol(pWorldStock);
	}
	setWorldStock.m_pDatabase->CommitTrans();
	setWorldStock.Close();
	Sort();
	m_lLastTotalWorldStock = m_vStock.size();
	TRACE("��װ��%d Finnhub Symbol\n", m_lLastTotalWorldStock);

	ASSERT(lMaxSymbolLength < 20); // ĿǰWorldMarket���ݿ�Ĺ�Ʊ���볤������Ϊ20���ַ�
	char buffer[100];
	sprintf_s(buffer, _T("%d"), lMaxSymbolLength);
	str = _T("WorldMarket��Ʊ���������Ϊ");
	str += buffer;
#ifdef _DEBUG
	gl_systemMessage.PushInnerSystemInformationMessage(str);
#endif

	return true;
}

/// <summary>
/// ���ֲ�ѯ��ʽ�Ƚϻ�ɬ���������ȷ��Ŀǰʹ�ô˺�����(���ܳ��ִ洢�����ͬ��������⣬�о�֮��
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
		gl_systemMessage.PushErrorMessage(_T("UpdateStockProfileDB������ʱ���������"));
		return false;
	}
	sm_fInProcess = true;

	//����ԭ�еĴ��뼯״̬
	if (IsUpdateProfileDB()) {
		for (const auto& pStock2 : m_vStock) {
			if (pStock2->IsUpdateProfileDB()) iStockNeedUpdate++;
		}
		setWorldStock.m_strSort = _T("[Symbol]");
		setWorldStock.Open();
		setWorldStock.m_pDatabase->BeginTrans();
		while (iCurrentUpdated < iStockNeedUpdate) {
			if (setWorldStock.IsEOF()) break;
			pStock = dynamic_pointer_cast<CWorldStock>(Get(setWorldStock.m_Symbol));
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
			for (const auto& pStock3 : m_vStock) {
				if (pStock3->IsUpdateProfileDB()) {
					//ASSERT(pStock3->IsTodayNewStock()); // ���е��¹�Ʊ�����ǽ��������ɵ�
					iCurrentUpdated++;
					dynamic_pointer_cast<CWorldStock>(pStock3)->Append(setWorldStock);
					dynamic_pointer_cast<CWorldStock>(pStock3)->SetTodayNewStock(false);
					TRACE("�洢��Ʊ��%s\n", pStock3->GetSymbol().GetBuffer());
				}
				if (iCurrentUpdated >= iStockNeedUpdate) break;
			}
		}
		setWorldStock.m_pDatabase->CommitTrans();
		setWorldStock.Close();
		m_lLastTotalWorldStock = m_vStock.size();
	}
	ASSERT(iCurrentUpdated == iStockNeedUpdate);
	tt = GetTickCount64() - tt;
	char buffer3[30];
	sprintf_s(buffer3, _T("%lld"), tt);
	CString strMessage = _T("����WorldMarket stock��ʱ");
	strMessage += buffer3;
	strMessage += _T("����,��������");
	sprintf_s(buffer3, _T("%d"), iCurrentUpdated);
	strMessage += buffer3;
	strMessage += _T("����Ʊ");
	//gl_systemMessage.PushInnerSystemInformationMessage(strMessage);

	sm_fInProcess = false;
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//
// ���»���������Ϣ
//
// Ϊ�˼��ٺ�����ִ�У�UpdateBasicFinancialAnnualDB��UpdateBasicFinancialQuarterDB�����ɶ����Ĺ����̣߳�����Ĳ���vStock
// ����Щ�����̳߳����ڼ�ͱ��뱣����Ч���ʶ����뽫vStock����Ϊstatic�ģ��������ܹ���֤������������UpdateBasicFinancialDBִ��
// ��Ϻ�vStock���ڴ���ͷ��ˣ��������߳̿�����δִ�����ǰ������vStock����Ч�ˡ�
//
////////////////////////////////////////////////////////////////////////////////////////////
bool CDataWorldStock::UpdateBasicFinancialDB(void) {
	static bool s_fInProcess = false;
	static vector<CWorldStockPtr> s_vStock{}; // �������Ҫʹ�þ�̬�洢���Ա�֤���������˳�ʱ��������Ȼ����Ч�ģ����������ɵĹ����߳������δִ����ϣ���ȻҪʹ��֮��
	CWorldStockPtr pStock = nullptr;

	if (s_fInProcess) {
		gl_systemMessage.PushErrorMessage(_T("UpdateBasicFinancialDB������ʱ���������"));
		return false;
	}
	s_fInProcess = true;

	s_vStock.clear();
	for (auto& pStock4 : m_vStock) {
		if (dynamic_pointer_cast<CWorldStock>(pStock4)->IsUpdateBasicFinancialDB()) {
			s_vStock.push_back(dynamic_pointer_cast<CWorldStock>(pStock4));
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
// ��������Ƚ�ռ��CPU�ļ����������ʶ���ʹ�ý�����CPUʱ����ʹ�ù����߳������д���
//
// ����ʹ�ò��д���ʽʱ�����ݿ�ż������ֹ��ϣ�������MySQL���ݿ��ͬ�����⣬Ŀǰ�������ҵ�����֮���ڡ�
// ��������ʹ�ô��з�ʽ�����������̡߳�--20221101
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////
bool CDataWorldStock::UpdateBasicFinancialQuarterDB(vector<CWorldStockPtr> vStock) {
	for (const auto& pStock : vStock) {
		if (gl_systemStatus.IsExitingSystem()) break;
		pStock->AppendBasicFinancialQuarter();
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
// ��������Ƚ�ռ��CPU�ļ����������ʶ���ʹ�ý�����CPUʱ����ʹ�ù����߳������д���
//
// ����ʹ�ò��д���ʽʱ�����ݿ�ż������ֹ��ϣ�������MySQL���ݿ��ͬ�����⣬Ŀǰ�������ҵ�����֮���ڡ�
// ��������ʹ�ô��з�ʽ�����������̡߳�--20221101
//
//////////////////////////////////////////////////////////////////////////////////////////////////
bool CDataWorldStock::UpdateBasicFinancialAnnualDB(vector<CWorldStockPtr> vStock) {
	for (const auto& pStock : vStock) {
		if (gl_systemStatus.IsExitingSystem()) break;
		pStock->AppendBasicFinancialAnnual();
	}
	return true;
}

bool CDataWorldStock::UpdateBasicFinancialMetricDB(vector<CWorldStockPtr> vStock) {
	CSetFinnhubStockBasicFinancialMetric setBasicFinancialMetric;
	auto iBasicFinancialNeedUpdate = vStock.size();
	size_t iCurrentUpdated = 0;

	ASSERT(IsBasicFinancialNeedUpdate());
	setBasicFinancialMetric.m_strSort = _T("[Symbol]");
	setBasicFinancialMetric.Open();
	setBasicFinancialMetric.m_pDatabase->BeginTrans();
	//����ԭ�еĻ���������Ϣ
	while (iCurrentUpdated < iBasicFinancialNeedUpdate) {
		if (setBasicFinancialMetric.IsEOF()) break;
		CWorldStockPtr pStockNeedUpdate = dynamic_pointer_cast<CWorldStock>(Get(setBasicFinancialMetric.m_symbol));
		if (vStock.end() != find(vStock.begin(), vStock.end(), pStockNeedUpdate)) {
			iCurrentUpdated++;
			pStockNeedUpdate->UpdateBasicFinancialMetric(setBasicFinancialMetric);
			pStockNeedUpdate->SetUpdateBasicFinancialDB(false);
		}
		setBasicFinancialMetric.MoveNext();
	}
	// ����µĻ�����������
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

void CDataWorldStock::ClearUpdateBasicFinancialFlag(vector<CWorldStockPtr> vStock) {
	for (const auto& pStock : vStock) {
		if (pStock->IsUpdateBasicFinancialDB()) {
			CString strMessage = _T("found stock:") + pStock->GetSymbol() + _T(" need update basic financial data");
			TRACE(strMessage);
			gl_systemMessage.PushErrorMessage(strMessage);
		}
		pStock->SetUpdateBasicFinancialDB(false);
	}
}

bool CDataWorldStock::CheckStockSymbol(CWorldStockPtr pStock) {
	CString strSymbol = pStock->GetSymbol();
	const CString strExchangeCode = pStock->GetExchangeCode();

	if (strExchangeCode.Compare(_T("US")) == 0) return true;
	const int pos = strSymbol.Find(_T(".") + strExchangeCode);
	if ((pos + 1) < (strSymbol.GetLength() - strExchangeCode.GetLength())) {
		gl_systemMessage.PushErrorMessage(_T("stock sysmbol Error: ") + strSymbol);
		return false;
	}
	return true;
}

bool CDataWorldStock::IsNeedSaveInsiderTransaction(void) {
	return ranges::any_of(m_vStock, [](CVirtualStockPtr& P) { return dynamic_pointer_cast<CWorldStock>(P)->IsInsiderTransactionNeedSave(); });
}

bool CDataWorldStock::IsNeedSaveInsiderSentiment(void) {
	return ranges::any_of(m_vStock, [](CVirtualStockPtr& p) { return dynamic_pointer_cast<CWorldStock>(p)->IsInsiderSentimentNeedSave(); });
}
