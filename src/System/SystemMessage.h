#pragma once
#include <concurrentqueue/moodycamel/concurrentqueue.h>

enum NET_ERROR {
	ERROR_INTERNET_OUT_OF_HANDLES = 12001,
	ERROR_INTERNET_TIMEOUT = 12002,
	ERROR_INTERNET_EXTENDED_ERROR = 12003,
	ERROR_INTERNET_INTERNAL_ERROR = 12004,
	ERROR_INTERNET_INVALID_URL = 12005,
	ERROR_INTERNET_UNRECOGNIZED_SCHEME = 12006,
	ERROR_INTERNET_NAME_NOT_RESOLVED = 12007,
	ERROR_INTERNET_PROTOCOL_NOT_FOUND = 12008,
	ERROR_INTERNET_INVALID_OPTION = 12009,
	ERROR_INTERNET_BAD_OPTION_LENGTH = 12010,
	ERROR_INTERNET_OPTION_NOT_SETTABLE = 12011,
	ERROR_INTERNET_SHUTDOWN = 12012,
	ERROR_INTERNET_INCORRECT_USER_NAME = 12013,
	ERROR_INTERNET_INCORRECT_PASSWORD = 12014,
	ERROR_INTERNET_LOGIN_FAILURE = 12015,
	ERROR_INTERNET_INVALID_OPERATION = 12016,
	ERROR_INTERNET_OPERATION_CANCELLED = 12017,
	ERROR_INTERNET_INCORRECT_HANDLE_TYPE = 12018,
	ERROR_INTERNET_INCORRECT_HANDLE_STATE = 12019,
	ERROR_INTERNET_NOT_PROXY_REQUEST = 12020,
	ERROR_INTERNET_REGISTRY_VALUE_NOT_FOUND = 12021,
	ERROR_INTERNET_BAD_REGISTRY_PARAMETER = 12022,
	ERROR_INTERNET_NO_DIRECT_ACCESS = 12023,
	ERROR_INTERNET_NO_CONTEXT = 12024,
	ERROR_INTERNET_NO_CALLBACK = 12025,
	ERROR_INTERNET_REQUEST_PENDING = 12026,
	ERROR_INTERNET_INCORRECT_FORMAT = 12027,
	ERROR_INTERNET_ITEM_NOT_FOUND = 12028,
	ERROR_INTERNET_CANNOT_CONNECT = 12029,
	ERROR_INTERNET_CONNECTION_ABORTED = 12030,
	ERROR_INTERNET_CONNECTION_RESET = 12031,
	ERROR_INTERNET_FORCE_RETRY = 12032,
	ERROR_INTERNET_INVALID_PROXY_REQUEST = 12033,

	ERROR_INTERNET_HANDLE_EXISTS = 12036,
	ERROR_INTERNET_SEC_CERT_DATE_INVALID = 12037,
	ERROR_INTERNET_SEC_CERT_CN_INVALID = 12038,
	ERROR_INTERNET_HTTP_TO_HTTPS_ON_REDIR = 12039,
	ERROR_INTERNET_HTTPS_TO_HTTP_ON_REDIR = 12040,
	ERROR_INTERNET_MIXED_SECURITY = 12041,
	ERROR_INTERNET_CHG_POST_IS_NON_SECURE = 12042,
	ERROR_INTERNET_POST_IS_NON_SECURE = 12043,

	ERROR_FTP_TRANSFER_IN_PROGRESS = 12110,
	ERROR_FTP_DROPPED = 12111,

	ERROR_GOPHER_PROTOCOL_ERROR = 12130,
	ERROR_GOPHER_NOT_FILE = 12131,
	ERROR_GOPHER_DATA_ERROR = 12132,
	ERROR_GOPHER_END_OF_DATA = 12133,
	ERROR_GOPHER_INVALID_LOCATOR = 12134,
	ERROR_GOPHER_INCORRECT_LOCATOR_TYPE = 12135,
	ERROR_GOPHER_NOT_GOPHER_PLUS = 12136,
	ERROR_GOPHER_ATTRIBUTE_NOT_FOUND = 12137,
	ERROR_GOPHER_UNKNOWN_LOCATOR = 12138,

