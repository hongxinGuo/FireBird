//////////////////////////////////////////////////////////////////////////////////////////
//
// CContainerFinnhubStock类操作worldMarket数据库中的finnhub_stock_profile表。
//
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "pch.h"

#include"WorldMarket.h"
#include"SetFinnhubStock.h"
#include "ContainerFinnhubStock.h"

#include <sqlpp11/transaction.h>

#include "CharSetTransfer.h"
#include "dataBaseConnector.h"
#include "jsonParse.h"

CContainerFinnhubStock::CContainerFinnhubStock() {
	CContainerFinnhubStock::Reset();
}

void CContainerFinnhubStock::Reset() {
	CContainerVirtualStock::Reset();
}

void CContainerFinnhubStock::ResetEPSSurprise() {
	for (size_t l = 0; l < m_vStock.size(); l++) {
		const CFinnhubStockPtr pStock = GetItem(l);
		pStock->SetLastEPSSurpriseUpdateDate(19800101);
		pStock->m_fUpdateEPSSurprise = true;
	}
}

void CContainerFinnhubStock::ResetPeer() {
	for (size_t l = 0; l < m_vStock.size(); l++) {
		const CFinnhubStockPtr pStock = GetItem(l);
		if (pStock->GetPeerUpdateDate() != 19800101) {
			pStock->SetPeerUpdateDate(19800101);
			pStock->SetUpdatePeer(true);
			pStock->SetUpdateProfileDB(true);
		}
	}
}

void CContainerFinnhubStock::ResetBasicFinancial() {
	for (size_t l = 0; l < m_vStock.size(); l++) {
		const CFinnhubStockPtr pStock = GetItem(l);
		if (pStock->GetBasicFinancialUpdateDate() != 19800101) {
			pStock->SetBasicFinancialUpdateDate(19800101);
			pStock->SetUpdateBasicFinancial(true);
			pStock->SetUpdateProfileDB(true);
		}
	}
}

void CContainerFinnhubStock::ResetDayLine() {
	for (size_t l = 0; l < m_vStock.size(); l++) {
		const CFinnhubStockPtr pStock = GetItem(l);
		pStock->SetIPOStatus(_STOCK_NOT_CHECKED_);
		pStock->SetDayLineStartDate(29900101);
		pStock->SetDayLineEndDate(19800101);
		pStock->SetUpdateDayLine(true);
		pStock->SetUpdateProfileDB(true);
	}
}

bool CContainerFinnhubStock::LoadProfileDB2() {
	CSetFinnhubStock setFinnhubStock;
	CFinnhubStockPtr pFinnhubStock = nullptr;
	long lMaxSymbolLength = 0;

	Reset();
	setFinnhubStock.m_strSort = "[Symbol]";
	setFinnhubStock.Open();
	setFinnhubStock.m_pDatabase->BeginTrans();
	while (!setFinnhubStock.IsEOF()) {
		pFinnhubStock = make_shared<CFinnhubStock>();
		pFinnhubStock->Load(setFinnhubStock);
		if (!IsSymbol(pFinnhubStock->GetSymbol())) {
			pFinnhubStock->CheckUpdateStatus(gl_pWorldMarket->GetMarketDate());
			Add(pFinnhubStock);
			ASSERT(pFinnhubStock->GetSymbol().length() < 12); // 目前WorldMarket数据库的股票代码长度限制为12个字符
		}
		else {
			setFinnhubStock.Delete(); // 删除此重复代码
		}
		setFinnhubStock.MoveNext();
	}
	setFinnhubStock.m_pDatabase->CommitTrans();
	setFinnhubStock.Close();
	Sort();

	return true;
}

