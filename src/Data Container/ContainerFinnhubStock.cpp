//////////////////////////////////////////////////////////////////////////////////////////
//
// CContainerFinnhubStock类操作worldMarket数据库中的finnhub_stock_profile表。
//
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "pch.h"

#include"WorldMarket.h"
#include "ContainerFinnhubStock.h"

#include "jsonParse.h"

#include<sqlpp23/sqlpp23.h>

#include "dataBaseConnector.h"
#include"StockMarketSQLTable.h"
#include "SystemConfiguration.h"
#include"FinnhubStock.h"
#include"TimeConvert.h"

using std::make_shared;

CContainerFinnhubStock::CContainerFinnhubStock() {
	CContainerFinnhubStock::Reset();
}

void CContainerFinnhubStock::Reset() {
	CContainerVirtualStock::Reset();
}

CFinnhubStockPtr CContainerFinnhubStock::GetItem(size_t lIndex) {
	return dynamic_pointer_cast<CFinnhubStock>(Get(lIndex));
}

CFinnhubStockPtr CContainerFinnhubStock::GetItem(const string& strStockCode) {
	return dynamic_pointer_cast<CFinnhubStock>(Get(strStockCode));
}

void CContainerFinnhubStock::ResetEPSSurprise() {
	for (size_t l = 0; l < m_vStock.size(); l++) {
		const CFinnhubStockPtr pStock = GetItem(l);
		pStock->SetLastEPSSurpriseUpdateDate(toLocalDays(19800101));
		pStock->m_fUpdateEPSSurprise = true;
	}
}

void CContainerFinnhubStock::ResetPeer() {
	for (size_t l = 0; l < m_vStock.size(); l++) {
		const CFinnhubStockPtr pStock = GetItem(l);
		if (pStock->GetPeerUpdateDate() != toLocalDays(19800101)) {
			pStock->SetPeerUpdateDate(toLocalDays(19800101));
			pStock->SetUpdatePeer(true);
			pStock->SetUpdateProfileDB(true);
		}
	}
}

void CContainerFinnhubStock::ResetBasicFinancial() {
	for (size_t l = 0; l < m_vStock.size(); l++) {
		const CFinnhubStockPtr pStock = GetItem(l);
		if (pStock->GetBasicFinancialUpdateDate() != toLocalDays(19800101)) {
			pStock->SetBasicFinancialUpdateDate(toLocalDays(19800101));
			pStock->SetUpdateBasicFinancial(true);
			pStock->SetUpdateProfileDB(true);
		}
	}
}

void CContainerFinnhubStock::ResetDayLine() {
	for (size_t l = 0; l < m_vStock.size(); l++) {
		const CFinnhubStockPtr pStock = GetItem(l);
		pStock->SetDayLineStartDate(toLocalDays(29900101));
		pStock->SetDayLineEndDate(toLocalDays(19800101));
		pStock->SetUpdateDayLine(true);
		pStock->SetUpdateProfileDB(true);
	}
}

