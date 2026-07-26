#pragma once

#include"ProductTiingo.h"

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

	string CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	shared_ptr<vector<shared_ptr<CTiingoIEXTopOfBook>>> ParseTiingoIEXTopOfBook(const CWebDataPtr& pWebData);

	void UpdateSystemStatus() override;
};

using CProductTiingoIEXTopOfBookPtr = shared_ptr<CProductTiingoIEXTopOfBook>;
