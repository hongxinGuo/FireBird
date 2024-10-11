//////////////////////////////////////////////////////////////////////////////////////////
//
// CContainerWorldStock�����worldMarket���ݿ��е�stock_profile��
//
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "pch.h"

#include "InfoReport.h"
#include"WorldMarket.h"

#include"SetWorldStock.h"

#include "ContainerWorldStock.h"

CContainerWorldStock::CContainerWorldStock() {
	CContainerWorldStock::Reset();
}

void CContainerWorldStock::Reset() {
	CContainerVirtualStock::Reset();
}

void CContainerWorldStock::ResetEPSSurprise() {
	for (size_t l = 0; l < m_vStock.size(); l++) {
		const CWorldStockPtr pStock = GetStock(l);
		pStock->SetLastEPSSurpriseUpdateDate(19800101);
		pStock->m_fUpdateEPSSurprise = true;
	}
}

void CContainerWorldStock::ResetPeer() {
	for (size_t l = 0; l < m_vStock.size(); l++) {
		const CWorldStockPtr pStock = GetStock(l);
		if (pStock->GetPeerUpdateDate() != 19800101) {
			pStock->SetPeerUpdateDate(19800101);
			pStock->SetUpdatePeer(true);
			pStock->SetUpdateProfileDB(true);
		}
	}
}

void CContainerWorldStock::ResetBasicFinancial() {
	for (size_t l = 0; l < m_vStock.size(); l++) {
		const CWorldStockPtr pStock = GetStock(l);
		if (pStock->GetBasicFinancialUpdateDate() != 19800101) {
			pStock->SetBasicFinancialUpdateDate(19800101);
			pStock->SetUpdateBasicFinancial(true);
			pStock->SetUpdateProfileDB(true);
		}
	}
}

void CContainerWorldStock::ResetDayLine() {
	for (size_t l = 0; l < m_vStock.size(); l++) {
		const CWorldStockPtr pStock = GetStock(l);
		pStock->SetIPOStatus(_STOCK_NOT_CHECKED_);
		pStock->SetDayLineStartDate(29900101);
		pStock->SetDayLineEndDate(19800101);
		pStock->SetUpdateDayLine(true);
		pStock->SetUpdateProfileDB(true);
	}
}