	ERROR_HTTP_HEADER_NOT_FOUND = 12150,
	ERROR_HTTP_DOWNLEVEL_SERVER = 12151,
	ERROR_HTTP_INVALID_SERVER_RESPONSE = 12152,
	ERROR_HTTP_INVALID_HEADER = 12153,
	ERROR_HTTP_INVALID_QUERY_REQUEST = 12154,
	ERROR_HTTP_HEADER_ALREADY_EXISTS = 12155,
	ERROR_HTTP_REDIRECT_FAILED = 12156
};

#include<map>

extern std::map<long, std::string> gl_mapHTTPStatusCode;
extern std::map<long, std::string> gl_mapNetError;

class COutputList;

using std::atomic_long;
using std::string;

class CSystemDeque {
public:
	CSystemDeque() = default;
	virtual ~CSystemDeque() = default;

	void PushMessage(const string& str);
	string PopMessage();
	size_t Size() const;

	void Display(COutputList* pOutputList, const string& strTime);

	virtual void SysCallOutputListAddString(COutputList* pOutputList, const string& str);

protected:
	moodycamel::ConcurrentQueue<string> m_queueMessage;
};

class CSystemMessage final {
public:
	CSystemMessage();
	// 只能有一个实例,不允许赋值。
	CSystemMessage(const CSystemMessage&) = delete;
	CSystemMessage& operator=(const CSystemMessage&) = delete;
	CSystemMessage(const CSystemMessage&&) noexcept = delete;
	CSystemMessage& operator=(const CSystemMessage&&) noexcept = delete;
	~CSystemMessage() = default;

	void PushStockMarketInformationMessage(const string& str) { m_StockMarketInformation.PushMessage(str); }
	string PopStockMarketInformationMessage() { return m_StockMarketInformation.PopMessage(); }
	size_t StockMarketInformationSize() const { return m_StockMarketInformation.Size(); }
	void DisplayStockMarketInformation(COutputList* pOutputList, const string& strTime) { m_StockMarketInformation.Display(pOutputList, strTime); }

	void PushInformationMessage(const string& str) { m_SystemInformation.PushMessage(str); }
	string PopInformationMessage() { return m_SystemInformation.PopMessage(); }
	size_t InformationSize() const { return m_SystemInformation.Size(); }
	void DisplayInformation(COutputList* pOutputList, const string& strTime) { m_SystemInformation.Display(pOutputList, strTime); }

	void PushChinaMarketInformationMessage(const string& str) { m_ChinaMarketInformation.PushMessage(str); }
	string PopChinaMarketInformationMessage() { return m_ChinaMarketInformation.PopMessage(); }
	size_t ChinaMarketInformationSize() const { return m_ChinaMarketInformation.Size(); }
	void DisplayChinaMarketInformation(COutputList* pOutputList, const string& strTime) { m_ChinaMarketInformation.Display(pOutputList, strTime); }

	void PushDayLineInfoMessage(const string& s) { m_DayLineInformation.PushMessage(s); }
	string PopDayLineInfoMessage() { return m_DayLineInformation.PopMessage(); }
	size_t DayLineInfoSize() const { return m_DayLineInformation.Size(); }

	void DisplayDayLineInfo(COutputList* pOutputList, const string& strTime) { m_DayLineInformation.Display(pOutputList, strTime); }

	void PushWebSocketInfoMessage(const string& str) { m_WebSocketInfo.PushMessage(str); }
	string PopWebSocketInfoMessage() { return m_WebSocketInfo.PopMessage(); }
	size_t WebSocketInfoSize() const { return m_WebSocketInfo.Size(); }

	void DisplayWebSocketInfo(COutputList* pOutputList, const string& strTime) { m_WebSocketInfo.Display(pOutputList, strTime); }

	void PushInnerSystemInformationMessage(const string& str) { m_InnerSystemInformation.PushMessage(str); }
	string PopInnerSystemInformationMessage() { return m_InnerSystemInformation.PopMessage(); }
	size_t InnerSystemInfoSize() const { return m_InnerSystemInformation.Size(); }

