#pragma once

#include"VirtualWebProduct.h"
#include "InquireEngine.h"

enum enum_ErrorMessageData {
	ERROR_NO_ERROR__ = 0,
	// finnhub
	ERROR_FINNHUB_NO_RIGHT_TO_ACCESS__,
	ERROR_FINNHUB_MISSING_API_KEY__,
	ERROR_FINNHUB_INQUIRE_RATE_TOO_HIGH__,
	ERROR_FINNHUB_NOT_HANDLED__,

	// tiingo
	ERROR_TIINGO_NO_RIGHT_TO_ACCESS__,
	ERROR_TIINGO_MISSING_API_KEY__,
	ERROR_TIINGO_INQUIRE_RATE_TOO_HIGH__,
	ERROR_TIINGO_ADD_ON_PERMISSION_NEEDED__,
	ERROR_TIINGO_FREQUENCY__,
	ERROR_TIINGO_REACH_MAX_SYMBOL_LIMIT__,
	ERROR_TIINGO_ENDPOINT_ONLY_FOR_US_LISTED_STOCK__,
	ERROR_TIINGO_NOT_FOUND__,
	ERROR_TIINGO_NOT_HANDLED__,

	// sina
	ERROR_SINA_HEADER_NEEDED__, // 
};

class CVirtualDataSource : public std::enable_shared_from_this<CVirtualDataSource> {
	friend CVirtualWebProduct;

public:
	CVirtualDataSource();
	// ֻ����һ��ʵ��,������ֵ��
	CVirtualDataSource(const CVirtualDataSource&) = delete;
	CVirtualDataSource& operator=(const CVirtualDataSource&) = delete;
	CVirtualDataSource(const CVirtualDataSource&&) noexcept = delete;
	CVirtualDataSource& operator=(const CVirtualDataSource&&) noexcept = delete;
	virtual ~CVirtualDataSource() = default;

	std::shared_ptr<CVirtualDataSource> GetShared() { return shared_from_this(); }

	virtual bool Reset() { return true; }

	void Run(long lMarketTime);
	shared_ptr<vector<CWebDataPtr>> InquireData();
	virtual bool GenerateInquiryMessage(const long) { return true; } // �̳������ʵ�ָ��ԵĲ�ѯ����. ����Ϊ��ǰ�г�ʱ�䣨hhmmss��
	virtual void CreateCurrentInquireString();
	virtual enum_ErrorMessageData IsAErrorMessageData(const CWebDataPtr&) { return ERROR_NO_ERROR__; } // ��WebData����Ϊ������Ϣ��

	void SetDefaultSessionOption();

	virtual void ConfigureInternetOption() {
		ASSERT(false); // �����˻��ຯ��ConfigureInternetOption
	} // ����internet�������̳������ʵ�ִ˹��ܣ�ÿ����վ��״̬����һ�����ʶ���Ҫ�������á�
	virtual void UpdateStatus(CWebDataPtr pData) {
	} //�ɹ����պ����ϵͳ״̬��

	void CreateTotalInquiringString();
	CString GetInquiringString() const noexcept { return m_strInquiry; }
	void SetInquiringString(const CString& str) noexcept { m_strInquiry = str; }
	void AppendInquiringString(const CString& str) noexcept { m_strInquiry += str; }

	virtual chrono::time_point<chrono::steady_clock> GetTickCount() { return chrono::steady_clock::now(); } // Ϊ�˲��Է��㣬��GetTickCount64������һ�㡣

	// ��״̬
	CVirtualProductWebDataPtr GetCurrentInquiry() const noexcept { return m_pCurrentProduct; }
	void SetCurrentInquiry(const CVirtualProductWebDataPtr& p) { m_pCurrentProduct = p; }

	size_t InquiryQueueSize() const noexcept { return m_qProduct.size(); }
	void DiscardCurrentInquiry() { m_qProduct.pop(); }
	void DiscardAllInquiry() { while (m_qProduct.size() > 0) m_qProduct.pop(); }
	void StoreInquiry(const CVirtualProductWebDataPtr& p) { m_qProduct.push(p); }
	CVirtualProductWebDataPtr GetFrontProduct() { return m_qProduct.front(); }
	CVirtualProductWebDataPtr GetCurrentProduct() {
		m_pCurrentProduct = m_qProduct.front();
		m_qProduct.pop();
		return m_pCurrentProduct;
	}
	bool HaveInquiry() const { return !m_qProduct.empty(); }

	bool IsInquiring() const noexcept { return m_fInquiring; }
	void SetInquiring(const bool fFlag) noexcept { m_fInquiring = fFlag; }

	bool IsEnable() const noexcept { return m_fEnable; }
	void Enable(const bool fFlag) noexcept { m_fEnable = fFlag; }

	CString GetHeaders() const noexcept { return m_strHeaders; }
	void SetHeaders(const CString& strHeaders) noexcept { m_strHeaders = strHeaders; }

