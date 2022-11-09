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
	virtual bool AddInaccessibleExchangeIfNeeded(void) { return true; }; // ����Ƿ���������������ݣ���ʹ������˻�ʱ������Դ������ʹ����ĳЩ���ܣ�

	bool IsParsed(CWebDataPtr pWebData) { return pWebData->IsParsed(); }
	bool IsVoidJson(CWebDataPtr pWebData) {
		if (pWebData->IsVoidJson()) { m_iReceivedDataStatus = __VOID_DATA__; return true; }
		else return false;
	}
	bool IsVoidData(void) noexcept { return m_iReceivedDataStatus == __VOID_DATA__; }
	bool CheckNoRightToAccess(CWebDataPtr pWebData) {
		if (pWebData->NoRightToAccess()) { m_iReceivedDataStatus = __NO_ACCESS_RIGHT__; return true; }
		else return false;
	}
	bool IsNoRightToAccess(void) noexcept { return m_iReceivedDataStatus == __NO_ACCESS_RIGHT__; }

	CString GetClassName(void) { return m_strClassName; }
	CString GetInquiry(void) { return m_strInquiry; }

	long GetIndex(void) { return m_lIndex; }
	void SetIndex(long lIndex) { m_lIndex = lIndex; }

	CVirtualMarket* GetMarket(void) { return m_pMarket; }
	void SetMarket(CVirtualMarket* pMarket) { m_pMarket = pMarket; }

	void SetInquiringExchange(CString exchange) noexcept { m_strInquiringExchange = exchange; }
	CString GetInquiringExchange(void) noexcept { return m_strInquiringExchange; }
	bool IsUSMarket(void); // ����������г�

	void SetProductType(int iProductType) { m_iProductType = iProductType; }
	int GetProductType(void) { return m_iProductType; }

protected:
	CVirtualMarket* m_pMarket; // �ò�Ʒ�������г�
	CString m_strClassName;
	CString m_strInquiry;
	CString m_strTotalInquiryMessage;
	CString m_strInquiringExchange; // Ŀǰ��ѯ�Ľ���������
	long m_lIndex;
	int m_iProductType;
	int m_iReceivedDataStatus; // 0:��Ч���ݣ�1:void data(ֻ��{}��������); 2:û��Ȩ�����루{"error": "You don't have access to this resource."}��
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
