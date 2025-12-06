#pragma once

#include"ProductTiingo.h"
#include "TiingoIEXTopOFBook.h"

class CProductTiingoIEXTopOfBook final : public CProductTiingo {
public:
	CProductTiingoIEXTopOfBook();
	~CProductTiingoIEXTopOfBook() override = default;

	string CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CTiingoIEXTopOfBooksPtr ParseTiingoIEXTopOfBook(const CWebDataPtr& pWebData);

	void UpdateSystemStatus() override;
};

using CProductTiingoIEXTopOfBookPtr = shared_ptr<CProductTiingoIEXTopOfBook>;
