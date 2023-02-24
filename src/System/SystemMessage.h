#pragma once

class COutputList;

#include<deque>
#include<mutex>
using std::deque;
using std::mutex;

class CSystemDeque {
public:
	CSystemDeque();
	virtual ~CSystemDeque();

	void PushMessage(const CString& str);
	CString PopMessage(void);
	size_t Size(void);

	void Display(COutputList* pOutputList, const CString& strTime);

	virtual void SysCallOutputListAddString(COutputList* pOutputList, const CString& str);

protected:
	deque<CString> m_dequeMessage;
	mutex m_mutex;
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
	CString PopInformationMessage(void) { return m_SystemInformation.PopMessage(); }
	size_t InformationSize(void) { return m_SystemInformation.Size(); }

	void DisplayInformation(COutputList* pOutputList, const CString& strTime) { m_SystemInformation.Display(pOutputList, strTime); }

	void PushDayLineInfoMessage(const CString& str) { m_DayLineInformation.PushMessage(str); }
	CString PopDayLineInfoMessage(void) { return m_DayLineInformation.PopMessage(); }
	size_t DayLineInfoSize(void) { return m_DayLineInformation.Size(); }

	void DisplayDayLineInfo(COutputList* pOutputList, const CString& strTime) { m_DayLineInformation.Display(pOutputList, strTime); }

	void PushTransactionMessage(const CString& str) { m_Transaction.PushMessage(str); }
	CString PopTransactionMessage(void) { return m_Transaction.PopMessage(); }
	size_t TransactionInfoSize(void) { return m_Transaction.Size(); }
	void DisplayTransaction(COutputList* pOutputList, const CString& strTime) { m_Transaction.Display(pOutputList, strTime); }

	void PushCancelSellMessage(const CString& str) { m_CancelSell.PushMessage(str); }
	CString PopCancelSellMessage(void) { return m_CancelSell.PopMessage(); }
	size_t CancelSellInfoSize(void) { return m_CancelSell.Size(); }
	void DisplayCancelSell(COutputList* pOutputList, const CString& strTime) { m_CancelSell.Display(pOutputList, strTime); }

	void PushCancelBuyMessage(const CString& str) { m_CancelBuy.PushMessage(str); }
	CString PopCancelBuyMessage(void) { return m_CancelBuy.PopMessage(); }
	size_t CancelBuyInfoSize(void) { return m_CancelBuy.Size(); }
	void DisplayCancelBuy(COutputList* pOutputList, const CString& strTime) { m_CancelBuy.Display(pOutputList, strTime); }

	void PushTrace2Message(const CString& str) { m_Trace2.PushMessage(str); }
	CString PopTrace2Message(void) { return m_Trace2.PopMessage(); }
	size_t Trace2Size(void) { return m_Trace2.Size(); }
	void DisplayTrace2(COutputList* pOutputList, const CString& strTime) { m_Trace2.Display(pOutputList, strTime); }

	void PushWebSocketInfoMessage(const CString& str) { m_WebSocketInfo.PushMessage(str); }
	CString PopWebSocketInfoMessage(void) { return m_WebSocketInfo.PopMessage(); }
	size_t WebSocketInfoSize(void) { return m_WebSocketInfo.Size(); }

	void DisplayWebSocketInfo(COutputList* pOutputList, const CString& strTime) { m_WebSocketInfo.Display(pOutputList, strTime); }

	void PushInnerSystemInformationMessage(const CString& str) { m_InnerSystemInformation.PushMessage(str); }
	CString PopInnerSystemInformationMessage(void) { return m_InnerSystemInformation.PopMessage(); }
	size_t InnerSystemInfoSize(void) { return m_InnerSystemInformation.Size(); }

	void DisplayInnerSystemInformation(COutputList* pOutputList, const CString& strTime) { m_InnerSystemInformation.Display(pOutputList, strTime); }

