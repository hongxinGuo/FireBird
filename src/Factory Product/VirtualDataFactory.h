//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// 网络数据的申请，采用Factory-Product模式，各数据对应相应的Product。
/// Factory只负责根据生成
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

class CVirtualDataFactory {
public:
	CVirtualDataFactory() = default;
	// 不允许赋值。
	CVirtualDataFactory(const CVirtualDataFactory&) = delete;
	CVirtualDataFactory& operator=(const CVirtualDataFactory&) = delete;
	CVirtualDataFactory(const CVirtualDataFactory&&) noexcept = delete;
	CVirtualDataFactory& operator=(const CVirtualDataFactory&&) noexcept = delete;
	virtual ~CVirtualDataFactory() = default;

	virtual CVirtualProductWebDataPtr CreateProduct(CVirtualMarketPtr pMarket, int iInquireType) = 0;
};
