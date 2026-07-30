module;

export module Product.Tiingo.FinancialState;

import Product.Tiingo;
import WebData;
import TiingoCompanyFinancialState;

import std;
using std::string;
using std::shared_ptr;
using std::vector;

export {
	class CProductTiingoFinancialState final : public CProductTiingo {
	public:
		CProductTiingoFinancialState();
		// 不允许赋值。
		CProductTiingoFinancialState(const CProductTiingoFinancialState&) = delete;
		CProductTiingoFinancialState& operator=(const CProductTiingoFinancialState&) = delete;
		CProductTiingoFinancialState(const CProductTiingoFinancialState&&) noexcept = delete;
		CProductTiingoFinancialState& operator=(const CProductTiingoFinancialState&&) noexcept = delete;
		~CProductTiingoFinancialState() override = default;

		string CreateMessage() override;
		void ParseAndStoreWebData(CWebDataPtr pWebData) override;
		shared_ptr<vector<shared_ptr<CTiingoCompanyFinancialState>>> ParseTiingoFinancialState(const CWebDataPtr& pWebData);
	};

	using CProductTiingoFinancialStatePtr = shared_ptr<CProductTiingoFinancialState>;
}