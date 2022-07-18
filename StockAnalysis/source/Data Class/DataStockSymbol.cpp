#include "pch.h"

#include"StockCodeConverter.h"
#include "DataStockSymbol.h"

CDataStockSymbol::CDataStockSymbol() {
	m_vStockSection.resize(2000); // �����1000���Ρ�
	CStockSectionPtr pStockSection;
	for (int i = 0; i < 2000; i++) {
		pStockSection = make_shared<CStockSection>();
		pStockSection->SetIndexNumber(i);
		if (i < 1000) pStockSection->SetMarket(__SHANGHAI_MARKET__);
		else pStockSection->SetMarket(__SHENZHEN_MARKET__);
		m_vStockSection.at(i) = pStockSection;
	}

	Reset();
}

CDataStockSymbol::~CDataStockSymbol()
{
}

void CDataStockSymbol::Reset(void) {
	m_fUpdateStockSection = false;
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
	for (int i = 0; i < m_vStockSection.size(); i++) {
		m_vStockSection.at(i)->SetBuildStockPtr(false);
	}

	LoadStockSectionDB();

	// ���ɹ�Ʊ�����
	CreateTotalStockContainer();

	m_lNextSinaStockInquiringMiddleStrIndex = 0;
	m_lNeteaseRTDataInquiryIndex = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// ��ʼ�����п��ܵĹ�Ʊ����أ�ֻ��CDataStockSymbol�ĳ�ʼ������Reset����һ�Ρ�
//
////////////////////////////////////////////////////////////////////////////////////////////////////
bool CDataStockSymbol::CreateTotalStockContainer(void) {
	CChinaStockPtr pStock = nullptr;

	for (int i = 0; i < m_vCurrentSectionStockCode.size(); i++) {
		CreateStockSection(m_vCurrentSectionStockCode.at(i));
	}
	return true;
}

void CDataStockSymbol::LoadStockSectionDB(void) {
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

bool CDataStockSymbol::UpdateStockSectionDB(void) {
	CSetStockSection setStockSection;
	CStockSectionPtr pStockSection = nullptr;

	setStockSection.m_strSort = _T("[ID]");
	setStockSection.Open();
	setStockSection.m_pDatabase->BeginTrans();
	if (setStockSection.IsEOF()) {// �ձ�
		for (int i = 0; i < 2000; i++) {
			pStockSection = m_vStockSection.at(i);
			setStockSection.AddNew();
			setStockSection.m_ID = i;
			setStockSection.m_Active = pStockSection->IsActive();
			setStockSection.m_Market = pStockSection->GetMarket();
			setStockSection.m_IndexNumber = pStockSection->GetIndexNumber();
			setStockSection.m_Comment = pStockSection->GetComment();
			setStockSection.Update();
		}
	}
	else { // ���Ѵ���
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

	return true;
}

void CDataStockSymbol::CreateStockSection(CString strFirstStockCode) {
	CString strCode = GetStockSymbol(strFirstStockCode);
	CString strStockCode, strStockSymbol, strExchange;
	CString str = _T("");
	int iCode = atoi(strCode.GetBuffer());
	int iMarket = 0;
	char buffer[10];
	CChinaStockPtr pStock = nullptr;

	if (IsShanghaiExchange(strFirstStockCode)) { // �Ϻ��г�
		iMarket = 0;
	}
	else if (IsShenzhenExchange(strFirstStockCode)) { // �����г�
		iMarket = 1000;
	}
	if (m_vStockSection.at((iCode / 1000) + iMarket)->IsBuildStockPtr()) return; // �Ѿ���֤ȯ���н�����
	// �����Ϻ���Ʊ����
	for (int i = iCode; i < (iCode + 1000); i++) {
		strExchange = GetStockExchange(strFirstStockCode);
		sprintf_s(buffer, _T("%06d"), i);
		strStockSymbol = buffer;
		strStockCode = CreateStockCode(strExchange, strStockSymbol);
		Add(strStockCode);
	}
	if (UpdateStockSection(iCode / 1000 + iMarket)) {
		SetUpdateStockSection(true);
	}
	m_vStockSection.at(iCode / 1000 + iMarket)->SetBuildStockPtr(true); // �Ѿ���֤ȯ���н�����
}

bool CDataStockSymbol::UpdateStockSection(CString strStockCode) {
	CString strCode = GetStockSymbol(strStockCode);
	int iCode = atoi(strCode.GetBuffer());
	int iMarket = 0;

	if (IsShanghaiExchange(strStockCode)) { // �Ϻ��г�
		iMarket = 0;
	}
	else if (IsShenzhenExchange(strStockCode)) { // �����г�
		iMarket = 1000;
	}
	return UpdateStockSection(iCode / 1000 + iMarket);
}

bool CDataStockSymbol::UpdateStockSection(long lIndex) {
	if (!m_vStockSection.at(lIndex)->IsActive()) {
		m_vStockSection.at(lIndex)->SetActive(true);
		return true;
	}
	return false;
}

bool CDataStockSymbol::Delete(CString strSymbol) {
	if (!IsStockSymbol(strSymbol)) return false;

	m_vStockSymbol.erase(m_vStockSymbol.begin() + m_mapStockSymbol.at(strSymbol));
	m_mapStockSymbol.erase(strSymbol);

	return true;
}

void CDataStockSymbol::Add(CString strSymbol) {
	m_mapStockSymbol[strSymbol] = m_vStockSymbol.size();
	m_vStockSymbol.push_back(strSymbol);
}

CString CDataStockSymbol::GetNextSinaStockInquiringMiddleStr(long lTotalNumber) {
	CString strReturn = _T("");
	CString strSuffix = _T(",");

	if (m_vStockSymbol.size() == 0) return _T("sh600000"); // ��û��֤ȯ�ɲ�ѯʱ������һ����Ч�ַ���
	strReturn = XferStandredToSina(m_vStockSymbol.at(m_lNextSinaStockInquiringMiddleStrIndex));  // �õ���һ����Ʊ����
	GetNextIndex(m_lNextSinaStockInquiringMiddleStrIndex);
	int iCount = 1; // ��1��ʼ��������Ϊ��һ������ǰ����Ҫ���postfix��
	while ((m_lNextSinaStockInquiringMiddleStrIndex < m_vStockSymbol.size()) && (iCount < lTotalNumber)) { // ÿ������ѯ��ΪlTotalNumber����Ʊ
		iCount++;
		strReturn += strSuffix;
		strReturn += XferStandredToSina(m_vStockSymbol.at(m_lNextSinaStockInquiringMiddleStrIndex));
		GetNextIndex(m_lNextSinaStockInquiringMiddleStrIndex);
	}
	if (m_lNextSinaStockInquiringMiddleStrIndex > 0) m_lNextSinaStockInquiringMiddleStrIndex--; // �˺�һ������ֹ���һ����Ʊ��ѯ������ʵ����Ҫ�ˣ�

	return strReturn;
}

CString CDataStockSymbol::GetNextNeteaseStockInquiringMiddleStr(long lTotalNumber) {
	CString strReturn;
	CString strPostfix = _T(",");

	if (m_vStockSymbol.size() == 0) return _T("0600000"); // ��û��֤ȯ�ɲ�ѯʱ������һ����Ч�ַ���
	strReturn = XferStandredToNetease(m_vStockSymbol.at(m_lNeteaseRTDataInquiryIndex));  // �õ���һ����Ʊ����
	GetNextIndex(m_lNeteaseRTDataInquiryIndex);
	int iCount = 1; // ��1��ʼ��������Ϊ��һ������ǰ����Ҫ���postfix��
	while ((m_lNeteaseRTDataInquiryIndex < m_vStockSymbol.size()) && (iCount < lTotalNumber)) { // ÿ������ѯ��ΪlTotalNumber����Ʊ
		iCount++;
		strReturn += strPostfix;
		strReturn += XferStandredToNetease(m_vStockSymbol.at(m_lNeteaseRTDataInquiryIndex));  // �õ���һ����Ʊ����
		GetNextIndex(m_lNeteaseRTDataInquiryIndex);
	}
	if (m_lNeteaseRTDataInquiryIndex > 0) m_lNeteaseRTDataInquiryIndex--; // �˺�һ������ֹ���һ����Ʊ��ѯ������ʵ����Ҫ�ˣ�

	return strReturn;
}

long CDataStockSymbol::GetNextIndex(long& lIndex) {
	if (++lIndex >= m_vStockSymbol.size()) {
		lIndex = 0;
	}
	return lIndex;
}