//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// 网络数据的申请，采用Factory-Product模式，各数据对应相应的Product。
/// Factory只负责根据生成
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
module;

export module Factory;

import Market;
import Product;

import std;
using std::shared_ptr;

export {
	class CVirtualDataFactory {
	public:
		CVirtualDataFactory() = default;
		// 不允许赋值。
		CVirtualDataFactory(const CVirtualDataFactory&) = delete;
		CVirtualDataFactory& operator=(const CVirtualDataFactory&) = delete;
		CVirtualDataFactory(const CVirtualDataFactory&&) noexcept = delete;
		CVirtualDataFactory& operator=(const CVirtualDataFactory&&) noexcept = delete;
		virtual ~CVirtualDataFactory() = default;

		virtual CVirtualWebProductPtr CreateProduct(shared_ptr<CVirtualMarket> pMarket, int iInquireType) = 0;
	};
}
