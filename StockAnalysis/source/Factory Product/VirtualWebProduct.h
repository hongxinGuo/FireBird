#pragma once

#include<memory>

#include"ClassDeclaration.h"

#include"WebData.h"

enum {
	_VOID_DATA_ = 1,
	_NO_ACCESS_RIGHT_ = 2,
};

class CVirtualWebProduct : public CObject {
public:
	DECLARE_DYNCREATE(CVirtualWebProduct)
	CVirtualWebProduct();
	virtual ~CVirtualWebProduct() override = default;

	// ������ҪDECLARE_DYNCREATE���࣬�ʶ��޷���CreateMessage��ParseAndStoreWebData����Ϊ���麯����
	virtual CString CreateMessage(void) { return _T(""); }
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData) { return true; }
	virtual bool AddInaccessibleExchangeIfNeeded(void) { return true; } // ����Ƿ���������������ݣ���ʹ������˻�ʱ������Դ������ʹ����ĳЩ���ܣ�

	bool IsVoidJson(const CWebDataPtr& pWebData) {
		if (pWebData->IsVoidJson()) {
			m_iReceivedDataStatus = _VOID_DATA_;
			return true;
		}
		else return false;
	}

	bool IsVoidData(void) const noexcept { return m_iReceivedDataStatus == _VOID_DATA_; }
	virtual bool CheckNoRightToAccess(CWebDataPtr pWebData) { return false; }
	bool IsNoRightToAccess(void) const noexcept { return m_iReceivedDataStatus == _NO_ACCESS_RIGHT_; }

	CString GetClassName(void) const noexcept { return m_strClassName; }
	CString GetInquiry(void) const noexcept { return m_strInquiry; }

	long GetIndex(void) const noexcept { return m_lIndex; }
	void SetIndex(long const lIndex) noexcept { m_lIndex = lIndex; }

	[[nodiscard]] CVirtualMarket* GetMarket(void) const noexcept { return m_pMarket; }
	void SetMarket(CVirtualMarket* pMarket) noexcept { m_pMarket = pMarket; }

	void SetInquiringExchange(CString exchange) noexcept { m_strInquiringExchange = exchange; }
	CString GetInquiringExchange(void) const noexcept { return m_strInquiringExchange; }
	bool IsUSMarket(void); // ����������г�

	void SetProductType(int iProductType) noexcept { m_iProductType = iProductType; }
	int GetProductType(void) const noexcept { return m_iProductType; }

protected:
	CVirtualMarket* m_pMarket; // �ò�Ʒ�������г�
	CString m_strClassName;
	CString m_strInquiry;
	CString m_strTotalInquiryMessage;
	CString m_strInquiringExchange; // Ŀǰ��ѯ�Ľ���������
	long m_lIndex;
	int m_iProductType;
	int m_iReceivedDataStatus;
	// 0:��Ч���ݣ�1:void data(ֻ��{}��������); 2:û��Ȩ�����루{"error": "You don't have access to this resource."}��
};

typedef shared_ptr<CVirtualWebProduct> CVirtualProductWebDataPtr;

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

	~TiingoWebData() {
	}

public:
	long m_lIndex;
	CString m_strSymbol;
	CWebDataPtr m_pData;
};
