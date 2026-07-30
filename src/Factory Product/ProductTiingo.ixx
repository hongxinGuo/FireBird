module;

export module Product.Tiingo;

import Product;
import WebData;

import std;
using std::int64_t;
using std::shared_ptr;
using std::vector;

export {
	class CProductTiingo : public CVirtualWebProduct {
	public:
		CProductTiingo() = default;
		// 不允许赋值。
		CProductTiingo(const CProductTiingo&) = delete;
		CProductTiingo& operator=(const CProductTiingo&) = delete;
		CProductTiingo(const CProductTiingo&&) noexcept = delete;
		CProductTiingo& operator=(const CProductTiingo&&) noexcept = delete;
		~CProductTiingo() override = default;

		void CalculateTotalDataLength(shared_ptr<vector<CWebDataPtr>> pvWebData) final;

		void AddInaccessibleSymbol() override; // 检查是否允许申请此类数据（当使用免费账户时，数据源会限制使用其某些功能）

		// 测试用函数
		bool Test_checkAccessRight_(CWebDataPtr pWebData) final; // todo 不再使用，准备删除之. 目前只用于测试函数中。
	};

	extern int64_t gl_TiingoTotalData;
}