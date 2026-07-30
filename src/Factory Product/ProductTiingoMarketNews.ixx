module;

export module Product.Tiingo.MarketNews;

import Product.Tiingo;
import WebData;
import TiingoMarketNews;

import std;
using std::string;
using std::shared_ptr;
using std::vector;

export {
	class CProductTiingoMarketNews final : public CProductTiingo {
	public:
		CProductTiingoMarketNews();
		CProductTiingoMarketNews(const CProductTiingoMarketNews&) = delete;
		CProductTiingoMarketNews& operator=(const CProductTiingoMarketNews&) = delete;
		CProductTiingoMarketNews(const CProductTiingoMarketNews&&) noexcept = delete;
		CProductTiingoMarketNews& operator=(const CProductTiingoMarketNews&&) noexcept = delete;
		~CProductTiingoMarketNews() override = default;

		string CreateMessage() override;
		void ParseAndStoreWebData(CWebDataPtr pWebData) override;
		shared_ptr<vector<CTiingoMarketNews>> ParseTiingoMarketNews(const CWebDataPtr& pWebData);

		void UpdateSystemStatus() override;
	};

	using CProductTiingoMarketNewsPtr = shared_ptr<CProductTiingoMarketNews>;
}