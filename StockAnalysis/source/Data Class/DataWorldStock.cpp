//////////////////////////////////////////////////////////////////////////////////////////
//
// CDataWorldStock�����worldmarket���ݿ��е�stock_profile��
//
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "pch.h"

#include"globedef.h"

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
			str = _T("�����ظ����룺");
			str += pWorldStock->GetSymbol();
			gl_systemMessage.PushInnerSystemInformationMessage(str);
			setWorldStock.Delete(); // ɾ�����ظ�����
		}
		setWorldStock.MoveNext();
	}
	setWorldStock.m_pDatabase->CommitTrans();
	setWorldStock.Close();
	SortStock();
	m_lLastTotalWorldStock = m_vWorldStock.size();
	TRACE("��װ��%d Finnhub Symbol\n", m_lLastTotalWorldStock);

	char buffer[100];
	sprintf_s(buffer, _T("%d"), lSymbolLength);
	str = _T("WorldMarket��Ʊ���������Ϊ");
	str += buffer;
	gl_systemMessage.PushInnerSystemInformationMessage(str);

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
	int iUpdatedStock = 0;
	int iCount = 0;
	int iUpdatedNumber = 0;
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
			if (pStock2->IsUpdateProfileDB()) iUpdatedStock++;
		}
		setWorldStock.m_strSort = _T("[Symbol]");
		setWorldStock.Open();
		setWorldStock.m_pDatabase->BeginTrans();
		while (iCount < iUpdatedStock) {
			if (setWorldStock.IsEOF()) break;
			pStock = GetStock(setWorldStock.m_Symbol);
			if (pStock->IsUpdateProfileDB()) {
				iCount++;
				iUpdatedNumber++;
				pStock->Update(setWorldStock);
				pStock->SetUpdateProfileDB(false);
			}
			setWorldStock.MoveNext();
		}
		if (iCount < iUpdatedStock) {
			if (!setWorldStock.IsEOF()) setWorldStock.MoveLast();
			if (!setWorldStock.IsEOF()) setWorldStock.MoveNext();
			for (auto& pStock3 : m_vWorldStock) {
				if (pStock3->IsUpdateProfileDB()) {
					//ASSERT(pStock3->IsTodayNewStock()); // ���е��¹�Ʊ�����ǽ��������ɵ�
					iCount++;
					iUpdatedNumber++;
					pStock3->Append(setWorldStock);
					pStock3->SetTodayNewStock(false);
					TRACE("�洢��Ʊ��%s\n", pStock3->GetSymbol().GetBuffer());
				}
				if (iCount >= iUpdatedStock) break;
			}
		}
		setWorldStock.m_pDatabase->CommitTrans();
		setWorldStock.Close();
		m_lLastTotalWorldStock = m_vWorldStock.size();
	}
	ASSERT(iCount == iUpdatedStock);
	tt = GetTickCount64() - tt;
	char buffer3[30];
	sprintf_s(buffer3, _T("%lld"), tt);
	CString strMessage = _T("����WorldMarket stock��ʱ");
	strMessage += buffer3;
	strMessage += _T("����,��������");
	sprintf_s(buffer3, _T("%d"), iUpdatedNumber);
	strMessage += buffer3;
	strMessage += _T("����Ʊ");
	gl_systemMessage.PushInnerSystemInformationMessage(strMessage);

	sm_fInProcess = false;
	return true;
}