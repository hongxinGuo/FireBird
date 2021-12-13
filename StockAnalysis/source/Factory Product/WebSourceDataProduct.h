#pragma once

using namespace std;
#include<memory>

#include"VirtualMarket.h"
#include"WebData.h"

class CWebSourceDataProduct : public CObject {
public:
	DECLARE_DYNCREATE(CWebSourceDataProduct)
	CWebSourceDataProduct();
	~CWebSourceDataProduct() {}

	virtual CString CreatMessage(void) { return _T(""); }
	virtual bool ProcessWebData(CWebDataPtr pWebData) { return true; }

	CString GetName(void) { return m_strClassName; }
	CString GetInquiringStr(void) { return m_strInquiringStr; }
	long GetIndex(void) { return m_lIndex; }
	void SetIndex(long lIndex) { m_lIndex = lIndex; }
	CVirtualMarket* GetMarket(void) { return m_pMarket; }
	void SetMarket(CVirtualMarket* pMarket) { m_pMarket = pMarket; }

protected:
	CVirtualMarket* m_pMarket; // �ò�Ʒ�������г�
	CString m_strClassName;
	CString m_strInquiringStr;
	long m_lIndex;
};

typedef shared_ptr<CWebSourceDataProduct> CWebSourceDataProductPtr;
