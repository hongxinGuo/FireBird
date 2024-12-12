#include"pch.h"
module;
#include"TiingoCompanyDividend.h"
export module FireBird.Product.Tiingo.CompanyDividend;

import FireBird.Product.Tiingo.Base;

export {
	class CProductTiingoCompanyDividendSymbol final : public CProductTiingo {
	public:
		CProductTiingoCompanyDividendSymbol();
		~CProductTiingoCompanyDividendSymbol() override = default;

		CString CreateMessage() override;
		void ParseAndStoreWebData(CWebDataPtr pWebData) override;
		CTiingoCompanyDividendsPtr ParseTiingoCompanyDividendSymbol(const CWebDataPtr& pWebData);

		void UpdateDataSourceStatus(CVirtualDataSourcePtr pDataSource) override;
	};

	using CTiingoCompanyDividendSymbolPtr = shared_ptr<CProductTiingoCompanyDividendSymbol>;
}