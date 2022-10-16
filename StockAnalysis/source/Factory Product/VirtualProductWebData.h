#pragma once

using namespace std;
#include<memory>
#include<string>

#include"ClassDeclaration.h"

#include"WebData.h"

class CVirtualProductWebData : public CObject {
public:
	DECLARE_DYNCREATE(CVirtualProductWebData)
		CVirtualProductWebData();
	virtual ~CVirtualProductWebData() {}

	// ������ҪDECLARE_DYNCREATE���࣬�ʶ��޷���CreatMessage��ParseAndStoreWebData����Ϊ���麯����
	virtual CString CreatMessage(void) { return _T(""); }
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData) { return true; }

	CString GetClassNameString(void) { return m_strClassName; }
	CString GetInquiringStr(void) { return m_strInquiringStr; }
	long GetIndex(void) { return m_lIndex; }
	void SetIndex(long lIndex) { m_lIndex = lIndex; }
	CVirtualMarket* GetMarket(void) { return m_pMarket; }
	void SetMarket(CVirtualMarket* pMarket) { m_pMarket = pMarket; }

	void SetProductType(int iProductType) { m_iProductType = iProductType; }
	int GetProductType(void) { return m_iProductType; }

protected:
	CVirtualMarket* m_pMarket; // �ò�Ʒ�������г�
	CString m_strClassName;
	CString m_strInquiringStr;
	long m_lIndex;
	int m_iProductType;
};

typedef shared_ptr<CVirtualProductWebData> CProductWebSourceDataPtr;

// �˽ṹֻ���ڲ�����
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

// �˽ṹֻ���ڲ�����
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
