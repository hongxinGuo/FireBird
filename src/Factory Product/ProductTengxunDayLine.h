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

#include"atomic"
using std::atomic_int;

class CProductTengxunDayLine final : public CVirtualWebProduct {
public:
	CProductTengxunDayLine();

	~CProductTengxunDayLine() override = default;

	CString CreateMessage() override;
	bool ParseAndStoreWebData(CWebDataPtr pWebData) override;

	static bool ReceivedAllData();

	static void Reset() {
		sm_vDayLinePtr.resize(0);
		sm_iInquiryNumber = 1;
		sm_iCurrentNumber = 0;
	}

	static void SetInquiryNumber(const int iNumber) { sm_iInquiryNumber = iNumber; }
	static int GetInquiryNumber() { return sm_iInquiryNumber; }
	static void AppendDayLine(const vector<CDayLinePtr>& vDayLine);

	static void CheckAndPrepareDayLine();

protected:
	long m_lCurrentStockPosition; // 股票当前查询位置

	static atomic_int sm_iInquiryNumber; // 本轮查询次数
	static atomic_int sm_iCurrentNumber; // 本次计数
	static vector<CDayLinePtr> sm_vDayLinePtr; //本轮查询到的日线数据。 
};