bool CContainerWorldStock::LoadDB() {
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

		//ValidateStockSymbol(pWorldStock);
	}
	setWorldStock.m_pDatabase->CommitTrans();
	setWorldStock.Close();
	Sort();

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
void CContainerWorldStock::UpdateProfileDB() {
	if (IsUpdateProfileDB()) {
		try {
			CSetWorldStock setWorldStock;
			int iCurrentUpdated = 0;
			int iStockNeedUpdate = 0;
			for (const auto& pStock : m_vStock) {
				if (pStock->IsUpdateProfileDB()) iStockNeedUpdate++;
			}
			if (iStockNeedUpdate > 500) iStockNeedUpdate = 500; // ÿ�θ���500��,��֤������ռ�ù���ʱ�䣨500��������Ҫ2-5���ӣ���ÿ������һ�εĻ�����������300����
			setWorldStock.m_strSort = _T("[Symbol]");
			setWorldStock.Open();
			setWorldStock.m_pDatabase->BeginTrans();
			while (iCurrentUpdated < iStockNeedUpdate) {	//����ԭ�еĴ��뼯״̬
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
			if (iCurrentUpdated < iStockNeedUpdate) { // ����µĹ�Ʊ���
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
		} catch (CException* e) {
			ReportInformationAndDeleteException(e);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
//
// ���»���������Ϣ
//
//
////////////////////////////////////////////////////////////////////////////////////////////
bool CContainerWorldStock::UpdateBasicFinancialDB() {
	static bool s_fInProcess = false;
	vector<CWorldStockPtr> vStock{};

	if (s_fInProcess) {
		gl_systemMessage.PushErrorMessage(_T("UpdateBasicFinancialDB������ʱ���������"));
		return false;
	}
	s_fInProcess = true;

	vStock.clear();
	for (size_t l = 0; l < m_vStock.size(); l++) {
		const CWorldStockPtr pStock = GetStock(l);
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
// ��������Ƚ�ռ��CPU�ļ����������ʶ���ʹ�ý�����CPUʱ����ʹ�ù����߳������д���
//
// ����ʹ�ò��д���ʽʱ�����ݿ�ż������ֹ��ϣ�������MySQL���ݿ��ͬ�����⣬Ŀǰ�������ҵ�����֮���ڡ�
// ��������ʹ�ô��з�ʽ�����������̡߳�--20221101
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////
void CContainerWorldStock::UpdateBasicFinancialQuarterDB(const vector<CWorldStockPtr>& vStock) {
	for (const auto& pStock : vStock) {
		if (gl_systemConfiguration.IsExitingSystem()) break;
		pStock->AppendBasicFinancialQuarter();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
// ��������Ƚ�ռ��CPU�ļ����������ʶ���ʹ�ý�����CPUʱ����ʹ�ù����߳������д���
//
// ����ʹ�ò��д���ʽʱ�����ݿ�ż������ֹ��ϣ�������MySQL���ݿ��ͬ�����⣬Ŀǰ�������ҵ�����֮���ڡ�
// ��������ʹ�ô��з�ʽ�����������̡߳�--20221101
//
//////////////////////////////////////////////////////////////////////////////////////////////////
void CContainerWorldStock::UpdateBasicFinancialAnnualDB(const vector<CWorldStockPtr>& vStock) {
	for (const auto& pStock : vStock) {
		if (gl_systemConfiguration.IsExitingSystem()) break;
		pStock->AppendBasicFinancialAnnual();
	}
}

void CContainerWorldStock::UpdateBasicFinancialMetricDB(const vector<CWorldStockPtr>& vStock) {
	try {
		CSetFinnhubStockBasicFinancialMetric setBasicFinancialMetric;
		const auto iBasicFinancialNeedUpdate = vStock.size();
		size_t iCurrentUpdated = 0;

		ASSERT(IsUpdateBasicFinancialDB());
		setBasicFinancialMetric.m_strSort = _T("[Symbol]");
		setBasicFinancialMetric.Open();
		setBasicFinancialMetric.m_pDatabase->BeginTrans();
		//����ԭ�еĻ���������Ϣ
		while (iCurrentUpdated < iBasicFinancialNeedUpdate) {
			if (setBasicFinancialMetric.IsEOF()) break;
			CWorldStockPtr pStockNeedUpdate = GetStock(setBasicFinancialMetric.m_symbol);
			if (vStock.end() != std::ranges::find(vStock.begin(), vStock.end(), pStockNeedUpdate)) {
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
	} catch (CException* e) {
		ReportInformationAndDeleteException(e);
	}
}

void CContainerWorldStock::ClearUpdateBasicFinancialFlag(const vector<CWorldStockPtr>& vStock) {
	for (const auto& pStock : vStock) {
		if (pStock->IsUpdateBasicFinancialDB()) {
			pStock->SetUpdateBasicFinancialDB(false);
		}
	}
}

bool CContainerWorldStock::ValidateStockSymbol(const CWorldStockPtr& pStock) {
	const CString strSymbol = pStock->GetSymbol();
	const CString strExchangeCode = pStock->GetExchangeCode();

	if (strExchangeCode.Compare(_T("US")) == 0) return true;
	const int pos = strSymbol.Find(_T(".") + strExchangeCode);
	if ((pos + 1) < (strSymbol.GetLength() - strExchangeCode.GetLength())) {
		return false;
	}
	return true;
}

bool CContainerWorldStock::IsUpdateCompanyNewsDB() {
	return std::ranges::any_of(m_vStock, [](const CVirtualStockPtr& pStock) { return dynamic_pointer_cast<CWorldStock>(pStock)->IsUpdateCompanyNewsDB(); });
}

bool CContainerWorldStock::IsUpdateBasicFinancialDB() {
	return std::ranges::any_of(m_vStock, [](const CVirtualStockPtr& pStock) { return dynamic_pointer_cast<CWorldStock>(pStock)->IsUpdateBasicFinancialDB(); });
}

bool CContainerWorldStock::IsUpdateInsiderTransactionDB() {
	return std::ranges::any_of(m_vStock, [](const CVirtualStockPtr& P) { return dynamic_pointer_cast<CWorldStock>(P)->IsUpdateInsiderTransactionDB(); });
}

bool CContainerWorldStock::IsUpdateInsiderSentimentDB() {
	return std::ranges::any_of(m_vStock, [](const CVirtualStockPtr& p) { return dynamic_pointer_cast<CWorldStock>(p)->IsUpdateInsiderSentimentDB(); });
}

bool CContainerWorldStock::IsUpdateEPSSurpriseDB() {
	return std::ranges::any_of(m_vStock, [](const CVirtualStockPtr& p) { return dynamic_pointer_cast<CWorldStock>(p)->IsUpdateEPSSurpriseDB(); });
}

bool CContainerWorldStock::IsUpdateSECFilingsDB() {
	return std::ranges::any_of(m_vStock, [](const CVirtualStockPtr& p) { return dynamic_pointer_cast<CWorldStock>(p)->IsUpdateSECFilingsDB(); });
}
