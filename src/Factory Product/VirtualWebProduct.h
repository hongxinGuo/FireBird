//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// 网络数据的申请，采用Factory-Product模式，各数据对应相应的Product。
/// Product具体完成申请数据的组装，返回的网络数据的解析和存储。
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"ClassDeclaration.h"

enum {
	GOOD_DATA__ = 1,
	VOID_DATA_,
	NO_ACCESS_RIGHT_,
};

[[nodiscard]] bool CompareDayLineDate(const CDayLinePtr& p1, const CDayLinePtr& p2);

class CVirtualWebProduct {
public:
	CVirtualWebProduct() {} // default do nothing
	virtual ~CVirtualWebProduct() = default;

	virtual string CreateMessage() { return _T(""); };
	virtual void CalculateTotalDataLength(shared_ptr<vector<CWebDataPtr>>) {}

	virtual void ParseAndStoreWebData(CWebDataPtr) {} // default do nothing
	virtual void ParseAndStoreWebData(shared_ptr<vector<CWebDataPtr>> pvWebData) {// 一次处理多个接收到的数据。目前只有腾讯日线数据需要这种模式
		ASSERT(pvWebData->size() == 1);
		ParseAndStoreWebData(pvWebData->at(0)); // 默认只有一个数据，
	}
	virtual void AddInaccessibleSymbol() {} // 检查是否允许申请此类数据（当使用免费账户时，数据源会限制使用其某些功能）
	virtual void UpdateDataSourceStatus(CVirtualDataSourcePtr) {} // default do nothing

	bool CheckInaccessible();
	bool IsVoidJson(const CWebDataPtr& pWebData);

	bool IsVoidData() const noexcept { return m_iReceivedDataStatus == VOID_DATA_; }
	bool IsNoRightToAccess() const noexcept { return m_iReceivedDataStatus == NO_ACCESS_RIGHT_; }
	int GetReceivedDataStatus() const noexcept { return m_iReceivedDataStatus; }
	void SetReceivedDataStatus(int iType) noexcept { m_iReceivedDataStatus = iType; }

	string GetInquiry() const noexcept { return m_strInquiry; }
	string GetInquiryFunction() const noexcept { return m_strInquiryFunction; }
	void SetInquiryFunction(const string& strMessage) noexcept { m_strInquiryFunction = strMessage; }

	auto GetIndex() const noexcept { return m_lIndex; }
	void SetIndex(const size_t lIndex) noexcept { m_lIndex = lIndex; }

	void SetMarket(const CVirtualMarketPtr& pMarket) noexcept { m_pMarket = pMarket; }

	void SetInquiringExchange(const CString& exchange) noexcept { m_strInquiringExchange = exchange; }
	string GetInquiringExchange() const noexcept { return m_strInquiringExchange; }
	bool IsUSMarket() const; // 如果是美国市场

	void SetInquiringSymbol(const string& symbol) noexcept { m_strInquiringSymbol = symbol; }
	string GetInquiringSymbol() const noexcept { return m_strInquiringSymbol; }

	void SetInquireType(const int iInquireType) noexcept { m_iInquireType = iInquireType; }
	int GetInquireType() const noexcept { return m_iInquireType; }

	// 测试用
	virtual bool __Test_checkAccessRight(CWebDataPtr) { return true; }  // todo 不再使用，准备删除之

protected:
	CVirtualMarketWeakPtr m_pMarket;// Product被用于工作线程中。当系统退出时，由于无法保证工作线程先结束，故而此处使用weak_ptr智能指针以防止内存泄露。
	string m_strInquiryFunction{ _T("") };
	string m_strInquiry{ _T("") };
	string m_strInquiringExchange{ _T("ALL") }; // 默认是申请所有的交易所数据。
	string m_strInquiringSymbol{ _T("") }; // 目前查询的证券名称
	size_t m_lIndex{ 0 }; // 当虚处理的product为一聚合时，这个是索引。 预先设置为越界
	int m_iInquireType{ -1 }; // product索引，Finnhub申请的索引，如SYMBOL_LOOKUP_等。 预先设置为越界
	int m_iReceivedDataStatus{ GOOD_DATA__ }; // 1:有效数据；2:void data(只有{}或[]两个数据); 3:没有权利申请
};

using CVirtualWebProductPtr = shared_ptr<CVirtualWebProduct>;
