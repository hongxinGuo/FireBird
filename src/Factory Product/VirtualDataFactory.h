//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// 网络数据的申请，采用Factory-Product模式，各数据对应相应的Product。
/// Factory只负责根据生成
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
class CVirtualWebProduct;
typedef shared_ptr<CVirtualWebProduct> CVirtualProductWebDataPtr;
class CVirtualMarket;
typedef shared_ptr<CVirtualMarket> CVirtualMarketPtr;
//typedef weak_ptr<CVirtualMarket> CVirtualMarketWeakPtr;

class CVirtualDataFactory {
public:
	CVirtualDataFactory() = default;

	virtual ~CVirtualDataFactory() = default;

	virtual CVirtualProductWebDataPtr CreateProduct(CVirtualMarketPtr pMarket, int iInquireType) = 0;
};
