//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// �������ݵ����룬����Factory-Productģʽ�������ݶ�Ӧ��Ӧ��Product��
/// Product��������������ݵ���װ�����ص��������ݵĽ����ʹ洢��
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"ClassDeclaration.h"
#include"WebData.h"

//#include "VirtualMarket.h"

using std::make_shared;

enum {
	VOID_DATA_ = 1,
	NO_ACCESS_RIGHT_ = 2,
};

[[nodiscard]] bool CompareDayLineDate(const CDayLinePtr& p1, const CDayLinePtr& p2);

class CVirtualWebProduct {
public:
	CVirtualWebProduct();
	virtual ~CVirtualWebProduct() = default;

	virtual CString CreateMessage() { return _T(""); };
	virtual bool ParseAndStoreWebData(CWebDataPtr pWebData) { return true; };
	virtual void AddInaccessibleExchange() { } // ����Ƿ���������������ݣ���ʹ������˻�ʱ������Դ������ʹ����ĳЩ���ܣ�

	virtual void UpdateDataSourceStatus(CVirtualDataSourcePtr pDataSource) {} // default do nothing

	bool CheckInaccessible(const CWebDataPtr& pWebData);
	bool CheckVoidJson(const CWebDataPtr& pWebData);

	bool IsVoidData() const noexcept { return m_iReceivedDataStatus == VOID_DATA_; }
	virtual bool CheckNoRightToAccess(CWebDataPtr pWebData) { return false; }
	bool IsNoRightToAccess() const noexcept { return m_iReceivedDataStatus == NO_ACCESS_RIGHT_; }

	CString GetClassName() const noexcept { return m_strClassName; }
	CString GetInquiry() const noexcept { return m_strInquiry; }
	CString GetInquiryFunction() const noexcept { return m_strInquiryFunction; }
	void SetInquiryFunction(const CString& strMessage) noexcept { m_strInquiryFunction = strMessage; }

	long GetIndex() const noexcept { return m_lIndex; }
	void SetIndex(const long lIndex) noexcept { m_lIndex = lIndex; }

	void SetMarket(const CVirtualMarketPtr& pMarket) noexcept { m_pMarket = pMarket; }

	[[nodiscard]] shared_ptr<CVirtualMarket> GetMarket() const noexcept {
		if (CVirtualMarketPtr p = m_pMarket.lock()) {
			return p;
		}
		//exit(1);  // todo ������Ϊnullptrʱ����ʱϵͳ���߳��Ѿ��˳��ˣ������߳�ҲҪ�����˳�
		return nullptr;
	}

	void SetInquiringExchange(const CString& exchange) noexcept { m_strInquiringExchange = exchange; }
	CString GetInquiringExchange() const noexcept { return m_strInquiringExchange; }
	bool IsUSMarket() const; // ����������г�

	void SetProductType(const int iProductType) noexcept { m_iProductType = iProductType; }
	int GetProductType() const noexcept { return m_iProductType; }

protected:
	CVirtualMarketWeakPtr m_pMarket;// Product�����ڹ����߳��С���ϵͳ�˳�ʱ�������޷���֤�����߳��Ƚ������ʶ��˴�ʹ��Weak����ָ���Է�ֹ�ڴ�й¶��
	CString m_strClassName;
	CString m_strInquiryFunction;
	CString m_strInquiry;
	CString m_strInquiringExchange; // Ŀǰ��ѯ�Ľ���������
	long m_lIndex;
	int m_iProductType;
	int m_iReceivedDataStatus; // 0:��Ч���ݣ�1:void data(ֻ��{}��������); 2:û��Ȩ�����루{"error": "You don't have access to this resource."}��
};

using CVirtualWebProductPtr = shared_ptr<CVirtualWebProduct>;

// �˽ṹֻ���ڲ�����
struct Test_FinnhubWebData {
	Test_FinnhubWebData(const long lIndex, const CString& strSymbol, const CString& strData) {
		m_lIndex = lIndex;
		m_strSymbol = strSymbol;
		m_pData = make_shared<CWebData>();
		m_pData->Test_SetBuffer_(strData);
	}

	~Test_FinnhubWebData() = default;

public:
	long m_lIndex;
	CString m_strSymbol;
	CWebDataPtr m_pData;
};

extern Test_FinnhubWebData finnhubWebData0;
extern Test_FinnhubWebData finnhubWebData1;

// �˽ṹֻ���ڲ�����
struct Test_TiingoWebData {
	Test_TiingoWebData(const long lIndex, const CString& strSymbol, const CString& strData) {
		m_lIndex = lIndex;
		m_strSymbol = strSymbol;
		m_pData = make_shared<CWebData>();
		m_pData->Test_SetBuffer_(strData);
	}

	~Test_TiingoWebData() { }

public:
	long m_lIndex;
	CString m_strSymbol;
	CWebDataPtr m_pData;
};
