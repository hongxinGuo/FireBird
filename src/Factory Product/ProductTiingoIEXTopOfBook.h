#pragma once

#include"ProductTiingo.h"
#include "TiingoIEXTopOFBook.h"

class CTiingoIEXTopOfBook;

class CProductTiingoIEXTopOfBook final : public CProductTiingo {
public:
	CProductTiingoIEXTopOfBook();
	// 不允许赋值。
	CProductTiingoIEXTopOfBook(const CProductTiingoIEXTopOfBook&) = delete;
	CProductTiingoIEXTopOfBook& operator=(const CProductTiingoIEXTopOfBook&) = delete;
	CProductTiingoIEXTopOfBook(const CProductTiingoIEXTopOfBook&&) noexcept = delete;
	CProductTiingoIEXTopOfBook& operator=(const CProductTiingoIEXTopOfBook&&) noexcept = delete;
	~CProductTiingoIEXTopOfBook() override = default;

	void InquireData(const std::stop_token& st) override; // default do nothing
	void WebStatusCheck(cpr::Response& r) override;

	shared_ptr<vector<string>> CreateMessage() override;
	CTiingoIEXTopOfBooksPtr Parse(const string& text);

	void UpdateSystemStatus() override;
};

using CProductTiingoIEXTopOfBookPtr = shared_ptr<CProductTiingoIEXTopOfBook>;