	void DisplayInnerSystemInformation(COutputList* pOutputList, const string& strTime) { m_InnerSystemInformation.Display(pOutputList, strTime); }

	void PushErrorMessage(const string& str) { m_ErrorMessage.PushMessage(str); }
	string PopErrorMessage() { return m_ErrorMessage.PopMessage(); }
	size_t ErrorMessageSize() const { return m_ErrorMessage.Size(); }
	void DisplayErrorMessage(COutputList* pOutputList, const string& strTime) { m_ErrorMessage.Display(pOutputList, strTime); }

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

	void SetStockCodeForInquiringRTData(const string& strStockCode);
	string GetStockCodeForInquiringRTData() const;

	void SetStockCodeForInquiryDayLine(const string& strStockCode);
	string GetStockCodeForInquiryDayLine() const;

	void SetCurrentFinnhubWebSocketStake(const string& s);
	string GetCurrentFinnhubWebSocketStake() const;
	void SetCurrentTiingoWebSocketIEX(const string& s);
	string GetCurrentTiingoWebSocketIEX() const;
	void SetCurrentTiingoWebSocketForex(const string& s);
	string GetCurrentTiingoWebSocketForex() const;
	void SetCurrentTiingoWebSocketCrypto(const string& s);
	string GetCurrentTiingoWebSocketCrypto() const;

	void CalcScheduleTaskTimePerSecond() noexcept { m_lScheduleTaskTimePerSecond = m_lScheduleTaskTime.exchange(0); }
	long GetScheduleTaskTimePerSecond() const noexcept { return m_lScheduleTaskTimePerSecond.load(); }
	void IncreaseScheduleTaskTime(long lTime) noexcept { m_lScheduleTaskTime += lTime; }

	string GetCurrentFinnhubFunction() const;
	void SetCurrentFinnhubFunction(const string& str);
	string GetCurrentTiingoFunction() const;
	void SetCurrentTiingoFunction(const string& str);
	string GetCurrentAlphaVantageFunction() const;
	void SetCurrentAlphaVantageFunction(const string& str);

	string GetChinaMarketSavingFunction() { return m_sChinaMarketSavingFunction; }
	void SetChinaMarketSavingFunction(const string& str) { m_sChinaMarketSavingFunction = str; }
	string GetWorldMarketSavingFunction() { return m_sWorldMarketSavingFunction; }
	void SetWorldMarketSavingFunction(const string& str) { m_sWorldMarketSavingFunction = str; }

protected:
	// 信息输出队列群
	CSystemDeque m_StockMarketInformation;
	CSystemDeque m_SystemInformation;
	CSystemDeque m_ChinaMarketInformation;
	CSystemDeque m_DayLineInformation;
	CSystemDeque m_WebSocketInfo;
	CSystemDeque m_InnerSystemInformation;
	CSystemDeque m_ErrorMessage;

	int m_iProcessedFinnhubWebSocket;
	int m_iProcessedTiingoCryptoWebSocket;
	int m_iProcessedTiingoForexWebSocket;
	int m_iProcessedTiingoIEXWebSocket;

	string m_sStockCodeForInquiringRTData;
	string m_sStockCodeForInquiryDayLine;

	string m_sCurrentFinnhubWebSocketStake;
	string m_sCurrentTiingoWebSocketIEX;
	string m_sCurrentTiingoWebSocketForex;
	string m_sCurrentTiingoWebSocketCrypto;

	string m_sCurrentFinnhubFunction{}; // 当前任务和处理的证券名称
	string m_sCurrentTiingoFunction{}; // 当前任务和处理的证券名称
	string m_sCurrentAlphaVantageFunction{}; // 当前任务和处理的证券名称

	string m_sChinaMarketSavingFunction{};
	string m_sWorldMarketSavingFunction{};

	atomic_long m_lScheduleTaskTime{ 0 };
	atomic_long m_lScheduleTaskTimePerSecond{ 0 };
};

extern CSystemMessage gl_systemMessage; // 系统消息汇总类。此变量必须放在全局变量初始化时的前面，其他全局变量初始化时用到此变量。
