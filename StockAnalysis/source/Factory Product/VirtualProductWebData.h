#pragma once

#include<memory>
#include<string>

#include"ClassDeclaration.h"

#include"WebData.h"

enum {
	_VOID_DATA_ = 1,
	_NO_ACCESS_RIGHT_ = 2,
};

class CVirtualProductWebData : public CObject {
public:
	DECLARE_DYNCREATE(CVirtualProductWebData)
	CVirtualProductWebData();
	virtual ~CVirtualProductWebData() override = default;

	// 由于需要DECLARE_DYNCREATE此类，故而无法将CreateMessage和ParseAndStoreWebData声明为纯虚函数。
	virtual CString CreateMessage(void) { return _T(""); }
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData) { return true; }
	virtual bool AddInaccessibleExchangeIfNeeded(void) { return true; }; // 检查是否允许申请此类数据（当使用免费账户时，数据源会限制使用其某些功能）

	bool IsParsed(CWebDataPtr pWebData) { return pWebData->IsParsed(); }

	bool IsVoidJson(CWebDataPtr pWebData) {
		if (pWebData->IsVoidJson()) {
			m_iReceivedDataStatus = _VOID_DATA_;
			return true;
		}
		else return false;
	}

	bool IsVoidData(void) const noexcept { return m_iReceivedDataStatus == _VOID_DATA_; }
	virtual bool CheckNoRightToAccess(CWebDataPtr pWebData) { return false; }
	bool IsNoRightToAccess(void) const noexcept { return m_iReceivedDataStatus == _NO_ACCESS_RIGHT_; }

	CString GetClassName(void) { return m_strClassName; }
	CString GetInquiry(void) { return m_strInquiry; }

	long GetIndex(void) const noexcept { return m_lIndex; }
	void SetIndex(long const lIndex) { m_lIndex = lIndex; }

	CVirtualMarket* GetMarket(void) const { return m_pMarket; }
	void SetMarket(CVirtualMarket* pMarket) { m_pMarket = pMarket; }

	void SetInquiringExchange(CString exchange) noexcept { m_strInquiringExchange = exchange; }
	CString GetInquiringExchange(void) noexcept { return m_strInquiringExchange; }
	bool IsUSMarket(void); // 如果是美国市场

	void SetProductType(int iProductType) { m_iProductType = iProductType; }
	int GetProductType(void) const noexcept { return m_iProductType; }

protected:
	CVirtualMarket* m_pMarket; // 该产品所属的市场
	CString m_strClassName;
	CString m_strInquiry;
	CString m_strTotalInquiryMessage;
	CString m_strInquiringExchange; // 目前查询的交易所代码
	long m_lIndex;
	int m_iProductType;
	int m_iReceivedDataStatus;
	// 0:有效数据；1:void data(只有{}两个数据); 2:没有权利申请（{"error": "You don't have access to this resource."}）
};

typedef shared_ptr<CVirtualProductWebData> CVirtualProductWebDataPtr;

// 此结构只用于测试中
struct FinnhubWebData {
	FinnhubWebData(const long lIndex, const CString& strSymbol, const CString& strData) {
		m_lIndex = lIndex;
		m_strSymbol = strSymbol;
		m_pData = make_shared<CWebData>();
		m_pData->_Test_SetBuffer_(strData);
	}

	~FinnhubWebData() = default;

public:
	long m_lIndex;
	CString m_strSymbol;
	CWebDataPtr m_pData;
};

// 此结构只用于测试中
struct TiingoWebData {
	TiingoWebData(long lIndex, CString strSymbol, CString strData) {
		m_lIndex = lIndex;
		m_strSymbol = strSymbol;
		m_pData = make_shared<CWebData>();
		m_pData->_Test_SetBuffer_(strData);
	}

	~TiingoWebData() {
	}

public:
	long m_lIndex;
	CString m_strSymbol;
	CWebDataPtr m_pData;
};
