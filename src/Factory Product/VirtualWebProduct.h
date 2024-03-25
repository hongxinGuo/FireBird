//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// 网络数据的申请，采用Factory-Product模式，各数据对应相应的Product。
/// Product具体完成申请数据的组装，返回的网络数据的解析和存储。
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"ClassDeclaration.h"
#include"WebData.h"

using std::make_shared;

// 此结构只用于测试中
struct Test_FinnhubWebData {
	Test_FinnhubWebData(const long lIndex, const CString& strSymbol, const CString& strData) {
		m_lIndex = lIndex;
		m_strSymbol = strSymbol;
		m_pData = make_shared<CWebData>();
		m_pData->Test_SetBuffer_(strData);
	}

	~Test_FinnhubWebData() = default;

public:
	long m_lIndex;
	CString m_strSymbol;
	CWebDataPtr m_pData;
};

extern Test_FinnhubWebData finnhubWebData0;
extern Test_FinnhubWebData finnhubWebData1;

// 此结构只用于测试中
struct Test_TiingoWebData {
	Test_TiingoWebData(const long lIndex, const CString& strSymbol, const CString& strData) {
		m_lIndex = lIndex;
		m_strSymbol = strSymbol;
		m_pData = make_shared<CWebData>();
		m_pData->Test_SetBuffer_(strData);
	}

	~Test_TiingoWebData() {}

public:
	long m_lIndex;
	CString m_strSymbol;
	CWebDataPtr m_pData;
};

enum {
	VOID_DATA_ = 1,
	NO_ACCESS_RIGHT_ = 2,
};

[[nodiscard]] bool CompareDayLineDate(const CDayLinePtr& p1, const CDayLinePtr& p2);

class CVirtualWebProduct {
public:
	CVirtualWebProduct();
	virtual ~CVirtualWebProduct() = default;

	virtual CString CreateMessage() { return _T(""); };
	virtual void ParseAndStoreWebData(CWebDataPtr pWebData) {};
	virtual void ParseAndStoreWebData(shared_ptr<vector<CWebDataPtr>> pvWebData) {// 一次处理多个接收到的数据。目前只有腾讯日线数据需要这种模式
		ASSERT(pvWebData->size() == 1);
		ParseAndStoreWebData(pvWebData->at(0)); // 默认只有一个数据，
	}
	virtual void AddInaccessibleExchange() {} // 检查是否允许申请此类数据（当使用免费账户时，数据源会限制使用其某些功能）

	virtual void UpdateDataSourceStatus(CVirtualDataSourcePtr pDataSource) {} // default do nothing

	bool CheckInaccessible(const CWebDataPtr& pWebData);
	bool IsVoidJson(const CWebDataPtr& pWebData);

	bool IsVoidData() const noexcept { return m_iReceivedDataStatus == VOID_DATA_; }
	virtual bool CheckAccessRight(CWebDataPtr pWebData) { return true; }
	bool IsNoRightToAccess() const noexcept { return m_iReceivedDataStatus == NO_ACCESS_RIGHT_; }
	int GetReceivedDataStatus() const noexcept { return m_iReceivedDataStatus; }
	void SetReceivedDataStatus(int iType) noexcept { m_iReceivedDataStatus = iType; }

	CString GetInquiry() const noexcept { return m_strInquiry; }
	CString GetInquiryFunction() const noexcept { return m_strInquiryFunction; }
	void SetInquiryFunction(const CString& strMessage) noexcept { m_strInquiryFunction = strMessage; }

	long GetIndex() const noexcept { return m_lIndex; }
	void SetIndex(const long lIndex) noexcept { m_lIndex = lIndex; }

	void SetMarket(const CVirtualMarketPtr& pMarket) noexcept { m_pMarket = pMarket; }

	void SetInquiringExchange(const CString& exchange) noexcept { m_strInquiringExchange = exchange; }
	CString GetInquiringExchange() const noexcept { return m_strInquiringExchange; }
	bool IsUSMarket() const; // 如果是美国市场

	void SetInquireType(const int iInquireType) noexcept { m_iInquireType = iInquireType; }
	int GetInquireType() const noexcept { return m_iInquireType; }

protected:
	CVirtualMarketWeakPtr m_pMarket;// Product被用于工作线程中。当系统退出时，由于无法保证工作线程先结束，故而此处使用weak_ptr智能指针以防止内存泄露。
	CString m_strInquiryFunction;
	CString m_strInquiry;
	CString m_strInquiringExchange; // 目前查询的交易所代码
	long m_lIndex; // 当虚处理的product为一聚合时，这个是索引
	int m_iInquireType; // product索引，Finnhub申请的索引，如SYMBOL_LOOKUP_等
	int m_iReceivedDataStatus; // 0:有效数据；1:void data(只有{}两个数据); 2:没有权利申请（{"error": "You don't have access to this resource."}）
};

using CVirtualWebProductPtr = shared_ptr<CVirtualWebProduct>;
