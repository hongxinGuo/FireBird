#pragma once

using namespace std;
#include<memory>

#include"VirtualMarket.h"
#include"WebData.h"

// 此结构只用于测试中
struct FinnhubWebData {
	FinnhubWebData(long lIndex, CString strSymbol, CString strData) {
		m_lIndex = lIndex;
		m_strSymbol = strSymbol;
		m_pData = make_shared<CWebData>();
		m_pData->__TEST_SetBuffer__(strData);
	}

	~FinnhubWebData() {
	}

public:
	long m_lIndex;
	CString m_strSymbol;
	CWebDataPtr m_pData;
};

struct TiingoWebData {
	TiingoWebData(long lIndex, CString strSymbol, CString strData) {
		m_lIndex = lIndex;
		m_strSymbol = strSymbol;
		m_pData = make_shared<CWebData>();
		m_pData->__TEST_SetBuffer__(strData);
	}

	~TiingoWebData() {
	}

public:
	long m_lIndex;
	CString m_strSymbol;
	CWebDataPtr m_pData;
};

class CProductWebSourceData : public CObject {
public:
	DECLARE_DYNCREATE(CProductWebSourceData)
	CProductWebSourceData();
	~CProductWebSourceData() {}

	// 由于需要DECLARE_DYNCREATE此类，故而无法将CreatMessage和ProcessWebData声明为纯虚函数。
	virtual CString CreatMessage(void) { return _T(""); }
	virtual bool ProcessWebData(CWebDataPtr pWebData) { return true; }

	CString GetClassNameString(void) { return m_strClassName; }
	CString GetInquiringStr(void) { return m_strInquiringStr; }
	long GetIndex(void) { return m_lIndex; }
	void SetIndex(long lIndex) { m_lIndex = lIndex; }
	CVirtualMarket* GetMarket(void) { return m_pMarket; }
	void SetMarket(CVirtualMarket* pMarket) { m_pMarket = pMarket; }

protected:
	CVirtualMarket* m_pMarket; // 该产品所属的市场
	CString m_strClassName;
	CString m_strInquiringStr;
	long m_lIndex;
};

typedef shared_ptr<CProductWebSourceData> CWebSourceDataProductPtr;
