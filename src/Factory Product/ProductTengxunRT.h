#pragma once

#include"VirtualChinaMarketWebProduct.h"

class CProductTengxunRT final : public CVirtualChinaMarketWebProduct {
public:
	CProductTengxunRT();
	// 不允许赋值。
	CProductTengxunRT(const CProductTengxunRT&) = delete;
	CProductTengxunRT& operator=(const CProductTengxunRT&) = delete;
	CProductTengxunRT(const CProductTengxunRT&&) noexcept = delete;
	CProductTengxunRT& operator=(const CProductTengxunRT&&) noexcept = delete;
	~CProductTengxunRT() override = default;

	void InquireData(const std::stop_token& st, const string& strHeaders, const string& strParams, const string& strSuffix, const string& strInquiryToken) override; // default do nothing
	void WebStatusCheck(cpr::Response& r) override;
	void UpdateSystemStatus() override;

	shared_ptr<vector<string>> CreateMessage() override;

protected:
	long m_lCurrentStockPosition; // 股票当前查询位置
};
