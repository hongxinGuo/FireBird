#pragma once

#include"ClassDeclaration.h"

#include"WebData.h"

#include<memory>
using std::make_shared;

enum {
	VOID_DATA_ = 1,
	NO_ACCESS_RIGHT_ = 2,
};

class CVirtualWebProduct {
public:
	CVirtualWebProduct();
	virtual ~CVirtualWebProduct() = default;

	virtual CString CreateMessage(void) { return _T(""); };
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData) { return true; };
	virtual bool AddInaccessibleExchangeIfNeeded(void) { return true; } // ����Ƿ���������������ݣ���ʹ������˻�ʱ������Դ������ʹ����ĳЩ���ܣ�

	bool IsVoidJson(const CWebDataPtr& pWebData) {
		if (pWebData->IsVoidJson()) {
			m_iReceivedDataStatus = VOID_DATA_;
			return true;
		}
		else return false;
	}

	bool IsVoidData(void) const noexcept { return m_iReceivedDataStatus == VOID_DATA_; }
	virtual bool CheckNoRightToAccess(CWebDataPtr pWebData) { return false; }
	bool IsNoRightToAccess(void) const noexcept { return m_iReceivedDataStatus == NO_ACCESS_RIGHT_; }

	CString GetClassName(void) const noexcept { return m_strClassName; }
	CString GetInquiry(void) const noexcept { return m_strInquiry; }
	CString GetInquiryFunction(void) const noexcept { return m_strInquiryFunction; }
	void SetInquiryFunction(CString strMessage) noexcept { m_strInquiryFunction = strMessage; }

	long GetIndex(void) const noexcept { return m_lIndex; }
	void SetIndex(const long lIndex) noexcept { m_lIndex = lIndex; }

	[[nodiscard]] CVirtualMarket* GetMarket(void) const noexcept { return m_pMarket; }
	void SetMarket(CVirtualMarket* pMarket) noexcept { m_pMarket = pMarket; }

	void SetInquiringExchange(const CString& exchange) noexcept { m_strInquiringExchange = exchange; }
	CString GetInquiringExchange(void) const noexcept { return m_strInquiringExchange; }
	bool IsUSMarket(void); // ����������г�

	void SetProductType(const int iProductType) noexcept { m_iProductType = iProductType; }
	int GetProductType(void) const noexcept { return m_iProductType; }

protected:
	CVirtualMarket* m_pMarket; // �ò�Ʒ�������г�
	CString m_strClassName;
	CString m_strInquiryFunction;
	CString m_strInquiry;
	CString m_strInquiringExchange; // Ŀǰ��ѯ�Ľ���������
	long m_lIndex;
	int m_iProductType;
	int m_iReceivedDataStatus;
	// 0:��Ч���ݣ�1:void data(ֻ��{}��������); 2:û��Ȩ�����루{"error": "You don't have access to this resource."}��
};

using CVirtualWebProductPtr = shared_ptr<CVirtualWebProduct>;

// �˽ṹֻ���ڲ�����
struct FinnhubWebData {
	FinnhubWebData(const long lIndex, const CString& strSymbol, const CString& strData) {
		m_lIndex = lIndex;
		m_strSymbol = strSymbol;
		m_pData = make_shared<CWebData>();
		m_pData->Test_SetBuffer_(strData);
	}

	~FinnhubWebData() = default;

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
		m_pData->Test_SetBuffer_(strData);
	}

	~TiingoWebData() { }

public:
	long m_lIndex;
	CString m_strSymbol;
	CWebDataPtr m_pData;
};