	CString GetInquiryFunction() const noexcept { return m_strInquiryFunction; }
	void SetInquiryFunction(const CString& strPrefix) noexcept { m_strInquiryFunction = strPrefix; }
	CString GetInquirySuffix() const noexcept { return m_strSuffix; }
	void SetInquirySuffix(const CString& strSuffix) noexcept { m_strSuffix = strSuffix; }
	CString GetInquiryToken() const noexcept { return m_strInquiryToken; }
	void SetInquiryToken(const CString& strToken) noexcept { m_strInquiryToken = strToken; }

	bool IsWebError() const noexcept { return m_dwWebErrorCode != 0; }
	void SetWebError(bool fFlag) noexcept {
		if (fFlag) m_dwWebErrorCode = 1;
		else m_dwWebErrorCode = 0;
	}
	auto GetWebErrorCode() const noexcept { return m_dwWebErrorCode.load(); }
	void SetWebErrorCode(INT64 dwErrorCode) noexcept { m_dwWebErrorCode = dwErrorCode; }

	auto GetHTTPStatusCode() const noexcept { return m_dwHTTPStatusCode.load(); }
	void SetHTTPStatusCode(INT64 dwStatusCode) noexcept { m_dwHTTPStatusCode = dwStatusCode; }

	long GetInquiringNumber() const noexcept { return m_lInquiringNumber; }
	void SetInquiringNumber(const long lValue) noexcept { m_lInquiringNumber = lValue; }

	static auto GetTotalByteRead() noexcept { return sm_lTotalByteRead.load(); }
	static void SetTotalByteRead(const long lValue) noexcept { sm_lTotalByteRead = lValue; }
	static void ClearTotalByteRead() noexcept { sm_lTotalByteRead = 0; }
	static auto GetTotalByteReadPerSecond() noexcept { return sm_lTotalByteReadPerSecond.load(); }
	static void CalcTotalBytePerSecond() { sm_lTotalByteReadPerSecond = sm_lTotalByteRead.exchange(0); }

	void SetCurrentInquiryTime(const time_t tt) noexcept { m_tCurrentInquiryTime = tt; }
	virtual time_t GetCurrentInquiryTime() const noexcept { return m_tCurrentInquiryTime.load(); }

	void SetWebBusy(bool busy) noexcept { m_bWebBusy = busy; }
	bool IsWebBusy() const noexcept { return m_bWebBusy.load(); }

	virtual void ReportErrorNotHandled(const string& sError);

	void SetErrorMessage(enum_ErrorMessageData error) { m_eErrorMessageData = error; }

protected:
	queue<CVirtualProductWebDataPtr, list<CVirtualProductWebDataPtr>> m_qProduct; // �����ѯ�������
	CVirtualProductWebDataPtr m_pCurrentProduct{ nullptr };

	bool m_fWebError{ false }; //�����ȡ����
	atomic_int64_t m_dwHTTPStatusCode{ 0 }; // ����״̬��
	atomic_int64_t m_dwWebErrorCode{ 0 }; // ���������
	enum_ErrorMessageData m_eErrorMessageData{ ERROR_NO_ERROR__ };

	InternetOption m_internetOption;
	CString m_strInquiry{ _T("") }; // ��ѯ������ַ�����m_strInquiryFunction + m_strParam + m_strSuffix + m_strInquiryToken).
	CString m_strInquiryFunction{ _T("") }; // ��ѯ�ַ������ܲ���
	CString m_strParam{ _T("") }; // ��ѯ�ַ����Ĳ���
	CString m_strSuffix{ _T("") }; // ��ѯ�ַ����ĺ�׺����
	CString m_strInquiryToken{ _T("") }; // ��ѯ�ַ�������
	CString m_strHeaders{ _T("") }; // OpenURLʱ��headers�ַ���ֵ�� Ĭ��Ϊ_T("")

	long m_lInquiringNumber{ 500 }; // ÿ�β�ѯ����Ĭ��ֵΪ500
	int m_iMaxNormalInquireTime{ 300 }; // ���������ѯʱ�䣨ÿ�����������Դ�Բ�ͬ��
	atomic_int64_t m_tCurrentInquiryTime{ 0 }; // ��ǰ������������ʱ�䣨�Ժ���ƣ�

	static atomic_int64_t sm_lTotalByteRead; // ��ǰ�����ȡ�ֽ��������е������ȡ�����޸Ĵ˱������ʶ�����Ϊ��̬��
	static atomic_int64_t sm_lTotalByteReadPerSecond; // 

	chrono::time_point<chrono::steady_clock> m_PrevTimePoint;

	atomic_bool m_fInquiring{ false };
	atomic_bool m_fEnable{ true }; // ����ִ�б�ʶ
	atomic_bool m_bIsWorkingThreadRunning{ false };
	atomic_bool m_bWebBusy{ false };
};

using CVirtualDataSourcePtr = shared_ptr<CVirtualDataSource>;