bool CContainerFinnhubStock::LoadProfileDB() {
	using namespace StockMarket;
	const auto& t = FinnhubStockProfile{};

	auto db = gl_dbStockMarket.get();
	auto tx = start_transaction(db);
	auto result = db(select(all_of(t)).from(t).order_by(t.ID.asc()));
	auto rowCount = result.size();
	Reserve(rowCount + 10); // 预留一些空间，避免后续添加新股票时频繁扩容
	CFinnhubStockPtr pFinnhubStock = nullptr;

	for (const auto& row : result) {
		pFinnhubStock = make_shared<CFinnhubStock>();
		pFinnhubStock->SetSymbol(row.Symbol);
		pFinnhubStock->SetExchange(row.Exchange);
		pFinnhubStock->SetDescription(row.Description);
		pFinnhubStock->SetDisplaySymbol(row.DisplaySymbol);
		pFinnhubStock->SetType(string{ row.Type });
		pFinnhubStock->SetMic(string{ row.Mic });
		pFinnhubStock->SetFigi(string{ row.Figi });
		pFinnhubStock->SetCurrency(string{ row.Currency });
		pFinnhubStock->SetAddress(string{ row.Address });
		pFinnhubStock->SetCity(string{ row.City });
		pFinnhubStock->SetCountry(string{ row.Country });
		pFinnhubStock->SetCusip(string{ row.cusip });
		pFinnhubStock->SetSedol(string{ row.sedol });
		pFinnhubStock->SetEmployeeTotal(row.EmployeeTotal);
		pFinnhubStock->SetGgroup(string{ row.ggroup });
		pFinnhubStock->SetGind(string{ row.gind });
		pFinnhubStock->SetGsector(string{ row.gsector });
		pFinnhubStock->SetGsubind(string{ row.gsubind });
		pFinnhubStock->SetIPODate(string{ row.IPODate });
		pFinnhubStock->SetIsin(string{ row.isin });
		pFinnhubStock->SetMarketCapitalization(row.MarketCapitalization);
		pFinnhubStock->SetNaics(string{ row.naics });
		pFinnhubStock->SetNaicsNationalIndustry(string{ row.naicsNationalIndustry });
		pFinnhubStock->SetNaicsSector(string{ row.naicsSector });
		pFinnhubStock->SetNaicsSubsector(string{ row.naicsSubsector });
		pFinnhubStock->SetName(string{ row.Name });
		pFinnhubStock->SetPhone(string{ row.Phone });
		pFinnhubStock->SetShareOutstanding(row.ShareOutstanding);
		pFinnhubStock->SetState(string{ row.state });
		pFinnhubStock->SetTicker(string{ row.Ticker });
		pFinnhubStock->SetWebURL(string{ row.WebURL });
		pFinnhubStock->SetLogo(string{ row.Logo });
		pFinnhubStock->SetFinnhubIndustry(string{ row.FinnhubIndustry });
		string str = string{ row.Peer };
		if (str.length() > 2) {
			nlohmannJson js;
			CreateJsonWithNlohmann(js, str);
			pFinnhubStock->SetPeer(js);
		}
		pFinnhubStock->LoadUpdateDate(string{ row.UpdateDate });
		if (!IsSymbol(pFinnhubStock->GetSymbol())) {
			pFinnhubStock->CheckUpdateStatus(gl_pWorldMarket->GetMarketDate());
			Add(pFinnhubStock);
			ABSL_DCHECK(pFinnhubStock->GetSymbol().length() < 12);// 目前WorldMarket数据库的股票代码长度限制为12个字符
		}
		else {
			db(sqlpp::delete_from(t).where(t.ID == row.ID)); // 如果数据库中存在重复的股票代码，则删除重复的记录。
		}
	}
	tx.commit();
	Sort();

	return true;
}

void CContainerFinnhubStock::UpdateProfileDB() {
	ABSL_DCHECK(IsUpdateProfileDB());

	using namespace StockMarket;
	const auto& t = FinnhubStockProfile{};
	auto db = gl_dbStockMarket.get();
	auto tx = start_transaction(db);

	for (size_t l = 0; l < m_vStock.size(); l++) {
		const CFinnhubStockPtr pStock = GetItem(l);
		ABSL_DCHECK(pStock != nullptr);
		if (pStock->IsUpdateProfileDB()) {
			pStock->UpdateJsonUpdateDate();
			if (pStock->IsNewStock()) {// 新代码，插入。
				db(insert_into(t).set(
					t.Symbol = pStock->GetSymbol(),
					t.Exchange = pStock->GetExchange(),
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
					t.EmployeeTotal = static_cast<int>(pStock->GetEmployeeTotal()),
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
					t.Peer = pStock->GetJsonPeer().dump(),
					t.UpdateDate = pStock->GetJsonUpdateDate().dump()
				));
				pStock->SetNewStock(false);
			}
			else { // 如果是原有的代码，则更新
				db(update(t).set(
					t.Symbol = pStock->GetSymbol(),
					t.Exchange = pStock->GetExchange(),
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
					t.EmployeeTotal = static_cast<int>(pStock->GetEmployeeTotal()),
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
					t.Peer = pStock->GetJsonPeer().dump(),
					t.UpdateDate = pStock->GetJsonUpdateDate().dump()
				).where(t.Symbol == pStock->GetSymbol()));
			}
			pStock->SetUpdateProfileDB(false);
		}
	}
	tx.commit();
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
	const string strExchangeCode = pStock->GetExchange();

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
