#include"pch.h"
module;
#include <afxinet.h>
#include"concurrentqueue/concurrentqueue.h"
#include"VirtualWebProduct.h"
#include"nlohmann/json_fwd.hpp"
#include"WebData.h"
export module FireBird.DataSource.InquireEngine;

using namespace moodycamel;
using std::string;

export {
	constexpr auto WEB_SOURCE_DATA_BUFFER_SIZE_ = 1024 * 16;

	struct InternetOption {
	public:
		int option_connect_retries{ 1 };
		int option_connect_timeout{ 5 };
		int option_receive_timeout{ 5000 };
		int option_data_receive_timeout{ 12000 };
		int option_send_timeout{ 2000 };
	};


	class CInquireEngine {
		friend CVirtualWebProduct;

	public:
		CInquireEngine();
		CInquireEngine(const InternetOption& internetOption, const CString& strInquire, const CString& strHeaders);
		virtual ~CInquireEngine() {
		}

		void ConfigureSession(const InternetOption& option) const;
		CWebDataPtr GetWebData(); // ʵ�ʶ�ȡ������
		virtual void OpenFile(); // �޷����ԣ��ʶ����⻯��ʹ��Mock�������ԡ�
		virtual void GetFileHeaderInformation(); // �޷����ԣ��ʶ����⻯��ʹ��Mock�������ԡ�
		void DeleteWebFile();
		void QueryDataLength();
		virtual UINT ReadWebFileOneTime(); // �޷����ԣ��ʶ����⻯��ʹ��Mock�������ԡ�
		void XferReadingToBuffer(UINT uByteRead);
		bool IncreaseBufferSizeIfNeeded(long lIncreaseSize);
		CWebDataPtr CreateWebData();

		void VerifyDataLength() const;
		void TransferDataToWebData(const CWebDataPtr& pWebData); // �����յ���������pWebData��

		long GetByteRead() const noexcept { return m_lByteRead; }
		void SetByteRead(long number) { m_lByteRead = number; } // �����ڲ�����

		// ��״̬
		void SetInquiryString(const CString& strInquiry) { m_strInquiry = strInquiry; }
		void SetInquiryHeader(const CString& strHeader) { m_strHeaders = strHeader; }

		bool IsWebError() const noexcept { return m_fWebError; }
		void SetWebError(bool fFlag) noexcept { m_fWebError = fFlag; }
		DWORD GetErrorCode() const noexcept { return m_dwErrorCode; }

		DWORD GetHTTPStatusCode() const noexcept { return m_dwHTTPStatusCode; }

		void SetContentLength(long length) noexcept { m_lContentLength = length; }
		long GetContentLength() const noexcept { return m_lContentLength; } // �����ڲ�����
		void SetBufferSize(long size) { m_sBuffer.resize(size); }
		size_t GetBufferSize() const noexcept { return m_sBuffer.size(); } // �����ڲ�����

	public:
		// ����Ϊ�����ú���
		void TESTSetBuffer(const char* buffer, INT64 lTotalNumber);
		void TESTSetBuffer(CString str);
		void TESTSetWebBuffer(const char* buffer, INT64 lTotalNumber);

	protected:
		shared_ptr<CInternetSession> m_pSession{ nullptr }; // ��ʼ��ʱ��������ָ�롣
		CHttpFile* m_pFile{ nullptr }; // �����ļ�ָ��
		DWORD m_dwHTTPStatusCode{ 0 }; //����״̬��
		DWORD m_dwErrorCode{ 0 }; // ���������

		bool m_fWebError{ false }; //�����ȡ������롣Ҳ������������жϵ����ݣ���Ϊ��ʱ�޴���
		string m_sBuffer{}; // ���յ����ݵĻ�����
		long m_lByteRead{ 0 }; // ���յ����ַ���.

		CString m_strInquiry{ _T("") }; // ��ѯ������ַ�����m_strInquiryFunction + m_strParam + m_strSuffix + m_strInquiryToken).
		CString m_strHeaders{ _T("") };

		long m_lContentLength{ 0 }; // Ԥ�ڵ��������ݳ��ȣ�ʹ��QueryInfo(HTTP_QUERY_CONTENT_LENGTH)�õ������ݣ�

	private:
		char m_dataBuffer[WEB_SOURCE_DATA_BUFFER_SIZE_]; //�������ݻ���
	};

	using CInquireEnginePtr = shared_ptr<CInquireEngine>;
}