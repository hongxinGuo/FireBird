/////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Note 腾讯日线目前一次能够提供2000个数据。当日线总量超过2000个时，需要分次查询不同日期的数据方可。
/// 目前采用的方法是生成一次多个查询，DataSource查询后将数据暂存于本Product中，待所有查询都完成后由本Product负责将数据组合起来。
///
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"VirtualChinaMarketWebProduct.h"

class CProductTengxunDayLine final : public CVirtualChinaMarketWebProduct {
public:
	CProductTengxunDayLine();

	~CProductTengxunDayLine() override = default;

	CString CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override { ASSERT(false); }; // 腾讯日线不使用此函数
	void ParseAndStoreWebData(shared_ptr<vector<CWebDataPtr>> pvWebData) override;

	void SetInquiryNumber(const int iNumber) { m_iInquiryNumber = iNumber; }
	int GetInquiryNumber() const { return m_iInquiryNumber; }

	void CheckAndPrepareDayLine(vector<CDayLinePtr>& vDayLine);

protected:
	long m_lCurrentStockPosition; // 股票当前查询位置
	int m_iInquiryNumber; // 本轮查询次数
};
