#pragma once

#include "ContainerTiingoSymbol.h"
#include"ProductTiingo.h"

#include"TiingoStock.h";

using std::shared_ptr;
using std::vector;
using std::string;

class CProductTiingoStockProfile final : public CProductTiingo {
public:
	CProductTiingoStockProfile();
	// 不允许赋值
	CProductTiingoStockProfile(const CProductTiingoStockProfile&) = delete;
	CProductTiingoStockProfile& operator=(const CProductTiingoStockProfile&) = delete;
	CProductTiingoStockProfile(const CProductTiingoStockProfile&&) noexcept = delete;
	CProductTiingoStockProfile& operator=(const CProductTiingoStockProfile&&) noexcept = delete;
	~CProductTiingoStockProfile() override = default;

	void InquireData(const std::stop_token& st, const string& strHeaders, const string& strParams, const string& strSuffix, const string& strInquiryToken) override; // default do nothing
	void WebStatusCheck(cpr::Response& r) override;
	void UpdateSystemStatus() override;

	shared_ptr<vector<string>> CreateMessage() override;
	CTiingoStocksPtr Parse(const string& text);

	void DeleteDuplicatedSymbol(const shared_ptr<vector<shared_ptr<CTiingoStock>>>& pvTiingoStock);
	static void SaveNewSymbol();
	void SaveDelistedSymbol();

protected:
	CContainerTiingoSymbol m_containerCurrentTiingoSymbols;
};

using CProductTiingoStockProfilePtr = shared_ptr<CProductTiingoStockProfile>;