	void PushErrorMessage(const CString& str) { m_ErrorMessage.PushMessage(str); }
	CString PopErrorMessage(void) { return m_ErrorMessage.PopMessage(); }
	size_t ErrorMessageSize(void) { return m_ErrorMessage.Size(); }
	void DisplayErrorMessage(COutputList* pOutputList, const CString& strTime) { m_ErrorMessage.Display(pOutputList, strTime); }

	int GetProcessedFinnhubWebSocket(void) const noexcept { return m_iProcessedFinnhubWebSocket; }
	void SetProcessedFinnhubWebSocket(const int iValue) noexcept { m_iProcessedFinnhubWebSocket = iValue; }
	void ClearProcessedFinnhubWebSocket(void) noexcept { m_iProcessedFinnhubWebSocket = 0; }
	int GetProcessedTiingoIEXWebSocket(void) const noexcept { return m_iProcessedTiingoIEXWebSocket; }
	void SetProcessedTiingoIEXWebSocket(const int iValue) noexcept { m_iProcessedTiingoIEXWebSocket = iValue; }
	void ClearProcessedTiingoIEXWebSocket(void) noexcept { m_iProcessedTiingoIEXWebSocket = 0; }
	int GetProcessedTiingoCryptoWebSocket(void) const noexcept { return m_iProcessedTiingoCryptoWebSocket; }
	void SetProcessedTiingoCryptoWebSocket(const int iValue) noexcept { m_iProcessedTiingoCryptoWebSocket = iValue; }
	void ClearProcessedTiingoCryptoWebSocket(void) noexcept { m_iProcessedTiingoCryptoWebSocket = 0; }
	int GetProcessedTiingoForexWebSocket(void) const noexcept { return m_iProcessedTiingoForexWebSocket; }
	void SetProcessedTiingoForexWebSocket(const int iValue) noexcept { m_iProcessedTiingoForexWebSocket = iValue; }
	void ClearProcessedTiingoForexWebSocket(void) noexcept { m_iProcessedTiingoForexWebSocket = 0; }

	void SetStockCodeForInquiringRTData(const CString& strStockCode) { m_strStockCodeForInquiringRTData = strStockCode; }
	CString GetStockCodeForInquiringRTData(void) const { return m_strStockCodeForInquiringRTData; }

	void SetStockCodeForInquiringNeteaseDayLine(const CString& strStockCode) { m_strStockCodeForInquiringNeteaseDayLine = strStockCode; }

	CString GetStockCodeForInquiringNeteaseDayLine(void) const { return m_strStockCodeForInquiringNeteaseDayLine; }

	void SetCurrentFinnhubWebSocketStake(const CString& s) { m_strCurrentFinnhubWebSocketStake = s; }
	CString GetCurrentFinnhubWebSocketStake(void) { return m_strCurrentFinnhubWebSocketStake; }
	void SetCurrentTiingoWebSocketIEX(const CString& s) { m_strCurrentTiingoWebSocketIEX = s; }
	CString GetCurrentTiingoWebSocketIEX(void) { return m_strCurrentTiingoWebSocketIEX; }
	void SetCurrentTiingoWebSocketForex(const CString& s) { m_strCurrentTiingoWebSocketForex = s; }
	CString GetCurrentTiingoWebSocketForex(void) { return m_strCurrentTiingoWebSocketForex; }
	void SetCurrentTiingoWebSocketCrypto(const CString& s) { m_strCurrentTiingoWebSocketCrypto = s; }
	CString GetCurrentTiingoWebSocketCrypto(void) { return m_strCurrentTiingoWebSocketCrypto; }

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
	CString m_strStockCodeForInquiringNeteaseDayLine;

	CString m_strCurrentFinnhubWebSocketStake;
	CString m_strCurrentTiingoWebSocketIEX;
	CString m_strCurrentTiingoWebSocketForex;
	CString m_strCurrentTiingoWebSocketCrypto;
};

extern CSystemMessage gl_systemMessage; // 系统消息汇总类。此变量必须放在全局变量初始化时的前面，其他全局变量初始化时用到此变量。
