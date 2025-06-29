//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// �������ݵ����룬����Factory-Productģʽ�������ݶ�Ӧ��Ӧ��Product��
/// Product��������������ݵ���װ�����ص��������ݵĽ����ʹ洢��
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"ClassDeclaration.h"

enum {
	GOOD_DATA__ = 1,
	VOID_DATA_,
	NO_ACCESS_RIGHT_,
};

[[nodiscard]] bool CompareDayLineDate(const CDayLinePtr& p1, const CDayLinePtr& p2);

class CVirtualWebProduct {
public:
	CVirtualWebProduct() {} // default do nothing
	virtual ~CVirtualWebProduct() = default;

	virtual string CreateMessage() { return _T(""); };
	virtual void CalculateTotalDataLength(shared_ptr<vector<CWebDataPtr>>) {}

	virtual void ParseAndStoreWebData(CWebDataPtr) {} // default do nothing
	virtual void ParseAndStoreWebData(shared_ptr<vector<CWebDataPtr>> pvWebData) {// һ�δ��������յ������ݡ�Ŀǰֻ����Ѷ����������Ҫ����ģʽ
		ASSERT(pvWebData->size() == 1);
		ParseAndStoreWebData(pvWebData->at(0)); // Ĭ��ֻ��һ�����ݣ�
	}
	virtual void AddInaccessibleSymbol() {} // ����Ƿ���������������ݣ���ʹ������˻�ʱ������Դ������ʹ����ĳЩ���ܣ�
	virtual void UpdateDataSourceStatus(CVirtualDataSourcePtr) {} // default do nothing

	bool CheckInaccessible();
	bool IsVoidJson(const CWebDataPtr& pWebData);

	bool IsVoidData() const noexcept { return m_iReceivedDataStatus == VOID_DATA_; }
	bool IsNoRightToAccess() const noexcept { return m_iReceivedDataStatus == NO_ACCESS_RIGHT_; }
	int GetReceivedDataStatus() const noexcept { return m_iReceivedDataStatus; }
	void SetReceivedDataStatus(int iType) noexcept { m_iReceivedDataStatus = iType; }

	string GetInquiry() const noexcept { return m_strInquiry; }
	string GetInquiryFunction() const noexcept { return m_strInquiryFunction; }
	void SetInquiryFunction(const string& strMessage) noexcept { m_strInquiryFunction = strMessage; }

	auto GetIndex() const noexcept { return m_lIndex; }
	void SetIndex(const size_t lIndex) noexcept { m_lIndex = lIndex; }

	void SetMarket(const CVirtualMarketPtr& pMarket) noexcept { m_pMarket = pMarket; }

	void SetInquiringExchange(const CString& exchange) noexcept { m_strInquiringExchange = exchange; }
	string GetInquiringExchange() const noexcept { return m_strInquiringExchange; }
	bool IsUSMarket() const; // ����������г�

	void SetInquiringSymbol(const string& symbol) noexcept { m_strInquiringSymbol = symbol; }
	string GetInquiringSymbol() const noexcept { return m_strInquiringSymbol; }

	void SetInquireType(const int iInquireType) noexcept { m_iInquireType = iInquireType; }
	int GetInquireType() const noexcept { return m_iInquireType; }

	// ������
	virtual bool __Test_checkAccessRight(CWebDataPtr) { return true; }  // todo ����ʹ�ã�׼��ɾ��֮

protected:
	CVirtualMarketWeakPtr m_pMarket;// Product�����ڹ����߳��С���ϵͳ�˳�ʱ�������޷���֤�����߳��Ƚ������ʶ��˴�ʹ��weak_ptr����ָ���Է�ֹ�ڴ�й¶��
	string m_strInquiryFunction{ _T("") };
	string m_strInquiry{ _T("") };
	string m_strInquiringExchange{ _T("ALL") }; // Ĭ�����������еĽ��������ݡ�
	string m_strInquiringSymbol{ _T("") }; // Ŀǰ��ѯ��֤ȯ����
	size_t m_lIndex{ 0 }; // ���鴦���productΪһ�ۺ�ʱ������������� Ԥ������ΪԽ��
	int m_iInquireType{ -1 }; // product������Finnhub�������������SYMBOL_LOOKUP_�ȡ� Ԥ������ΪԽ��
	int m_iReceivedDataStatus{ GOOD_DATA__ }; // 1:��Ч���ݣ�2:void data(ֻ��{}��[]��������); 3:û��Ȩ������
};

using CVirtualWebProductPtr = shared_ptr<CVirtualWebProduct>;
