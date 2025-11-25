#include "pch.h"

#include"ChinaStockCodeConverter.h"
#include "ContainerStockSymbol.h"

#include"SetStockSection.h"

CContainerStockSymbol::CContainerStockSymbol() {
	CContainerStockSymbol::Reset();
}

void CContainerStockSymbol::Reset() {
	CVirtualContainer::Reset();

	m_vStockSymbol.resize(0);
	m_mapStockSymbol.clear();
	m_vCurrentSectionStockCode.resize(0);
	// 预设股票代码集如下
	m_vCurrentSectionStockCode.push_back("000000.SS"); // 上海指数
	m_vCurrentSectionStockCode.push_back("600000.SS"); // 上海主板
	m_vCurrentSectionStockCode.push_back("601000.SS"); // 上海主板
	m_vCurrentSectionStockCode.push_back("603000.SS"); // 上海三板
	m_vCurrentSectionStockCode.push_back("688000.SS"); // 上海科创板
	m_vCurrentSectionStockCode.push_back("900000.SS"); // 上海B股
	m_vCurrentSectionStockCode.push_back("000000.SZ"); // 深圳主板
	m_vCurrentSectionStockCode.push_back("001000.SZ"); // 深圳主板
	m_vCurrentSectionStockCode.push_back("002000.SZ"); // 深圳中小板
	m_vCurrentSectionStockCode.push_back("003000.SZ"); // 深圳中小板
	m_vCurrentSectionStockCode.push_back("004000.SZ"); // 深圳中小板
	m_vCurrentSectionStockCode.push_back("200000.SZ"); // 深圳B股
	m_vCurrentSectionStockCode.push_back("300000.SZ"); // 深圳创业板
	m_vCurrentSectionStockCode.push_back("399000.SZ"); // 深圳指数
	// 从股票代码集数据库中读入其他股票集

	//重置StockSection
	m_vStockSection.resize(0);
	m_vStockSection.resize(2000); // 沪深各1000个段。
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

	// 生成股票代码池
	CreateTotalStockContainer();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// 初始化所有可能的股票代码池，只被CContainerStockSymbol的初始化函数Reset调用一次。
//
////////////////////////////////////////////////////////////////////////////////////////////////////
bool CContainerStockSymbol::CreateTotalStockContainer() {
	CChinaStockPtr pStock = nullptr;

	for (int i = 0; i < m_vCurrentSectionStockCode.size(); i++) {
		CreateStockSection(m_vCurrentSectionStockCode.at(i));
	}
	return true;
}
string CContainerStockSymbol::GetItemSymbol(size_t lIndex) {
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
			m_vStockSection.at(setStockSection.m_IndexNumber)->SetComment(ToUTF8(setStockSection.m_Comment));
		}
		setStockSection.MoveNext();
	}
	setStockSection.Close();
}

void CContainerStockSymbol::UpdateStockSectionDB() {
	CSetStockSection setStockSection;

	setStockSection.m_strSort = "[ID]";
	setStockSection.Open();
	setStockSection.m_pDatabase->BeginTrans();
	if (setStockSection.IsEOF()) {// 空表
		for (int i = 0; i < 2000; i++) {
			const CStockSectionPtr pStockSection = m_vStockSection.at(i);
			setStockSection.AddNew();
			setStockSection.m_ID = i;
			setStockSection.m_Active = pStockSection->IsActive();
			setStockSection.m_Market = pStockSection->GetMarket();
			setStockSection.m_IndexNumber = pStockSection->GetIndexNumber();
			setStockSection.m_Comment = pStockSection->GetComment().c_str();
			setStockSection.Update();
		}
	}
	else {// 表已存在
		while (!setStockSection.IsEOF()) {
			if (setStockSection.m_Active != m_vStockSection.at(setStockSection.m_ID)->IsActive()) {
				setStockSection.Edit();
				setStockSection.m_Active = m_vStockSection.at(setStockSection.m_ID)->IsActive();
				setStockSection.m_Market = m_vStockSection.at(setStockSection.m_ID)->GetMarket();
				setStockSection.m_IndexNumber = m_vStockSection.at(setStockSection.m_ID)->GetIndexNumber();
				setStockSection.m_Comment = m_vStockSection.at(setStockSection.m_ID)->GetComment().c_str();
				setStockSection.Update();
			}
			setStockSection.MoveNext();
		}
	}
	setStockSection.m_pDatabase->CommitTrans();
	setStockSection.Close();

	m_fUpdateStockSection = false;
}

void CContainerStockSymbol::CreateStockSection(const string& strFirstStockCode) {
	string strCode = GetStockSymbol(strFirstStockCode);
	const int iCode = atoi(strCode.c_str());
	int iMarket = 0;
	CChinaStockPtr pStock = nullptr;

	if (IsShanghaiExchange(strFirstStockCode)) {// 上海市场
		iMarket = 0;
	}
	else if (IsShenzhenExchange(strFirstStockCode)) {// 深圳市场
		iMarket = 1000;
	}
	if (m_vStockSection.at((iCode / 1000) + iMarket)->IsBuildStockPtr()) return; // 已经在证券池中建立了
	// 生成上海股票代码
	for (int i = iCode; i < (iCode + 1000); i++) {
		const string strExchange = GetStockExchange(strFirstStockCode);
		string sStockSymbol = fmt::format("{:06d}", i);
		const string strStockCode = CreateStockCode(strExchange, sStockSymbol);
		Add(strStockCode);
	}
	if (UpdateStockSection(iCode / 1000 + iMarket)) {
		SetUpdateStockSection(true);
	}
	m_vStockSection.at(iCode / 1000 + iMarket)->SetBuildStockPtr(true); // 已经在证券池中建立了
}

bool CContainerStockSymbol::UpdateStockSection(const string& strStockCode) const {
	string strCode = GetStockSymbol(strStockCode);
	char* pChar;
	const int iCode = strtol(strCode.c_str(), &pChar, 10);
	int iMarket = 0;

	if (IsShanghaiExchange(strStockCode)) {// 上海市场
		iMarket = 0;
	}
	else if (IsShenzhenExchange(strStockCode)) {// 深圳市场
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

bool CContainerStockSymbol::Delete(const string& strSymbol) {
	if (!IsStockSymbol(strSymbol)) return false;

	m_vStockSymbol.erase(m_vStockSymbol.begin() + m_mapStockSymbol.at(strSymbol));
	m_mapStockSymbol.erase(strSymbol);

	return true;
}

void CContainerStockSymbol::Add(const string& strSymbol) {
	m_mapStockSymbol[strSymbol] = m_vStockSymbol.size();
	m_vStockSymbol.push_back(strSymbol);
}
