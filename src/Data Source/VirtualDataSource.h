#pragma once

#include "InternetOption.h"
#include"VirtualWebProduct.h"
#include "WorldMarket.h"

enum enum_ErrorMessageData {
	ERROR_NO_ERROR__ = 0,
	// finnhub
	ERROR_FINNHUB_NO_RIGHT_TO_ACCESS__,
	ERROR_FINNHUB_MISSING_API_KEY__,
	ERROR_FINNHUB_REACH_MAX_API_LIMIT__,
	ERROR_FINNHUB_INQUIRE_RATE_TOO_HIGH__,
	ERROR_FINNHUB_NOT_HANDLED__,

	// tiingo
	ERROR_TIINGO_NO_RIGHT_TO_ACCESS__,
	ERROR_TIINGO_MISSING_API_KEY__,
	ERROR_TIINGO_INQUIRE_RATE_TOO_HIGH__,
	ERROR_TIINGO_ADD_ON_PERMISSION_NEEDED__,
	ERROR_TIINGO_FREQUENCY__,
	ERROR_TIINGO_REACH_MAX_SYMBOL_LIMIT__,
	ERROR_TIINGO_REACH_MAX_BANDWIDTH_LIMIT__,
	ERROR_TIINGO_REACH_MAX_API_LIMIT__,
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

	template <typename UpdateCheck, typename ProductFactory, typename ReportMsg>
	bool GenerateSimpleInquiry(int inquireType, UpdateCheck&& isUpdateNeeded, ProductFactory&& createProduct, ReportMsg&& reportMsg) {
		ASSERT(!IsInquiring());
		if (isUpdateNeeded()) {
			StoreInquiry(createProduct(inquireType));
			SetInquiring(true);
			reportMsg();
			return true;
		}
		return false;
	}

	template <typename UpdateCheck, typename ValidDate, typename SetUpdated, typename ReportMsg1, typename ProductFactory, typename ReportMsg>
	bool GenerateSimpleInquiryWithCheck(int inquireType, ValidDate validDate, SetUpdated setUpdated, ReportMsg1 reportMsg1,
	                                    UpdateCheck&& isUpdateNeeded, ProductFactory&& createProduct, ReportMsg&& reportMsg) {
		ASSERT(!IsInquiring());
		if (isUpdateNeeded()) {
			if (!validDate()) {
				setUpdated();
				reportMsg1();
				return false;
			}
			StoreInquiry(createProduct(inquireType));
			SetInquiring(true);
			reportMsg();
			return true;
		}
		return false;
	}

	template <typename Container, typename IsNeedUpdate, typename IsItemNeedUpdate, typename IsAccessible, typename CreateProduct, typename SetIndex, typename SetMessage, typename SetUpdateFlag>
	bool GenerateInquiryIterateWithAccessCheck(
		Container& container,
		int inquireType,
		IsNeedUpdate isNeedUpdate,
		IsItemNeedUpdate isItemNeedUpdate,
		IsAccessible isAccessible,
		CreateProduct createProduct,
		SetIndex setIndex,
		SetMessage setMessage,
		SetUpdateFlag setUpdateFlag,
		const std::string& finishedMsg
	) {
		const auto size = container.Size();
		bool haveInquiry = false;

		if (isNeedUpdate()) {
			bool found = false;
			long pos;
			for (pos = 0; pos < size; ++pos) {
				auto item = container.GetItem(pos);
				if (isItemNeedUpdate(item) && isAccessible(inquireType, container.GetItemExchangeCode(pos))) {
					found = true;
					break;
				}
			}
			if (found) {
				auto product = createProduct(inquireType);
				setIndex(product, pos);
				StoreInquiry(product);
				SetInquiring(true);
				setMessage(container.GetItem(pos));
				haveInquiry = true;
			}
			else {
				setUpdateFlag(false);
				gl_systemMessage.PushInformationMessage(finishedMsg);
				haveInquiry = false;
			}
		}
		return haveInquiry;
	}

	std::shared_ptr<CVirtualDataSource> GetShared() { return shared_from_this(); }

	virtual bool Reset() { return true; }

	void Run(long lMarketTime);
	void InquireData();
	virtual bool GenerateInquiryMessage(const long) { return true; } // �̳������ʵ�ָ��ԵĲ�ѯ����. ����Ϊ��ǰ�г�ʱ�䣨hhmmss��
	virtual void CreateCurrentInquireString();
	virtual enum_ErrorMessageData IsAErrorMessageData(const CWebDataPtr&) { return ERROR_NO_ERROR__; } // ��WebData����Ϊ������Ϣ��

