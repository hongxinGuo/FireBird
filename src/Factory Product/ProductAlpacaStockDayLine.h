/////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Note 腾讯日线目前一次能够提供2000个数据。当日线总量超过2000个时，需要分次查询不同日期的数据方可。
/// 目前采用的方法是生成一次多个查询，DataSource查询后将数据暂存于本Product中，待所有查询都完成后由本Product负责将数据组合起来。
///
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "TiingoCandleLine.h"
#include"VirtualChinaMarketWebProduct.h"

class CDayLine;

using std::vector;

class CProductAlpacaStockDayLine final : public CVirtualChinaMarketWebProduct {
public:
	CProductAlpacaStockDayLine();
	// 不允许复制和赋值。
	CProductAlpacaStockDayLine(const CProductAlpacaStockDayLine&) = delete;
	CProductAlpacaStockDayLine& operator=(const CProductAlpacaStockDayLine&) = delete;
	CProductAlpacaStockDayLine(const CProductAlpacaStockDayLine&&) noexcept = delete;
	CProductAlpacaStockDayLine& operator=(const CProductAlpacaStockDayLine&&) noexcept = delete;
	~CProductAlpacaStockDayLine() override = default;

	string CreateMessage() override;
	void ParseAndStoreWebData(shared_ptr<CWebData>) override { ABSL_DCHECK(0); } // Alpaca日线不使用此函数
	void ParseAndStoreWebData(shared_ptr<vector<CWebDataPtr>> pvWebData) override;
	CTiingoCandleLinesPtr ParseWebData(CWebDataPtr pWebData);
	void CheckAndPrepareDayLine(vector<CTiingoCandleLine>& vDayLine);

	void SetInquiryNumber(const int iNumber) { m_iInquiryNumber = iNumber; }
	int GetInquiryNumber() const { return m_iInquiryNumber; }

protected:
	long m_lCurrentStockPosition; // 股票当前查询位置
	int m_iInquiryNumber; // 本轮查询次数
};
