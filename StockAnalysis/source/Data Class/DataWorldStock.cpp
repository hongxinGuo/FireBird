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

bool CDataWorldStock::IsCompanyNewsNeedUpdate(void) {
	const int iTotal = m_vWorldStock.size();
	for (int i = 0; i < iTotal; i++) {
		if (m_vWorldStock.at(i)->IsUpdateCompanyNewsDB()) return true;
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
	long lMaxSymbolLength = 0;

	setWorldStock.m_strSort = _T("[Symbol]");
	setWorldStock.Open();
	setWorldStock.m_pDatabase->BeginTrans();
	while (!setWorldStock.IsEOF()) {
		pWorldStock = make_shared<CWorldStock>();
		pWorldStock->Load(setWorldStock);
		if (!IsStock(pWorldStock->GetSymbol())) {
			pWorldStock->CheckUpdateStatus(gl_pWorldMarket->GetMarketDate());
			m_mapWorldStock[pWorldStock->GetSymbol()] = m_vWorldStock.size();
			m_vWorldStock.push_back(pWorldStock);
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
	SortStock();
	m_lLastTotalWorldStock = m_vWorldStock.size();
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
	else {
		sm_fInProcess = true;
	}

	//����ԭ�еĴ��뼯״̬
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
					//ASSERT(pStock3->IsTodayNewStock()); // ���е��¹�Ʊ�����ǽ��������ɵ�
					iCurrentUpdated++;
					pStock3->Append(setWorldStock);
					pStock3->SetTodayNewStock(false);
					TRACE("�洢��Ʊ��%s\n", pStock3->GetSymbol().GetBuffer());
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
	else {
		s_fInProcess = true;
	}

	s_vStock.clear();
	for (auto& pStock4 : m_vWorldStock) {
		if (pStock4->IsUpdateBasicFinancialDB()) {
			s_vStock.push_back(pStock4);
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
	for (auto& pStock : vStock) {
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
	for (auto& pStock : vStock) {
		if (gl_systemStatus.IsExitingSystem()) break;
		pStock->AppendBasicFinancialAnnual();
	}
	return true;
}

bool CDataWorldStock::UpdateBasicFinancialMetricDB(vector<CWorldStockPtr> vStock) {
	CSetFinnhubStockBasicFinancialMetric setBasicFinancialMetric;
	int iBasicFinancialNeedUpdate = vStock.size();
	int iCurrentUpdated = 0;

	ASSERT(IsBasicFinancialNeedUpdate());
	setBasicFinancialMetric.m_strSort = _T("[Symbol]");
	setBasicFinancialMetric.Open();
	setBasicFinancialMetric.m_pDatabase->BeginTrans();
	//����ԭ�еĻ���������Ϣ
	while (iCurrentUpdated < iBasicFinancialNeedUpdate) {
		if (setBasicFinancialMetric.IsEOF()) break;
		auto pStockNeedUpdate = GetStock(setBasicFinancialMetric.m_symbol);
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
			auto& pStockNeedAppend = vStock.at(i);
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
	for (auto& pStock5 : vStock) {
		if (pStock5->IsUpdateBasicFinancialDB()) {
			CString strMessage = _T("found stock") + pStock5->GetSymbol() + _T(" need update basic financial data");
			TRACE(strMessage);
			gl_systemMessage.PushErrorMessage(strMessage);
		}
		pStock5->SetUpdateBasicFinancialDB(false);
	}
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

bool CDataWorldStock::IsNeedSaveInsiderSentiment(void) {
	for (auto& pStock : m_vWorldStock) {
		if (pStock->IsInsiderSentimentNeedSave()) return true;
	}
	return false;
}