bool CContainerFinnhubStock::LoadProfileDB() {
	bool fDeleteDuplicatedSymbol = false;
	using namespace StockMarket;
	const auto& t = FinnhubStockProfile{};

	Reset();
	auto db = gl_dbStockMarket.get();
	auto tx = start_transaction(db);
	auto result = db(select(all_of(t)).from(t).unconditionally().order_by(t.Symbol.asc()));
	auto rowCount = result.size();
	Reserve(rowCount + 10); // 预留一些空间，避免后续添加新股票时频繁扩容
	CFinnhubStockPtr pFinnhubStock = nullptr;

	for (const auto& row : result) {
		pFinnhubStock = make_shared<CFinnhubStock>();
		pFinnhubStock->SetSymbol(row.Symbol);
		pFinnhubStock->SetExchangeCode(row.ExchangeCode);
		pFinnhubStock->SetDescription(row.Description);
		pFinnhubStock->SetDisplaySymbol(row.DisplaySymbol);
		pFinnhubStock->SetType(row.Type);
		pFinnhubStock->SetMic(row.Mic);
		pFinnhubStock->SetFigi(row.Figi);
		pFinnhubStock->SetCurrency(row.Currency);
		pFinnhubStock->SetAddress(row.Address);
		pFinnhubStock->SetCity(row.City);
		pFinnhubStock->SetCountry(row.Country);
		pFinnhubStock->SetCusip(row.cusip);
		pFinnhubStock->SetSedol(row.sedol);
		pFinnhubStock->SetEmployeeTotal(row.EmployeeTotal);
		pFinnhubStock->SetGgroup(row.ggroup);
		pFinnhubStock->SetGind(row.gind);
		pFinnhubStock->SetGsector(row.gsector);
		pFinnhubStock->SetGsubind(row.gsubind);
		pFinnhubStock->SetIPODate(row.IPODate);
		pFinnhubStock->SetIsin((row.isin));
		pFinnhubStock->SetMarketCapitalization(row.MarketCapitalization);
		pFinnhubStock->SetNaics(row.naics);
		pFinnhubStock->SetNaicsNationalIndustry(row.naicsNationalIndustry);
		pFinnhubStock->SetNaicsSector(row.naicsSector);
		pFinnhubStock->SetNaicsSubsector(row.naicsSubsector);
		pFinnhubStock->SetName(row.Name);
		pFinnhubStock->SetPhone(row.Phone);
		pFinnhubStock->SetShareOutstanding(row.ShareOutstanding);
		pFinnhubStock->SetState(row.state);
		pFinnhubStock->SetTicker(row.Ticker);
		pFinnhubStock->SetWebURL(row.WebURL);
		pFinnhubStock->SetLogo(row.Logo);
		pFinnhubStock->SetFinnhubIndustry(row.FinnhubIndustry);
		string str = row.Peer;
		if (str.length() > 2) {
			nlohmannJson js;
			CreateJsonWithNlohmann(js, row.Peer);
			pFinnhubStock->SetPeer(js);
		}
		pFinnhubStock->SetIPOStatus(row.IPOStatus);
		pFinnhubStock->LoadUpdateDate(row.UpdateDate);
		if (!IsSymbol(pFinnhubStock->GetSymbol())) {
			pFinnhubStock->CheckUpdateStatus(gl_pWorldMarket->GetMarketDate());
			Add(pFinnhubStock);
			ASSERT(pFinnhubStock->GetSymbol().length() < 12);// 目前WorldMarket数据库的股票代码长度限制为12个字符
		}
		else {
			fDeleteDuplicatedSymbol = true;
		}
	}
	tx.commit();

	if (fDeleteDuplicatedSymbol) {
		DeleteDuplicatedSymbolFromDB();// 删除重复的股票代码
	}

	Sort();

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
///
/// 删除数据库中重复的股票代码。由于之前的代码逻辑问题，可能会在数据库中存储多个相同代码的股票简介，此函数用来删除这些重复的股票代码。
/// Note: MySQL数据库不允许在同一张表中使用DELETE语句删除重复的记录，因此需要使用JOIN的方式来删除重复的记录。
/// 具体来说，使用INNER JOIN将表自身连接起来，找到重复的记录，并删除其中一个记录。
///
///
//////////////////////////////////////////////////////////////////////////////////////////////////
void CContainerFinnhubStock::DeleteDuplicatedSymbolFromDB() {
	auto db = gl_dbStockMarket.get();
	// Use execute(string) to run raw SQL text (operator() requires a sqlpp statement)
	db.execute("DELETE t1 FROM finnhub_stock_profile t1 INNER JOIN finnhub_stock_profile t2 ON t1.Symbol = t2.Symbol AND t1.ID > t2.ID");
	db.execute("COMMIT");
}

void CContainerFinnhubStock::UpdateProfileDB() {
	ASSERT(IsUpdateProfileDB());

	using namespace StockMarket;
	const auto& t = FinnhubStockProfile{};
	auto db = gl_dbStockMarket.get();
	auto tx = start_transaction(db);

	for (size_t l = 0; l < m_vStock.size(); l++) {
		const CFinnhubStockPtr pStock = GetItem(l);
		ASSERT(pStock != nullptr);
		if (pStock->IsUpdateProfileDB()) {
			pStock->UpdateJsonUpdateDate();
			nlohmannJson jsPeer = pStock->GetPeer();
			string sPeer = pStock->GetJsonPeer().dump();
			string sUpdateDate = pStock->GetJsonUpdateDate().dump();
			if (pStock->IsTodayNewStock()) {// 新代码，插入。
				db(insert_into(t).set(
					t.Symbol = pStock->GetSymbol(),
					t.ExchangeCode = pStock->GetExchangeCode(),
					t.Description = pStock->GetDescription(),
					t.DisplaySymbol = pStock->GetDisplaySymbol(),
					t.Type = pStock->GetType(),
					t.Mic = pStock->GetMic(),
					t.Figi = pStock->GetFigi(),
					t.Currency = pStock->GetCurrency(),
					t.Address = pStock->GetAddress(),
					t.City = pStock->GetCity(),
					t.Country = pStock->GetCountry(),
					t.cusip = pStock->GetCusip(),
					t.sedol = pStock->GetSedol(),
					t.EmployeeTotal = pStock->GetEmployeeTotal(),
					t.ggroup = pStock->GetGgroup(),
					t.gind = pStock->GetGind(),
					t.gsector = pStock->GetGsector(),
					t.gsubind = pStock->GetGsubind(),
					t.IPODate = pStock->GetIPODate(),
					t.isin = pStock->GetIsin(),
					t.MarketCapitalization = pStock->GetMarketCapitalization(),
					t.naics = pStock->GetNaics(),
					t.naicsNationalIndustry = pStock->GetNaicsNationalIndustry(),
					t.naicsSector = pStock->GetNaicsSector(),
					t.naicsSubsector = pStock->GetNaicsSubsector(),
					t.Name = pStock->GetName(),
					t.Phone = pStock->GetPhone(),
					t.ShareOutstanding = pStock->GetShareOutstanding(),
					t.state = pStock->GetState(),
					t.Ticker = pStock->GetTicker(),
					t.WebURL = pStock->GetWebURL(),
					t.Logo = pStock->GetLogo(),
					t.FinnhubIndustry = pStock->GetFinnhubIndustry(),
					t.Peer = sPeer,
					t.IPOStatus = pStock->GetIPOStatus(),
					t.UpdateDate = sUpdateDate
				));
				pStock->SetTodayNewStock(false);
			}
			else { // 如果是原有的代码，则更新
				db(update(t).set(
					t.Symbol = pStock->GetSymbol(),
					t.ExchangeCode = pStock->GetExchangeCode(),
					t.Description = pStock->GetDescription(),
					t.DisplaySymbol = pStock->GetDisplaySymbol(),
					t.Type = pStock->GetType(),
					t.Mic = pStock->GetMic(),
					t.Figi = pStock->GetFigi(),
					t.Currency = pStock->GetCurrency(),
					t.Address = pStock->GetAddress(),
					t.City = pStock->GetCity(),
					t.Country = pStock->GetCountry(),
					t.cusip = pStock->GetCusip(),
					t.sedol = pStock->GetSedol(),
					t.EmployeeTotal = pStock->GetEmployeeTotal(),
					t.ggroup = pStock->GetGgroup(),
					t.gind = pStock->GetGind(),
					t.gsector = pStock->GetGsector(),
					t.gsubind = pStock->GetGsubind(),
					t.IPODate = pStock->GetIPODate(),
					t.isin = pStock->GetIsin(),
					t.MarketCapitalization = pStock->GetMarketCapitalization(),
					t.naics = pStock->GetNaics(),
					t.naicsNationalIndustry = pStock->GetNaicsNationalIndustry(),
					t.naicsSector = pStock->GetNaicsSector(),
					t.naicsSubsector = pStock->GetNaicsSubsector(),
					t.Name = pStock->GetName(),
					t.Phone = pStock->GetPhone(),
					t.ShareOutstanding = pStock->GetShareOutstanding(),
					t.state = pStock->GetState(),
					t.Ticker = pStock->GetTicker(),
					t.WebURL = pStock->GetWebURL(),
					t.Logo = pStock->GetLogo(),
					t.FinnhubIndustry = pStock->GetFinnhubIndustry(),
					t.Peer = sPeer,
					t.IPOStatus = pStock->GetIPOStatus(),
					t.UpdateDate = sUpdateDate
				).where(t.Symbol == pStock->GetSymbol()));
			}
			pStock->SetUpdateProfileDB(false);
		}
	}
	tx.commit();
}

/////////////////////////////////////////////////////////////////////////////////////////////
//
// 更新基本金融信息
//
// Note 更新300条信息大致需要45秒。
//
//
////////////////////////////////////////////////////////////////////////////////////////////
bool CContainerFinnhubStock::UpdateBasicFinancialDB() {
	static bool s_fInProcess = false;
	vector<CFinnhubStockPtr> vStock{};

	if (s_fInProcess) {
		gl_systemMessage.PushErrorMessage("UpdateBasicFinancialDB任务用时超过五分钟");
		return false;
	}
	s_fInProcess = true;

	vStock.clear();
	for (size_t l = 0; l < m_vStock.size(); l++) {
		const CFinnhubStockPtr pStock = GetItem(l);
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
// 这个函数比较占用CPU的计算能力，故而当使用较慢的CPU时，不使用工作线程做并行处理
//
// 发现使用并行处理方式时，数据库偶尔会出现故障，估计是MySQL数据库的同步问题，目前不容易找到问题之所在。
// 决定还是使用串行方式，不再生成线程。--20221101
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////
void CContainerFinnhubStock::UpdateBasicFinancialQuarterDB(const vector<CFinnhubStockPtr>& vStock) {
	for (const auto& pStock : vStock) {
		if (gl_systemConfiguration.IsExitingSystem()) break;
		pStock->AppendBasicFinancialQuarter();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
// 这个函数比较占用CPU的计算能力，故而当使用较慢的CPU时，不使用工作线程做并行处理
//
// 发现使用并行处理方式时，数据库偶尔会出现故障，估计是MySQL数据库的同步问题，目前不容易找到问题之所在。
// 决定还是使用串行方式，不再生成线程。--20221101
//
//////////////////////////////////////////////////////////////////////////////////////////////////
void CContainerFinnhubStock::UpdateBasicFinancialAnnualDB(const vector<CFinnhubStockPtr>& vStock) {
	for (const auto& pStock : vStock) {
		if (gl_systemConfiguration.IsExitingSystem()) break;
		pStock->AppendBasicFinancialAnnual();
	}
}

void CContainerFinnhubStock::UpdateBasicFinancialMetricDB(const vector<CFinnhubStockPtr>& vStock) {
	CSetFinnhubStockBasicFinancialMetric setBasicFinancialMetric;
	const auto iBasicFinancialNeedUpdate = vStock.size();
	size_t iCurrentUpdated = 0;

	setBasicFinancialMetric.m_strSort = "[Symbol]";
	setBasicFinancialMetric.Open();
	setBasicFinancialMetric.m_pDatabase->BeginTrans();
	//更新原有的基本财务信息
	while (iCurrentUpdated < iBasicFinancialNeedUpdate) {
		if (setBasicFinancialMetric.IsEOF()) break;
		if (IsSymbol(T2Utf8(setBasicFinancialMetric.m_symbol))) {
			CFinnhubStockPtr pStockNeedUpdate = GetItem(T2Utf8(setBasicFinancialMetric.m_symbol));
			if (vStock.end() != std::ranges::find(vStock.begin(), vStock.end(), pStockNeedUpdate)) {
				iCurrentUpdated++;
				pStockNeedUpdate->UpdateBasicFinancialMetric(setBasicFinancialMetric);
				pStockNeedUpdate->SetUpdateBasicFinancialDB(false);
			}
		}
		else {
			setBasicFinancialMetric.Delete(); //Note 自动删除代码已不存在的数据。
		}
		setBasicFinancialMetric.MoveNext();
	}
	// 添加新的基本财务数据
	if (iCurrentUpdated < iBasicFinancialNeedUpdate) {
		ASSERT(setBasicFinancialMetric.IsEOF());
		for (size_t i = 0; i < iBasicFinancialNeedUpdate; i++) {
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
}

void CContainerFinnhubStock::ClearUpdateBasicFinancialFlag(const vector<CFinnhubStockPtr>& vStock) {
	for (const auto& pStock : vStock) {
		if (pStock->IsUpdateBasicFinancialDB()) {
			pStock->SetUpdateBasicFinancialDB(false);
			pStock->ClearBasicFinancialState();
		}
	}
}

void CContainerFinnhubStock::UpdateInsiderTransactionDB() {
	for (size_t i = 0; i < m_vStock.size(); i++) {
		const CFinnhubStockPtr pStock = GetItem(i);
		if (pStock->IsUpdateInsiderTransactionDB()) {
			pStock->SetUpdateInsiderTransactionDB(false);
			if (pStock->HaveInsiderTransaction()) {
				pStock->UpdateInsiderTransactionDB();
			}
		}
		if (gl_systemConfiguration.IsExitingSystem()) {
			break; // 如果程序正在退出，则停止存储。
		}
	}
}

bool CContainerFinnhubStock::ValidateStockSymbol(const CFinnhubStockPtr& pStock) {
	const string strSymbol = pStock->GetSymbol();
	const string strExchangeCode = pStock->GetExchangeCode();

	if (strExchangeCode == "US") return true;
	const auto pos = strSymbol.find("." + strExchangeCode);
	if ((pos + 1) < (strSymbol.length() - strExchangeCode.length())) {
		return false;
	}
	return true;
}

bool CContainerFinnhubStock::IsUpdateCompanyNewsDB() {
	return std::ranges::any_of(m_vStock, [](const CVirtualStockPtr& pStock) { return dynamic_pointer_cast<CFinnhubStock>(pStock)->IsUpdateCompanyNewsDB(); });
}

bool CContainerFinnhubStock::IsUpdateBasicFinancialDB() {
	return std::ranges::any_of(m_vStock, [](const CVirtualStockPtr& pStock) { return dynamic_pointer_cast<CFinnhubStock>(pStock)->IsUpdateBasicFinancialDB(); });
}

bool CContainerFinnhubStock::IsUpdateInsiderTransactionDB() {
	return std::ranges::any_of(m_vStock, [](const CVirtualStockPtr& P) { return dynamic_pointer_cast<CFinnhubStock>(P)->IsUpdateInsiderTransactionDB(); });
}

bool CContainerFinnhubStock::IsUpdateInsiderSentimentDB() {
	return std::ranges::any_of(m_vStock, [](const CVirtualStockPtr& p) { return dynamic_pointer_cast<CFinnhubStock>(p)->IsUpdateInsiderSentimentDB(); });
}

bool CContainerFinnhubStock::IsUpdateEPSSurpriseDB() {
	return std::ranges::any_of(m_vStock, [](const CVirtualStockPtr& p) { return dynamic_pointer_cast<CFinnhubStock>(p)->IsUpdateEPSSurpriseDB(); });
}

bool CContainerFinnhubStock::IsUpdateSECFilingsDB() {
	return std::ranges::any_of(m_vStock, [](const CVirtualStockPtr& p) { return dynamic_pointer_cast<CFinnhubStock>(p)->IsUpdateSECFilingsDB(); });
}
