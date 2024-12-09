#include"pch.h"
module;
#include"concurrentqueue/concurrentqueue.h"
export module FireBird.System.Message;

using namespace moodycamel;
using std::atomic_long;
import FireBird.WindowInterface.OutputWnd;

export {
	enum {
		ERROR_INTERNET_OUT_OF_HANDLES = 12001,
		ERROR_INTERNET_TIMEOUT,
		ERROR_INTERNET_EXTENDED_ERROR,
		ERROR_INTERNET_INTERNAL_ERROR,
		ERROR_INTERNET_INVALID_URL,
		ERROR_INTERNET_UNRECOGNIZED_SCHEME,
		ERROR_INTERNET_NAME_NOT_RESOLVED,
		ERROR_INTERNET_PROTOCOL_NOT_FOUND,
		ERROR_INTERNET_INVALID_OPTION,
		ERROR_INTERNET_BAD_OPTION_LENGTH,
		ERROR_INTERNET_OPTION_NOT_SETTABLE,
		ERROR_INTERNET_SHUTDOWN,
		ERROR_INTERNET_INCORRECT_USER_NAME,
		ERROR_INTERNET_INCORRECT_PASSWORD,
		ERROR_INTERNET_LOGIN_FAILURE,
		ERROR_INTERNET_INVALID_OPERATION,
		ERROR_INTERNET_OPERATION_CANCELLED,
		ERROR_INTERNET_INCORRECT_HANDLE_TYPE,
		ERROR_INTERNET_INCORRECT_HANDLE_STATE,
		ERROR_INTERNET_NOT_PROXY_REQUEST,
		ERROR_INTERNET_REGISTRY_VALUE_NOT_FOUND,
		ERROR_INTERNET_BAD_REGISTRY_PARAMETER,
		ERROR_INTERNET_NO_DIRECT_ACCESS,
		ERROR_INTERNET_NO_CONTEXT,
		ERROR_INTERNET_NO_CALLBACK,
		ERROR_INTERNET_REQUEST_PENDING,
		ERROR_INTERNET_INCORRECT_FORMAT,
		ERROR_INTERNET_ITEM_NOT_FOUND,
		ERROR_INTERNET_CANNOT_CONNECT,
		ERROR_INTERNET_CONNECTION_ABORTED,
		ERROR_INTERNET_CONNECTION_RESET,
		ERROR_INTERNET_FORCE_RETRY,
		ERROR_INTERNET_INVALID_PROXY_REQUEST,

		ERROR_INTERNET_HANDLE_EXISTS = 12036,
		ERROR_INTERNET_SEC_CERT_DATE_INVALID,
		ERROR_INTERNET_SEC_CERT_CN_INVALID,
		ERROR_INTERNET_HTTP_TO_HTTPS_ON_REDIR,
		ERROR_INTERNET_HTTPS_TO_HTTP_ON_REDIR,
		ERROR_INTERNET_MIXED_SECURITY,
		ERROR_INTERNET_CHG_POST_IS_NON_SECURE,
		ERROR_INTERNET_POST_IS_NON_SECURE,

		ERROR_FTP_TRANSFER_IN_PROGRESS = 12110,
		ERROR_FTP_DROPPED,

		ERROR_GOPHER_PROTOCOL_ERROR = 12130,
		ERROR_GOPHER_NOT_FILE,
		ERROR_GOPHER_DATA_ERROR,
		ERROR_GOPHER_END_OF_DATA,
		ERROR_GOPHER_INVALID_LOCATOR,
		ERROR_GOPHER_INCORRECT_LOCATOR_TYPE,
		ERROR_GOPHER_NOT_GOPHER_PLUS,
		ERROR_GOPHER_ATTRIBUTE_NOT_FOUND,
		ERROR_GOPHER_UNKNOWN_LOCATOR,

		ERROR_HTTP_HEADER_NOT_FOUND = 12150,
		ERROR_HTTP_DOWNLEVEL_SERVER,
		ERROR_HTTP_INVALID_SERVER_RESPONSE,
		ERROR_HTTP_INVALID_HEADER,
		ERROR_HTTP_INVALID_QUERY_REQUEST,
		ERROR_HTTP_HEADER_ALREADY_EXISTS,
		ERROR_HTTP_REDIRECT_FAILED,
	};

	extern map<long, string> gl_mapHTTPStatusCode;
	extern map<long, string> gl_mapNetError;

	class CSystemDeque {
	public:
		CSystemDeque();
		virtual ~CSystemDeque();

		void PushMessage(const CString& str);
		CString PopMessage();
		size_t Size() const;

		void Display(COutputList* pOutputList, const CString& strTime);

		virtual void SysCallOutputListAddString(COutputList* pOutputList, const CString& str);

	protected:
		ConcurrentQueue<string> m_queueMessage;
	};

	class CSystemMessage final {
	public:
		CSystemMessage();
		// 只能有一个实例,不允许赋值。
		CSystemMessage(const CSystemMessage&) = delete;
		CSystemMessage& operator=(const CSystemMessage&) = delete;
		CSystemMessage(const CSystemMessage&&) noexcept = delete;
		CSystemMessage& operator=(const CSystemMessage&&) noexcept = delete;
		~CSystemMessage();

		void PushInformationMessage(const CString& str) { m_SystemInformation.PushMessage(str); }
		CString PopInformationMessage() { return m_SystemInformation.PopMessage(); }
		size_t InformationSize() const { return m_SystemInformation.Size(); }

		void DisplayInformation(COutputList* pOutputList, const CString& strTime) { m_SystemInformation.Display(pOutputList, strTime); }

		void PushDayLineInfoMessage(const CString& str) { m_DayLineInformation.PushMessage(str); }
		CString PopDayLineInfoMessage() { return m_DayLineInformation.PopMessage(); }
		size_t DayLineInfoSize() const { return m_DayLineInformation.Size(); }

		void DisplayDayLineInfo(COutputList* pOutputList, const CString& strTime) { m_DayLineInformation.Display(pOutputList, strTime); }

		void PushWebSocketInfoMessage(const CString& str) { m_WebSocketInfo.PushMessage(str); }
		CString PopWebSocketInfoMessage() { return m_WebSocketInfo.PopMessage(); }
		size_t WebSocketInfoSize() const { return m_WebSocketInfo.Size(); }

		void DisplayWebSocketInfo(COutputList* pOutputList, const CString& strTime) { m_WebSocketInfo.Display(pOutputList, strTime); }

		void PushInnerSystemInformationMessage(const CString& str) { m_InnerSystemInformation.PushMessage(str); }
		CString PopInnerSystemInformationMessage() { return m_InnerSystemInformation.PopMessage(); }
		size_t InnerSystemInfoSize() const { return m_InnerSystemInformation.Size(); }

		void DisplayInnerSystemInformation(COutputList* pOutputList, const CString& strTime) { m_InnerSystemInformation.Display(pOutputList, strTime); }

		void PushErrorMessage(const CString& str) { m_ErrorMessage.PushMessage(str); }
		CString PopErrorMessage() { return m_ErrorMessage.PopMessage(); }
		size_t ErrorMessageSize() const { return m_ErrorMessage.Size(); }
		void DisplayErrorMessage(COutputList* pOutputList, const CString& strTime) { m_ErrorMessage.Display(pOutputList, strTime); }

		int GetProcessedFinnhubWebSocket() const noexcept { return m_iProcessedFinnhubWebSocket; }
		void SetProcessedFinnhubWebSocket(const int iValue) noexcept { m_iProcessedFinnhubWebSocket = iValue; }
		void ClearProcessedFinnhubWebSocket() noexcept { m_iProcessedFinnhubWebSocket = 0; }
		int GetProcessedTiingoIEXWebSocket() const noexcept { return m_iProcessedTiingoIEXWebSocket; }
		void SetProcessedTiingoIEXWebSocket(const int iValue) noexcept { m_iProcessedTiingoIEXWebSocket = iValue; }
		void ClearProcessedTiingoIEXWebSocket() noexcept { m_iProcessedTiingoIEXWebSocket = 0; }
		int GetProcessedTiingoCryptoWebSocket() const noexcept { return m_iProcessedTiingoCryptoWebSocket; }
		void SetProcessedTiingoCryptoWebSocket(const int iValue) noexcept { m_iProcessedTiingoCryptoWebSocket = iValue; }
		void ClearProcessedTiingoCryptoWebSocket() noexcept { m_iProcessedTiingoCryptoWebSocket = 0; }
		int GetProcessedTiingoForexWebSocket() const noexcept { return m_iProcessedTiingoForexWebSocket; }
		void SetProcessedTiingoForexWebSocket(const int iValue) noexcept { m_iProcessedTiingoForexWebSocket = iValue; }
		void ClearProcessedTiingoForexWebSocket() noexcept { m_iProcessedTiingoForexWebSocket = 0; }

		void SetStockCodeForInquiringRTData(const CString& strStockCode);
		CString GetStockCodeForInquiringRTData() const;

		void SetStockCodeForInquiryDayLine(const CString& strStockCode);
		CString GetStockCodeForInquiryDayLine() const;

		void SetCurrentFinnhubWebSocketStake(const CString& s);
		CString GetCurrentFinnhubWebSocketStake() const;
		void SetCurrentTiingoWebSocketIEX(const CString& s);
		CString GetCurrentTiingoWebSocketIEX() const;
		void SetCurrentTiingoWebSocketForex(const CString& s);
		CString GetCurrentTiingoWebSocketForex() const;
		void SetCurrentTiingoWebSocketCrypto(const CString& s);
		CString GetCurrentTiingoWebSocketCrypto() const;

		void CalcScheduleTaskTimePerSecond() noexcept { m_lScheduleTaskTimePerSecond = m_lScheduleTaskTime.exchange(0); }
		long GetScheduleTaskTimePerSecond() const noexcept { return m_lScheduleTaskTimePerSecond.load(); }
		void IncreaseScheduleTaskTime(long lTime) noexcept { m_lScheduleTaskTime += lTime; }

		CString GetCurrentFinnhubFunction() const;
		void SetCurrentFinnhubFunction(const CString& str);
		CString GetCurrentTiingoFunction() const;
		void SetCurrentTiingoFunction(const CString& str);

	protected:
		// 信息输出队列群
		CSystemDeque m_SystemInformation;
		CSystemDeque m_DayLineInformation;
		CSystemDeque m_Transaction;
		CSystemDeque m_CancelSell;
		CSystemDeque m_CancelBuy;
		CSystemDeque m_Trace2;
		CSystemDeque m_WebSocketInfo;
		CSystemDeque m_InnerSystemInformation;
		CSystemDeque m_ErrorMessage;

		int m_iProcessedFinnhubWebSocket;
		int m_iProcessedTiingoCryptoWebSocket;
		int m_iProcessedTiingoForexWebSocket;
		int m_iProcessedTiingoIEXWebSocket;

		CString m_strStockCodeForInquiringRTData;
		CString m_strStockCodeForInquiryDayLine;

		CString m_strCurrentFinnhubWebSocketStake;
		CString m_strCurrentTiingoWebSocketIEX;
		CString m_strCurrentTiingoWebSocketForex;
		CString m_strCurrentTiingoWebSocketCrypto;

		CString m_strCurrentFinnhubFunction{ _T("") }; // 当前任务和处理的证券名称
		CString m_strCurrentTiingoFunction{ _T("") }; // 当前任务和处理的证券名称

		atomic_long m_lScheduleTaskTime{ 0 };
		atomic_long m_lScheduleTaskTimePerSecond{ 0 };
	};

	extern CSystemMessage gl_systemMessage; // 系统消息汇总类。此变量必须放在全局变量初始化时的前面，其他全局变量初始化时用到此变量。
}