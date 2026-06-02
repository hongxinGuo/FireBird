#include "pch.h"

#include"ChinaStockCodeConverter.h"
#include "ContainerStockSymbol.h"

#include "dataBaseConnector.h"

CContainerStockSymbol::CContainerStockSymbol() {
	m_vStockSymbol.resize(0);
	m_mapStockSymbol.clear();
	m_vCurrentSectionStockCode.resize(0);
	// 预设股票代码集如下
	m_vCurrentSectionStockCode.emplace_back("000000.SS"); // 上海指数
	m_vCurrentSectionStockCode.emplace_back("600000.SS"); // 上海主板
	m_vCurrentSectionStockCode.emplace_back("601000.SS"); // 上海主板
	m_vCurrentSectionStockCode.emplace_back("603000.SS"); // 上海三板
	m_vCurrentSectionStockCode.emplace_back("688000.SS"); // 上海科创板
	m_vCurrentSectionStockCode.emplace_back("900000.SS"); // 上海B股
	m_vCurrentSectionStockCode.emplace_back("000000.SZ"); // 深圳主板
	m_vCurrentSectionStockCode.emplace_back("001000.SZ"); // 深圳主板
	m_vCurrentSectionStockCode.emplace_back("002000.SZ"); // 深圳中小板
	m_vCurrentSectionStockCode.emplace_back("003000.SZ"); // 深圳中小板
	m_vCurrentSectionStockCode.emplace_back("004000.SZ"); // 深圳中小板
	m_vCurrentSectionStockCode.emplace_back("200000.SZ"); // 深圳B股
	m_vCurrentSectionStockCode.emplace_back("300000.SZ"); // 深圳创业板
	m_vCurrentSectionStockCode.emplace_back("399000.SZ"); // 深圳指数
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
	for (const auto& pStockSection : m_vStockSection) {
		pStockSection->SetBuildStockPtr(false);
	}
	m_fUpdateStockSection = false;
}

void CContainerStockSymbol::Reset() {
	CVirtualContainer::Reset();

	m_vStockSymbol.resize(0);
	m_mapStockSymbol.clear();
	m_vCurrentSectionStockCode.resize(0);
	// 预设股票代码集如下
	m_vCurrentSectionStockCode.emplace_back("000000.SS"); // 上海指数
	m_vCurrentSectionStockCode.emplace_back("600000.SS"); // 上海主板
	m_vCurrentSectionStockCode.emplace_back("601000.SS"); // 上海主板
	m_vCurrentSectionStockCode.emplace_back("603000.SS"); // 上海三板
	m_vCurrentSectionStockCode.emplace_back("688000.SS"); // 上海科创板
	m_vCurrentSectionStockCode.emplace_back("900000.SS"); // 上海B股
	m_vCurrentSectionStockCode.emplace_back("000000.SZ"); // 深圳主板
	m_vCurrentSectionStockCode.emplace_back("001000.SZ"); // 深圳主板
	m_vCurrentSectionStockCode.emplace_back("002000.SZ"); // 深圳中小板
	m_vCurrentSectionStockCode.emplace_back("003000.SZ"); // 深圳中小板
	m_vCurrentSectionStockCode.emplace_back("004000.SZ"); // 深圳中小板
	m_vCurrentSectionStockCode.emplace_back("200000.SZ"); // 深圳B股
	m_vCurrentSectionStockCode.emplace_back("300000.SZ"); // 深圳创业板
	m_vCurrentSectionStockCode.emplace_back("399000.SZ"); // 深圳指数
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
	for (const auto& pStockSection : m_vStockSection) {
		pStockSection->SetBuildStockPtr(false);
	}
	m_fUpdateStockSection = false;

	// 生成股票代码池
	CreateTotalStockContainer();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// 初始化所有可能的股票代码池，只被CContainerStockSymbol的初始化函数Reset调用一次。
//
////////////////////////////////////////////////////////////////////////////////////////////////////
bool CContainerStockSymbol::CreateTotalStockContainer() {
	LoadStockSectionDB();

	for (const auto& str : m_vCurrentSectionStockCode) {
		CreateStockSection(str);
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

void CContainerStockSymbol::LoadStockSectionDB() {
	using namespace StockMarket;
	const auto& t = ChinaStockSymbolSection{};

	auto db = gl_dbStockMarket.get();
	auto tx = sqlpp::start_transaction(db);

	auto result = db(sqlpp::select(all_of(t)).from(t).unconditionally());
	for (const auto& row : result) {
		if (!m_vStockSection.at(row.IndexNumber.value())->IsActive()) {
			m_vStockSection.at(row.IndexNumber.value())->SetActive(row.Active.value());
			m_vStockSection.at(row.IndexNumber.value())->SetMarket(row.Market);
			m_vStockSection.at(row.IndexNumber.value())->SetIndexNumber(row.IndexNumber.value());
			m_vStockSection.at(row.IndexNumber.value())->SetComment(row.Comment.value());
		}
	}
	tx.commit();

	m_fDBLoaded = true;
}

void CContainerStockSymbol::UpdateStockSectionDB() {
	using namespace StockMarket;
	const auto& t = ChinaStockSymbolSection{};

	auto db = gl_dbStockMarket.get();
	auto tx = sqlpp::start_transaction(db);

	auto result = db(sqlpp::select(all_of(t)).from(t).unconditionally());
	auto multi_insert = insert_into(t).columns(t.ID, t.Active, t.Market, t.IndexNumber, t.Comment);
	int rows = result.size();
	if (rows == 0) {
		for (int i = 0; i < 2000; i++) {
			const CStockSectionPtr pStockSection = m_vStockSection.at(i);
			multi_insert.values.add(
				t.ID = i,
				t.Active = pStockSection->IsActive() ? 1 : 0,
				t.Market = static_cast<int>(pStockSection->GetMarket()),
				t.IndexNumber = static_cast<int>(pStockSection->GetIndexNumber()),
				t.Comment = pStockSection->GetComment()
			);
		}
		db(multi_insert);
	}
	else {// 表已存在
		for (int i = 0; i < 2000; i++) {
			const CStockSectionPtr pStockSection = m_vStockSection.at(i);
			db(update(t).set(
				t.ID = i,
				t.Active = pStockSection->IsActive() ? 1 : 0,
				t.Market = pStockSection->GetMarket(),
				t.IndexNumber = pStockSection->GetIndexNumber(),
				t.Comment = pStockSection->GetComment()
			).where(t.ID == i));
		}
	}
	tx.commit();

	m_fUpdateStockSection = false;
}

void CContainerStockSymbol::CreateStockSection(const string& strFirstStockCode) {
	const string strCode = GetStockSymbol(strFirstStockCode);
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
