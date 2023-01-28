/////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// 腾讯日线目前一次能够提供2000个数据。当日线总量超过2000个时，需要分次查询不同日期的数据方可。
///
///
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include"ClassDeclaration.h"

#include"VirtualWebProduct.h"

class CProductTengxunDayLine final : public CVirtualWebProduct {
public:
	CProductTengxunDayLine();

	~CProductTengxunDayLine() override = default;

	CString CreateMessage(void) final;
	bool ParseAndStoreWebData(CWebDataPtr pWebData) override;

	bool ReceivedAllData(void);

	static void Reset() {
		sm_vDayLinePtr.resize(0);
		sm_lInquiryNumber = 1;
		sm_lCurrentNumber = 0;
	}

	static void SetInquiryNumber(long lNumber) { sm_lInquiryNumber = lNumber; }
	static void AppendDayLine(vector<CDayLinePtr> vDayLine);

	void CheckAndPrepareDayLine();

protected:
	long m_lCurrentStockPosition; // 股票当前查询位置

	static long sm_lInquiryNumber; // 本轮查询次数
	static long sm_lCurrentNumber; // 本次计数
	static vector<CDayLinePtr> sm_vDayLinePtr; //本轮查询到的日线数据。 
};
