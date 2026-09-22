#pragma once
#include <concurrentqueue/moodycamel/concurrentqueue.h>

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

	void PushWebInformationMessage(const string& str) { m_WebInformation.PushMessage(str); }
	string PopWebInformationMessage() { return m_WebInformation.PopMessage(); }
	size_t WebInformationSize() const { return m_WebInformation.Size(); }
	void DisplayWebInformation(COutputList* pOutputList, const string& strTime) { m_WebInformation.Display(pOutputList, strTime); }

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
	string GetCurrentAlpacaFunction() const;
	void SetCurrentAlpacaFunction(const string& str);

	string GetChinaMarketSavingFunction() { return m_sChinaMarketSavingFunction; }
	void SetChinaMarketSavingFunction(const string& str) { m_sChinaMarketSavingFunction = str; }
	string GetWorldMarketSavingFunction() { return m_sWorldMarketSavingFunction; }
	void SetWorldMarketSavingFunction(const string& str) { m_sWorldMarketSavingFunction = str; }

protected:
	// 信息输出队列群
	CSystemDeque m_StockMarketInformation;
	CSystemDeque m_SystemInformation;
	CSystemDeque m_WebInformation;
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
	string m_sCurrentAlpacaFunction{}; // 当前任务和处理的证券名称

	string m_sChinaMarketSavingFunction{};
	string m_sWorldMarketSavingFunction{};

	atomic_long m_lScheduleTaskTime{ 0 };
	atomic_long m_lScheduleTaskTimePerSecond{ 0 };
};

extern CSystemMessage gl_systemMessage; // 系统消息汇总类。此变量必须放在全局变量初始化时的前面，其他全局变量初始化时用到此变量。
