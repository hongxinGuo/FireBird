#pragma once

#include"ProductTiingo.h"
#include "TiingoIEXTopOFBook.h"

class CProductTiingoIEXTopOFBook final : public CProductTiingo {
public:
	CProductTiingoIEXTopOFBook();
	~CProductTiingoIEXTopOFBook() override = default;

	CString CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CTiingoIEXTopOFBooksPtr ParseTiingoIEXTopOFBook(const CWebDataPtr& pWebData);

	void UpdateDataSourceStatus(CVirtualDataSourcePtr pDataSource) override;
};

using CProductTiingoIEXTopOFBookPtr = shared_ptr<CProductTiingoIEXTopOFBook>;
