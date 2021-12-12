#pragma once

using namespace std;
#include<memory>

#include"WebData.h"

class CWebSourceDataProduct : public CObject {
public:
	CWebSourceDataProduct() {
		m_lIndex = -1; // 设置为无效数字
	}
	~CWebSourceDataProduct() {}

	virtual CString CreatMessage(void) = 0;
	virtual bool ProcessWebData(CWebDataPtr pWebData) = 0;

	CString GetName(void) { return m_strClassName; }
	CString GetInquiringStr(void) { return m_strInquiringStr; }
	long GetIndex(void) { return m_lIndex; }
	void SetIndex(long lIndex) { m_lIndex = lIndex; }

protected:
	CString m_strClassName;
	CString m_strInquiringStr;
	long m_lIndex;
};

typedef shared_ptr<CWebSourceDataProduct> CWebSourceDataProductPtr;