	void SetDefaultSessionOption();

	virtual void ConfigureInternetOption() {
		ASSERT(false); // �����˻��ຯ��ConfigureInternetOption
	} // ����internet�������̳������ʵ�ִ˹��ܣ�ÿ����վ��״̬����һ�����ʶ���Ҫ�������á�
	virtual void UpdateStatus(CWebDataPtr) {} //�ɹ����պ����ϵͳ״̬��

	void CreateTotalInquiringString();
	string GetInquiringString() const noexcept { return m_strInquiry; }
	void SetInquiringString(const string& str) noexcept { m_strInquiry = str; }
	void AppendInquiringString(const string& str) noexcept { m_strInquiry += str; }

	virtual chrono::time_point<chrono::steady_clock> GetTickCount() { return chrono::steady_clock::now(); } // Ϊ�˲��Է��㣬��GetTickCount64������һ�㡣

	// ��״̬
	CVirtualProductWebDataPtr GetCurrentInquiry() const noexcept { return m_pCurrentProduct; }
	void SetCurrentInquiry(const CVirtualProductWebDataPtr& p) { m_pCurrentProduct = p; }

	size_t InquiryQueueSize() const noexcept { return m_qProduct.size(); }
	void DiscardCurrentInquiry() { m_qProduct.pop(); }
	void DiscardAllInquiry() { while (m_qProduct.size() > 0) m_qProduct.pop(); }
	void StoreInquiry(const CVirtualProductWebDataPtr& p) { m_qProduct.push(p); }
	CVirtualProductWebDataPtr PeekFrontProduct() { return m_qProduct.front(); }
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

	string GetHeaders() const noexcept { return m_strHeaders; }
	void SetHeaders(const string& strHeaders) noexcept { m_strHeaders = strHeaders; }

	string GetInquiryFunction() const noexcept { return m_strInquiryFunction; }
	void SetInquiryFunction(const string& strPrefix) noexcept { m_strInquiryFunction = strPrefix; }
	string GetInquirySuffix() const noexcept { return m_strSuffix; }
	void SetInquirySuffix(const string& strSuffix) noexcept { m_strSuffix = strSuffix; }
	string GetInquiryToken() const noexcept { return m_strInquiryToken; }
	void SetInquiryToken(const string& strToken) noexcept { m_strInquiryToken = strToken; }

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
	queue<CVirtualProductWebDataPtr> m_qProduct; // �����ѯ�������
	CVirtualProductWebDataPtr m_pCurrentProduct{ nullptr };

	atomic_int64_t m_dwHTTPStatusCode{ 0 }; // ����״̬��
	atomic_int64_t m_dwWebErrorCode{ 0 }; // ���������
	enum_ErrorMessageData m_eErrorMessageData{ ERROR_NO_ERROR__ };

	InternetOption m_internetOption;
	string m_strInquiry{ _T("") }; // ��ѯ������ַ�����m_strInquiryFunction + m_strParam + m_strSuffix + m_strInquiryToken).
	string m_strInquiryFunction{ _T("") }; // ��ѯ�ַ������ܲ���
	string m_strParam{ _T("") }; // ��ѯ�ַ����Ĳ���
	string m_strSuffix{ _T("") }; // ��ѯ�ַ����ĺ�׺����
	string m_strInquiryToken{ _T("") }; // ��ѯ�ַ�������
	string m_strHeaders{ _T("") }; // OpenURLʱ��headers�ַ���ֵ�� Ĭ��Ϊ_T("")

	long m_lInquiringNumber{ 500 }; // ÿ�β�ѯ����Ĭ��ֵΪ500
	int m_iMaxNormalInquireTime{ 500 }; // ���������ѯʱ�䣨ÿ�����������Դ�Բ�ͬ��
	atomic_int64_t m_tCurrentInquiryTime{ 0 }; // ��ǰ������������ʱ�䣨�Ժ���ƣ�

	static atomic<int64_t> sm_lTotalByteRead; // ��ǰ�����ȡ�ֽ��������е������ȡ�����޸Ĵ˱������ʶ�����Ϊ��̬��
	static atomic<int64_t> sm_lTotalByteReadPerSecond; // 

	chrono::time_point<chrono::steady_clock> m_PrevInquireTimePoint{};

	atomic_bool m_fInquiring{ false };
	atomic_bool m_fEnable{ true }; // ����ִ�б�ʶ
	atomic_bool m_bIsWorkingThreadRunning{ false };
	atomic_bool m_bWebBusy{ false };
};

using CVirtualDataSourcePtr = shared_ptr<CVirtualDataSource>;
