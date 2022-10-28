#pragma once

using namespace std;
#include<memory>
#include<string>

#include"ClassDeclaration.h"

#include"WebData.h"

enum {
	__VOID_DATA__ = 1,
	__NO_ACCESS_RIGHT__ = 2,
};

class CVirtualProductWebData : public CObject {
public:
	DECLARE_DYNCREATE(CVirtualProductWebData)
		CVirtualProductWebData();
	virtual ~CVirtualProductWebData() {}

	// 由于需要DECLARE_DYNCREATE此类，故而无法将CreatMessage和ParseAndStoreWebData声明为纯虚函数。
	virtual CString CreatMessage(void) { return _T(""); }
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData) { return true; }
	virtual bool CheckAccessibility(void) { return true; } // 检查是否允许申请此类数据（当使用免费账户时，数据源会限制使用其某些功能）

	CString GetClassNameString(void) { return m_strClassName; }
	CString GetInquiringStr(void) { return m_strInquiringStr; }
	long GetIndex(void) { return m_lIndex; }
	void SetIndex(long lIndex) { m_lIndex = lIndex; }
	CVirtualMarket* GetMarket(void) { return m_pMarket; }
	void SetMarket(CVirtualMarket* pMarket) { m_pMarket = pMarket; }

	void SetProductType(int iProductType) { m_iProductType = iProductType; }
	int GetProductType(void) { return m_iProductType; }

	bool IsVoidData(void) noexcept { return m_iReceivedDataStatus == __VOID_DATA__; }
	bool NoRightToAccess(void) noexcept { return m_iReceivedDataStatus == __NO_ACCESS_RIGHT__; }

protected:
	CVirtualMarket* m_pMarket; // 该产品所属的市场
	CString m_strClassName;
	CString m_strInquiringStr;
	CString m_strTotalInquiryMessage;
	long m_lIndex;
	int m_iProductType;
	int m_iReceivedDataStatus; // 0:有效数据；1:void data(只有{}两个数据); 2:没有权利申请（{"error": "You don't have access to this resource."}）
	int m_iNumberOfNoRightToAccess;
};

typedef shared_ptr<CVirtualProductWebData> CProductWebSourceDataPtr;

// 此结构只用于测试中
struct FinnhubWebData {
	FinnhubWebData(long lIndex, CString strSymbol, CString strData) {
		m_lIndex = lIndex;
		m_strSymbol = strSymbol;
		m_pData = make_shared<CWebData>();
		m_pData->__TEST_SetBuffer(strData);
	}

	~FinnhubWebData() {
	}

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
		m_pData->__TEST_SetBuffer(strData);
	}

	~TiingoWebData() {
	}

public:
	long m_lIndex;
	CString m_strSymbol;
	CWebDataPtr m_pData;
};
