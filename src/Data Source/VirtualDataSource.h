#pragma once

#include"VirtualWebProduct.h"
#include "InquireEngine.h"

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
	void RunWorkingThread(long lMarketTime);
	virtual bool GenerateInquiryMessage(const long) { return true; } // �̳������ʵ�ָ��ԵĲ�ѯ����. ����Ϊ��ǰ�г�ʱ�䣨hhmmss��
	virtual void GenerateCurrentInquiryMessage();
	virtual void CheckInaccessible(const CWebDataPtr&) const {
	}

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

	virtual ULONGLONG GetTickCount() { return ::GetTickCount64(); } // Ϊ�˲��Է��㣬��GetTickCount64������һ�㡣

	// ��״̬
	CVirtualProductWebDataPtr GetCurrentInquiry() const noexcept { return m_pCurrentProduct; }
	void SetCurrentInquiry(const CVirtualProductWebDataPtr& p) { m_pCurrentProduct = p; }

	size_t InquiryQueueSize() const noexcept { return m_qProduct.size(); }
	void StoreInquiry(const CVirtualProductWebDataPtr& p) { m_qProduct.push(p); }
	CVirtualProductWebDataPtr GetCurrentProduct() {
		m_pCurrentProduct = m_qProduct.front();
		m_qProduct.pop();
		return m_pCurrentProduct;
	}
	bool HaveInquiry() const { return !m_qProduct.empty(); }
	void DiscardAllInquiry() { while (m_qProduct.size() > 0) m_qProduct.pop(); }

	bool IsInquiring() const noexcept { return m_fInquiring; }
	void SetInquiring(const bool fFlag) noexcept { m_fInquiring = fFlag; }

	bool IsWorkingThreadRunning() const noexcept { return m_bIsWorkingThreadRunning; }
	void SetWorkingThreadRunning(const bool fFlag) noexcept { m_bIsWorkingThreadRunning = fFlag; }

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

	bool IsWebError() const noexcept { return m_fWebError; }
	void SetWebError(bool fFlag) noexcept { m_fWebError = fFlag; }
	DWORD GetWebErrorCode() const noexcept { return m_dwWebErrorCode.load(); }
	void SetWebErrorCode(INT64 dwErrorCode) noexcept { m_dwWebErrorCode = dwErrorCode; }

	long GetInquiringNumber() const noexcept { return m_lInquiringNumber; }
	void SetInquiringNumber(const long lValue) noexcept { m_lInquiringNumber = lValue; }

	static long GetTotalByteRead() noexcept { return sm_lTotalByteRead.load(); }
	static void SetTotalByteRead(const long lValue) noexcept { sm_lTotalByteRead = lValue; }
	static void ClearTotalByteRead() noexcept { sm_lTotalByteRead = 0; }
	static long GetTotalByteReadPerSecond() noexcept { return sm_lTotalByteReadPerSecond.load(); }
	static void CalcTotalBytePerSecond() { sm_lTotalByteReadPerSecond = sm_lTotalByteRead.exchange(0); }

	void SetCurrentInquiryTime(const time_t tt) noexcept { m_tCurrentInquiryTime = tt; }
	virtual time_t GetCurrentInquiryTime() const noexcept { return m_tCurrentInquiryTime.load(); }

	void SetWebBusy(bool busy) noexcept { m_bWebBusy = busy; }
	bool IsWebBusy() const noexcept { return m_bWebBusy.load(); }

protected:
	queue<CVirtualProductWebDataPtr, list<CVirtualProductWebDataPtr>> m_qProduct; // �����ѯ�������
	CVirtualProductWebDataPtr m_pCurrentProduct;

	bool m_fWebError; //�����ȡ����
	atomic_int64_t m_dwWebErrorCode{0}; // ���������

	InternetOption m_internetOption;
	CString m_strInquiry; // ��ѯ������ַ�����m_strInquiryFunction + m_strParam + m_strSuffix + m_strInquiryToken).
	CString m_strInquiryFunction; // ��ѯ�ַ������ܲ���
	CString m_strParam; // ��ѯ�ַ����Ĳ���
	CString m_strSuffix; // ��ѯ�ַ����ĺ�׺����
	CString m_strInquiryToken; // ��ѯ�ַ�������
	CString m_strHeaders; // OpenURLʱ��headers�ַ���ֵ�� Ĭ��Ϊ_T("")

	long m_lInquiringNumber; // ÿ�β�ѯ����
	int m_iMaxNormalInquireTime{300}; // ���������ѯʱ�䣨ÿ�����������Դ�Բ�ͬ��
	atomic_int64_t m_tCurrentInquiryTime; // ��ǰ������������ʱ�䣨�Ժ���ƣ�

	static atomic_long sm_lTotalByteRead; // ��ǰ�����ȡ�ֽ��������е������ȡ�����޸Ĵ˱������ʶ�����Ϊ��̬��
	static atomic_long sm_lTotalByteReadPerSecond; // 

	long long m_llLastTimeTickCount;

	atomic_bool m_fEnable; // ����ִ�б�ʶ
	atomic_bool m_fInquiring;
	atomic_bool m_bIsWorkingThreadRunning;
	atomic_bool m_bWebBusy{false};
};

using CVirtualDataSourcePtr = shared_ptr<CVirtualDataSource>;
