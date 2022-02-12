#include "pch.h"

#include"globedef.h"
#include"GlobeSymbolDef.h"
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

	setStockSection.Open();
	setStockSection.m_pDatabase->BeginTrans();
	while (!setStockSection.IsEOF()) {
		setStockSection.Delete();
		setStockSection.MoveNext();
	}
	setStockSection.m_pDatabase->CommitTrans();
	setStockSection.Close();

	CStockSectionPtr pStockSection = nullptr;

	setStockSection.Open();
	setStockSection.m_pDatabase->BeginTrans();
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
	setStockSection.m_pDatabase->CommitTrans();
	setStockSection.Close();

	m_fUpdateStockSection = false;

	return true;
}

bool CDataStockSymbol::DeleteStockSectionDB(void) {
	CDatabase database;

	if (gl_fTestMode) {
		ASSERT(0); // ���ڴ���ʵ�����ݿ⣬�ʲ�������Դ˺���
		exit(1); //�˳�ϵͳ
	}

	database.Open(_T("ChinaMarket"), FALSE, FALSE, _T("ODBC;UID=hxguo;PASSWORD=hxguo;charset=utf8mb4"));
	database.BeginTrans();
	database.ExecuteSQL(_T("TRUNCATE `chinamarket`.`stock_code_section`;"));
	database.CommitTrans();
	database.Close();

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
	long lEndPosition = m_vStockSymbol.size();
	CString strSuffix = _T(",");
	static long s_lIndex = 0;

	if (0 == lEndPosition) return _T("sh600000"); // ��û��֤ȯ�ɲ�ѯʱ������һ����Ч�ַ���
	strReturn = XferStandredToSina(m_vStockSymbol.at(s_lIndex));  // �õ���һ����Ʊ����
	IncreaseIndex(s_lIndex, lEndPosition);
	int iCount = 1; // ��1��ʼ��������Ϊ��һ������ǰ����Ҫ���postfix��
	while ((s_lIndex < lEndPosition) && (iCount < lTotalNumber)) { // ÿ������ѯ��ΪlTotalNumber����Ʊ
		iCount++;
		strReturn += strSuffix;
		strReturn += XferStandredToSina(m_vStockSymbol.at(s_lIndex));
		IncreaseIndex(s_lIndex, lEndPosition);
	}
	if (s_lIndex > 0) s_lIndex--; // �˺�һ������ֹ���һ����Ʊ��ѯ������ʵ����Ҫ�ˣ�

	return strReturn;
}

CString CDataStockSymbol::GetNextNeteaseStockInquiringMiddleStr(long lTotalNumber) {
	CString strReturn;
	long lEndPosition = m_vStockSymbol.size();
	CString strPostfix = _T(",");
	static long s_lNeteaseRTDataInquiryIndex = 0;

	if (0 == lEndPosition) return _T("0600000"); // ��û��֤ȯ�ɲ�ѯʱ������һ����Ч�ַ���
	strReturn = XferStandredToNetease(m_vStockSymbol.at(s_lNeteaseRTDataInquiryIndex));  // �õ���һ����Ʊ����
	IncreaseIndex(s_lNeteaseRTDataInquiryIndex, lEndPosition);
	int iCount = 1; // ��1��ʼ��������Ϊ��һ������ǰ����Ҫ���postfix��
	while ((s_lNeteaseRTDataInquiryIndex < lEndPosition) && (iCount < lTotalNumber)) { // ÿ������ѯ��ΪlTotalNumber����Ʊ
		iCount++;
		strReturn += strPostfix;
		strReturn += XferStandredToNetease(m_vStockSymbol.at(s_lNeteaseRTDataInquiryIndex));  // �õ���һ����Ʊ����
		IncreaseIndex(s_lNeteaseRTDataInquiryIndex, lEndPosition);
	}
	if (s_lNeteaseRTDataInquiryIndex > 0) s_lNeteaseRTDataInquiryIndex--; // �˺�һ������ֹ���һ����Ʊ��ѯ������ʵ����Ҫ�ˣ�

	return strReturn;
}

long CDataStockSymbol::IncreaseIndex(long& lIndex, long lEndPosition) {
	if (++lIndex >= lEndPosition) {
		lIndex = 0;
	}
	return lIndex;
}