#include "pch.h"
module;
#include"SetStockSection.h"
module FireBird.Container.StockSymbol;

import FireBird.Accessory.InfoReport;
import FireBird.Accessory.ChinaStockCodeConverter;
import FireBird.System.ConstantChinaMarket;
import FireBird.Stock.ChinaStock;

CContainerStockSymbol::CContainerStockSymbol() {
	CContainerStockSymbol::Reset();
}

void CContainerStockSymbol::Reset() {
	CVirtualContainer::Reset();

	m_vStockSymbol.resize(0);
	m_mapStockSymbol.clear();
	m_vCurrentSectionStockCode.resize(0);
	// Ԥ���Ʊ���뼯����
	m_vCurrentSectionStockCode.push_back(_T("000000.SS")); // �Ϻ�ָ��
	m_vCurrentSectionStockCode.push_back(_T("600000.SS")); // �Ϻ�����
	m_vCurrentSectionStockCode.push_back(_T("601000.SS")); // �Ϻ�����
	m_vCurrentSectionStockCode.push_back(_T("603000.SS")); // �Ϻ�����
	m_vCurrentSectionStockCode.push_back(_T("688000.SS")); // �Ϻ��ƴ���
	m_vCurrentSectionStockCode.push_back(_T("900000.SS")); // �Ϻ�B��
	m_vCurrentSectionStockCode.push_back(_T("000000.SZ")); // ��������
	m_vCurrentSectionStockCode.push_back(_T("001000.SZ")); // ��������
	m_vCurrentSectionStockCode.push_back(_T("002000.SZ")); // ������С��
	m_vCurrentSectionStockCode.push_back(_T("003000.SZ")); // ������С��
	m_vCurrentSectionStockCode.push_back(_T("004000.SZ")); // ������С��
	m_vCurrentSectionStockCode.push_back(_T("200000.SZ")); // ����B��
	m_vCurrentSectionStockCode.push_back(_T("300000.SZ")); // ���ڴ�ҵ��
	m_vCurrentSectionStockCode.push_back(_T("399000.SZ")); // ����ָ��
	// �ӹ�Ʊ���뼯���ݿ��ж���������Ʊ��

	//����StockSection
	m_vStockSection.resize(0);
	m_vStockSection.resize(2000); // �����1000���Ρ�
	for (int i = 0; i < 2000; i++) {
		const auto pStockSection = make_shared<CStockSection>();
		pStockSection->SetIndexNumber(i);
		if (i < 1000) pStockSection->SetMarket(_SHANGHAI_MARKET_);
		else pStockSection->SetMarket(_SHENZHEN_MARKET_);
		m_vStockSection.at(i) = pStockSection;
	}
	for (int i = 0; i < m_vStockSection.size(); i++) {
		m_vStockSection.at(i)->SetBuildStockPtr(false);
	}
	m_fUpdateStockSection = false;

	LoadStockSectionDB();

	// ���ɹ�Ʊ�����
	CreateTotalStockContainer();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// ��ʼ�����п��ܵĹ�Ʊ����أ�ֻ��CContainerStockSymbol�ĳ�ʼ������Reset����һ�Ρ�
//
////////////////////////////////////////////////////////////////////////////////////////////////////
bool CContainerStockSymbol::CreateTotalStockContainer() {
	CChinaStockPtr pStock = nullptr;

	for (int i = 0; i < m_vCurrentSectionStockCode.size(); i++) {
		CreateStockSection(m_vCurrentSectionStockCode.at(i));
	}
	return true;
}
CString CContainerStockSymbol::GetItemSymbol(size_t lIndex) {
	return m_vStockSymbol.at(lIndex);
}

size_t CContainerStockSymbol::Size() {
	{
		return m_vStockSymbol.size();
	}
}

void CContainerStockSymbol::LoadStockSectionDB() const {
	CSetStockSection setStockSection;

	setStockSection.Open();
	while (!setStockSection.IsEOF()) {
		if (!m_vStockSection.at(setStockSection.m_IndexNumber)->IsActive()) {
			m_vStockSection.at(setStockSection.m_IndexNumber)->SetActive(setStockSection.m_Active);
			m_vStockSection.at(setStockSection.m_IndexNumber)->SetMarket(setStockSection.m_Market);
			m_vStockSection.at(setStockSection.m_IndexNumber)->SetIndexNumber(setStockSection.m_IndexNumber);
			m_vStockSection.at(setStockSection.m_IndexNumber)->SetComment(setStockSection.m_Comment);
		}
		setStockSection.MoveNext();
	}
	setStockSection.Close();
}

void CContainerStockSymbol::UpdateStockSectionDB() {
	try {
		CSetStockSection setStockSection;

		setStockSection.m_strSort = _T("[ID]");
		setStockSection.Open();
		setStockSection.m_pDatabase->BeginTrans();
		if (setStockSection.IsEOF()) {// �ձ�
			for (int i = 0; i < 2000; i++) {
				const CStockSectionPtr pStockSection = m_vStockSection.at(i);
				setStockSection.AddNew();
				setStockSection.m_ID = i;
				setStockSection.m_Active = pStockSection->IsActive();
				setStockSection.m_Market = pStockSection->GetMarket();
				setStockSection.m_IndexNumber = pStockSection->GetIndexNumber();
				setStockSection.m_Comment = pStockSection->GetComment();
				setStockSection.Update();
			}
		}
		else {// ���Ѵ���
			while (!setStockSection.IsEOF()) {
				if (setStockSection.m_Active != m_vStockSection.at(setStockSection.m_ID)->IsActive()) {
					setStockSection.Edit();
					setStockSection.m_Active = m_vStockSection.at(setStockSection.m_ID)->IsActive();
					setStockSection.m_Market = m_vStockSection.at(setStockSection.m_ID)->GetMarket();
					setStockSection.m_IndexNumber = m_vStockSection.at(setStockSection.m_ID)->GetIndexNumber();
					setStockSection.m_Comment = m_vStockSection.at(setStockSection.m_ID)->GetComment();
					setStockSection.Update();
				}
				setStockSection.MoveNext();
			}
		}
		setStockSection.m_pDatabase->CommitTrans();
		setStockSection.Close();

		m_fUpdateStockSection = false;
	} catch (CException* e) {
		ReportInformationAndDeleteException(e);
	}
}

void CContainerStockSymbol::CreateStockSection(const CString& strFirstStockCode) {
	CString strCode = GetStockSymbol(strFirstStockCode);
	CString str = _T("");
	const int iCode = atoi(strCode.GetBuffer());
	int iMarket = 0;
	char buffer[10];
	CChinaStockPtr pStock = nullptr;

	if (IsShanghaiExchange(strFirstStockCode)) {// �Ϻ��г�
		iMarket = 0;
	}
	else if (IsShenzhenExchange(strFirstStockCode)) {// �����г�
		iMarket = 1000;
	}
	if (m_vStockSection.at((iCode / 1000) + iMarket)->IsBuildStockPtr()) return; // �Ѿ���֤ȯ���н�����
	// �����Ϻ���Ʊ����
	for (int i = iCode; i < (iCode + 1000); i++) {
		const CString strExchange = GetStockExchange(strFirstStockCode);
		sprintf_s(buffer, _T("%06d"), i);
		const CString strStockSymbol = buffer;
		const CString strStockCode = CreateStockCode(strExchange, strStockSymbol);
		Add(strStockCode);
	}
	if (UpdateStockSection(iCode / 1000 + iMarket)) {
		SetUpdateStockSection(true);
	}
	m_vStockSection.at(iCode / 1000 + iMarket)->SetBuildStockPtr(true); // �Ѿ���֤ȯ���н�����
}

bool CContainerStockSymbol::UpdateStockSection(const CString& strStockCode) const {
	CString strCode = GetStockSymbol(strStockCode);
	char* pChar;
	const int iCode = strtol(strCode.GetBuffer(), &pChar, 10);
	int iMarket = 0;

	if (IsShanghaiExchange(strStockCode)) {// �Ϻ��г�
		iMarket = 0;
	}
	else if (IsShenzhenExchange(strStockCode)) {// �����г�
		iMarket = 1000;
	}
	return UpdateStockSection(iCode / 1000 + iMarket);
}

bool CContainerStockSymbol::UpdateStockSection(const size_t lIndex) const {
	if (!m_vStockSection.at(lIndex)->IsActive()) {
		m_vStockSection.at(lIndex)->SetActive(true);
		return true;
	}
	return false;
}

bool CContainerStockSymbol::Delete(const CString& strSymbol) {
	if (!IsStockSymbol(strSymbol)) return false;

	m_vStockSymbol.erase(m_vStockSymbol.begin() + m_mapStockSymbol.at(strSymbol));
	m_mapStockSymbol.erase(strSymbol);

	return true;
}

void CContainerStockSymbol::Add(const CString& strSymbol) {
	m_mapStockSymbol[strSymbol] = m_vStockSymbol.size();
	m_vStockSymbol.push_back(strSymbol);
}
