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

	// ������ҪDECLARE_DYNCREATE���࣬�ʶ��޷���CreatMessage��ParseAndStoreWebData����Ϊ���麯����
	virtual CString CreatMessage(void) { return _T(""); }
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData) { return true; }
	virtual bool CheckAccessibility(void) { return true; } // ����Ƿ���������������ݣ���ʹ������˻�ʱ������Դ������ʹ����ĳЩ���ܣ�

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
	CVirtualMarket* m_pMarket; // �ò�Ʒ�������г�
	CString m_strClassName;
	CString m_strInquiringStr;
	CString m_strTotalInquiryMessage;
	long m_lIndex;
	int m_iProductType;
	int m_iReceivedDataStatus; // 0:��Ч���ݣ�1:void data(ֻ��{}��������); 2:û��Ȩ�����루{"error": "You don't have access to this resource."}��
	int m_iNumberOfNoRightToAccess;
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
