#include "pch.h"

#include"StockCodeConverter.h"
#include "DataStockSymbol.h"

CDataStockSymbol::CDataStockSymbol() {
	m_vStockSection.resize(2000); // 沪深各1000个段。
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
	// 预设股票代码集如下
	m_vCurrentSectionStockCode.push_back(_T("000000.SS")); // 上海指数
	m_vCurrentSectionStockCode.push_back(_T("600000.SS")); // 上海主板
	m_vCurrentSectionStockCode.push_back(_T("601000.SS")); // 上海主板
	m_vCurrentSectionStockCode.push_back(_T("603000.SS")); // 上海三板
	m_vCurrentSectionStockCode.push_back(_T("688000.SS")); // 上海科创板
	m_vCurrentSectionStockCode.push_back(_T("900000.SS")); // 上海B股
	m_vCurrentSectionStockCode.push_back(_T("000000.SZ")); // 深圳主板
	m_vCurrentSectionStockCode.push_back(_T("001000.SZ")); // 深圳主板
	m_vCurrentSectionStockCode.push_back(_T("002000.SZ")); // 深圳中小板
	m_vCurrentSectionStockCode.push_back(_T("003000.SZ")); // 深圳中小板
	m_vCurrentSectionStockCode.push_back(_T("004000.SZ")); // 深圳中小板
	m_vCurrentSectionStockCode.push_back(_T("200000.SZ")); // 深圳B股
	m_vCurrentSectionStockCode.push_back(_T("300000.SZ")); // 深圳创业板
	m_vCurrentSectionStockCode.push_back(_T("399000.SZ")); // 深圳指数
	// 从股票代码集数据库中读入其他股票集

	//重置StockSection
	for (int i = 0; i < m_vStockSection.size(); i++) {
		m_vStockSection.at(i)->SetBuildStockPtr(false);
	}

	LoadStockSectionDB();

	// 生成股票代码池
	CreateTotalStockContainer();

	m_lNextSinaStockInquiringMiddleStrIndex = 0;
	m_lNeteaseRTDataInquiryIndex = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// 初始化所有可能的股票代码池，只被CDataStockSymbol的初始化函数Reset调用一次。
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
	if (setStockSection.IsEOF()) {// 空表
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
	else { // 表已存在
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

	if (IsShanghaiExchange(strFirstStockCode)) { // 上海市场
		iMarket = 0;
	}
	else if (IsShenzhenExchange(strFirstStockCode)) { // 深圳市场
		iMarket = 1000;
	}
	if (m_vStockSection.at((iCode / 1000) + iMarket)->IsBuildStockPtr()) return; // 已经在证券池中建立了
	// 生成上海股票代码
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
	m_vStockSection.at(iCode / 1000 + iMarket)->SetBuildStockPtr(true); // 已经在证券池中建立了
}

bool CDataStockSymbol::UpdateStockSection(CString strStockCode) {
	CString strCode = GetStockSymbol(strStockCode);
	int iCode = atoi(strCode.GetBuffer());
	int iMarket = 0;

	if (IsShanghaiExchange(strStockCode)) { // 上海市场
		iMarket = 0;
	}
	else if (IsShenzhenExchange(strStockCode)) { // 深圳市场
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

	if (m_vStockSymbol.size() == 0) return _T("sh600000"); // 当没有证券可查询时，返回一个有效字符串
	strReturn = XferStandredToSina(m_vStockSymbol.at(m_lNextSinaStockInquiringMiddleStrIndex));  // 得到第一个股票代码
	GetNextIndex(m_lNextSinaStockInquiringMiddleStrIndex);
	int iCount = 1; // 从1开始计数，因为第一个数据前不需要添加postfix。
	while ((m_lNextSinaStockInquiringMiddleStrIndex < m_vStockSymbol.size()) && (iCount < lTotalNumber)) { // 每次最大查询量为lTotalNumber个股票
		iCount++;
		strReturn += strSuffix;
		strReturn += XferStandredToSina(m_vStockSymbol.at(m_lNextSinaStockInquiringMiddleStrIndex));
		GetNextIndex(m_lNextSinaStockInquiringMiddleStrIndex);
	}
	if (m_lNextSinaStockInquiringMiddleStrIndex > 0) m_lNextSinaStockInquiringMiddleStrIndex--; // 退后一步，防止最后一个股票查询错误（其实不必要了）

	return strReturn;
}

CString CDataStockSymbol::GetNextNeteaseStockInquiringMiddleStr(long lTotalNumber) {
	CString strReturn;
	CString strPostfix = _T(",");

	if (m_vStockSymbol.size() == 0) return _T("0600000"); // 当没有证券可查询时，返回一个有效字符串
	strReturn = XferStandredToNetease(m_vStockSymbol.at(m_lNeteaseRTDataInquiryIndex));  // 得到第一个股票代码
	GetNextIndex(m_lNeteaseRTDataInquiryIndex);
	int iCount = 1; // 从1开始计数，因为第一个数据前不需要添加postfix。
	while ((m_lNeteaseRTDataInquiryIndex < m_vStockSymbol.size()) && (iCount < lTotalNumber)) { // 每次最大查询量为lTotalNumber个股票
		iCount++;
		strReturn += strPostfix;
		strReturn += XferStandredToNetease(m_vStockSymbol.at(m_lNeteaseRTDataInquiryIndex));  // 得到第一个股票代码
		GetNextIndex(m_lNeteaseRTDataInquiryIndex);
	}
	if (m_lNeteaseRTDataInquiryIndex > 0) m_lNeteaseRTDataInquiryIndex--; // 退后一步，防止最后一个股票查询错误（其实不必要了）

	return strReturn;
}

long CDataStockSymbol::GetNextIndex(long& lIndex) {
	if (++lIndex >= m_vStockSymbol.size()) {
		lIndex = 0;
	}
	return lIndex;
}