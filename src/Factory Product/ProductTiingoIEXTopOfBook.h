#pragma once

#include"ProductTiingo.h"
import FireBird.Type.TiingoIEXTopOfBook; 

class CProductTiingoIEXTopOfBook final : public CProductTiingo {
public:
	CProductTiingoIEXTopOfBook();
	~CProductTiingoIEXTopOfBook() override = default;

	CString CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CTiingoIEXTopOfBooksPtr ParseTiingoIEXTopOfBook(const CWebDataPtr& pWebData);

	void UpdateDataSourceStatus(CVirtualDataSourcePtr pDataSource) override;
};

using CProductTiingoIEXTopOfBookPtr = shared_ptr<CProductTiingoIEXTopOfBook>;
