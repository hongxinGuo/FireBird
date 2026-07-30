module;

export module Product.Finnhub;

import Product;
import WebData;

import std;
using std::shared_ptr;
using std::vector;
using std::int64_t;

export {
	class CProductFinnhub : public CVirtualWebProduct {
	public:
		CProductFinnhub() = default;
		// 不允许赋值。
		CProductFinnhub(const CProductFinnhub&) = delete;
		CProductFinnhub& operator=(const CProductFinnhub&) = delete;
		CProductFinnhub(const CProductFinnhub&&) noexcept = delete;
		CProductFinnhub& operator=(const CProductFinnhub&&) noexcept = delete;
		~CProductFinnhub() override = default;

		void CalculateTotalDataLength(shared_ptr<vector<shared_ptr<CWebData>>> pvWebData) final;

		void AddInaccessibleSymbol() override; // 检查是否允许申请此类数据（当使用免费账户时，数据源会限制使用其某些功能）

		// 测试用函数
		bool Test_checkAccessRight_(shared_ptr<CWebData> pWebData) final; // todo 不再使用，准备删除之. 目前只用于测试函数中。
	};

	extern int64_t gl_FinnhubTotalData;
}