/////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Note 腾讯日线目前一次能够提供2000个数据。当日线总量超过2000个时，需要分次查询不同日期的数据方可。
/// 目前采用的方法是生成一次多个查询，DataSource查询后将数据暂存于本Product中，待所有查询都完成后由本Product负责将数据组合起来。
///
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"VirtualChinaMarketWebProduct.h"

using std::atomic_int;

class CProductTengxunDayLine final : public CVirtualChinaMarketWebProduct {
public:
	CProductTengxunDayLine();

	~CProductTengxunDayLine() override = default;

	CString CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	void ParseAndStoreWebData(vector<CWebDataPtr> vWebData) override;

	bool ReceivedAllData() const;

	static void ResetStaticVariable() {
		sm_vDayLinePtr.resize(0);
		sm_iCurrentNumber = 0;
	}

	void SetInquiryNumber(const int iNumber) { m_iInquiryNumber = iNumber; }
	int GetInquiryNumber() { return m_iInquiryNumber; }
	static void AppendDayLine(const vector<CDayLinePtr>& vDayLine);

	void CheckAndPrepareDayLine(vector<CDayLinePtr>& vDayLine);

protected:
	long m_lCurrentStockPosition; // 股票当前查询位置

	atomic_int m_iInquiryNumber; // 本轮查询次数

	static atomic_int sm_iCurrentNumber; // 本次计数值
	static vector<CDayLinePtr> sm_vDayLinePtr; //本轮查询到的日线数据。
};
