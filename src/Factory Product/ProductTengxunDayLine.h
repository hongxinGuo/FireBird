/////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Note 腾讯日线目前一次能够提供2000个数据。当日线总量超过2000个时，需要分次查询不同日期的数据方可。
/// 目前采用的方法是生成一次多个查询，DataSource查询后将数据暂存于本Product中，待所有查询都完成后由本Product负责将数据组合起来。
///
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"VirtualChinaMarketWebProduct.h"

class CDayLine;

using std::vector;

class CProductTengxunDayLine final : public CVirtualChinaMarketWebProduct {
public:
	CProductTengxunDayLine();
	// 不允许复制和赋值。
	CProductTengxunDayLine(const CProductTengxunDayLine&) = delete;
	CProductTengxunDayLine& operator=(const CProductTengxunDayLine&) = delete;
	CProductTengxunDayLine(const CProductTengxunDayLine&&) noexcept = delete;
	CProductTengxunDayLine& operator=(const CProductTengxunDayLine&&) noexcept = delete;
	~CProductTengxunDayLine() override = default;

	void InquireData(const std::stop_token& st, const string& strHeaders, const string& strParams, const string& strSuffix, const string& strInquiryToken) override; // default do nothing
	void WebStatusCheck(cpr::Response& r) override;
	void UpdateSystemStatus() override;

	shared_ptr<vector<string>> CreateMessage() override;

	void SetStockIndex(long index) noexcept { m_lCurrentStockPosition = index; }
	void SetStockSymbol(string symbol) { m_stockSymbol = symbol; }

	void SetInquiryNumber(const int iNumber) { m_iInquiryNumber = iNumber; }
	int GetInquiryNumber() const { return m_iInquiryNumber; }

	void CheckAndPrepareDayLine(vector<CDayLine>& vDayLine);

protected:
	string m_strInquiryFunction{ "https://web.ifzq.gtimg.cn/appstock/app/fqkline/get?param=" };
	string m_strSuffix{ ",2000,," };

	string m_stockSymbol;
	long m_lCurrentStockPosition; // 股票当前查询位置
	int m_iInquiryNumber; // 本轮查询次数
};
