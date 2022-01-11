#pragma once

#include"OutputWnd.h"

using namespace std;
#include<deque>
#include<mutex>

class CSystemDeque {
public:
	CSystemDeque();
	virtual ~CSystemDeque();

	void PushMessage(CString str);
	CString PopMessage(void);
	size_t GetDequeSize(void);

	void Display(COutputList* pOutputList, CString strTime);

	virtual void SysCallOutputListAddString(COutputList* pOutputList, CString str);

protected:
	deque<CString> m_dequeMessage;
	mutex m_mutex;
};

class CSystemMessage final
{
public:
	CSystemMessage();
	~CSystemMessage();

	void PushInformationMessage(CString str) { m_SystemInformation.PushMessage(str); }
	CString PopInformationMessage(void) { return m_SystemInformation.PopMessage(); }
	size_t GetInformationDequeSize(void) { return m_SystemInformation.GetDequeSize(); }
	void DisplayInformation(COutputList* pOutputList, CString strTime) { m_SystemInformation.Display(pOutputList, strTime); }

	void PushDayLineInfoMessage(CString str) { m_DayLineInformation.PushMessage(str); }
	CString PopDayLineInfoMessage(void) { return m_DayLineInformation.PopMessage(); }
	size_t GetDayLineInfoDequeSize(void) { return m_DayLineInformation.GetDequeSize(); }
	void DisplayDayLineInfo(COutputList* pOutputList, CString strTime) { m_DayLineInformation.Display(pOutputList, strTime); }

	void PushTransactionMessage(CString str) { m_Transaction.PushMessage(str); }
	CString PopTransactionMessage(void) { return m_Transaction.PopMessage(); }
	size_t GetTransactionDequeSize(void) { return m_Transaction.GetDequeSize(); }
	void DisplayTransaction(COutputList* pOutputList, CString strTime) { m_Transaction.Display(pOutputList, strTime); }

	void PushCancelSellMessage(CString str) { m_CancelSell.PushMessage(str); }
	CString PopCancelSellMessage(void) { return m_CancelSell.PopMessage(); }
	size_t GetCancelSellDequeSize(void) { return m_CancelSell.GetDequeSize(); }
	void DisplayCancelSell(COutputList* pOutputList, CString strTime) { m_CancelSell.Display(pOutputList, strTime); }

	void PushCancelBuyMessage(CString str) { m_CancelBuy.PushMessage(str); }
	CString PopCancelBuyMessage(void) { return m_CancelBuy.PopMessage(); }
	size_t GetCancelBuyDequeSize(void) { return m_CancelBuy.GetDequeSize(); }
	void DisplayCancelBuy(COutputList* pOutputList, CString strTime) { m_CancelBuy.Display(pOutputList, strTime); }

	void PushTrace2Message(CString str) { m_Trace2.PushMessage(str); }
	CString PopTrace2Message(void) { return m_Trace2.PopMessage(); }
	size_t GetTrace2DequeSize(void) { return m_Trace2.GetDequeSize(); }
	void DisplayTrace2(COutputList* pOutputList, CString strTime) { m_Trace2.Display(pOutputList, strTime); }

	void PushWebSocketInfoMessage(CString str) { m_WebSocketInfo.PushMessage(str); }
	CString PopWebSocketInfoMessage(void) { return m_WebSocketInfo.PopMessage(); }
	size_t GetWebSocketInfoDequeSize(void) { return m_WebSocketInfo.GetDequeSize(); }
	void DisplayWebSocketInfo(COutputList* pOutputList, CString strTime) { m_WebSocketInfo.Display(pOutputList, strTime); }

	void PushInnerSystemInformationMessage(CString str) { m_InnerSystemInformation.PushMessage(str); }
	CString PopInnerSystemInformationMessage(void) { return m_InnerSystemInformation.PopMessage(); }
	size_t GetInnerSystemInformationDequeSize(void) { return m_InnerSystemInformation.GetDequeSize(); }
	void DisplayInnerSystemInformation(COutputList* pOutputList, CString strTime) { m_InnerSystemInformation.Display(pOutputList, strTime); }

	void PushErrorMessage(CString str) { m_ErrorMessage.PushMessage(str); }
	CString PopErrorMessage(void) { return m_ErrorMessage.PopMessage(); }
	size_t GetErrorMessageDequeSize(void) { return m_ErrorMessage.GetDequeSize(); }
	void DisplayErrorMessage(COutputList* pOutputList, CString strTime) { m_ErrorMessage.Display(pOutputList, strTime); }

	int GetProcessedFinnhubWebSocket(void) noexcept { return m_iProcessedFinnhubWebSocket; }
	void SetProcessedFinnhubWebSocket(int iValue) noexcept { m_iProcessedFinnhubWebSocket = iValue; }
	void ClearProcessedFinnhubWebSocket(void) noexcept { m_iProcessedFinnhubWebSocket = 0; }
	int GetProcessedTiingoIEXWebSocket(void) noexcept { return m_iProcessedTiingoIEXWebSocket; }
	void SetProcessedTiingoIEXWebSocket(int iValue) noexcept { m_iProcessedTiingoIEXWebSocket = iValue; }
	void ClearProcessedTiingoIEXWebSocket(void) noexcept { m_iProcessedTiingoIEXWebSocket = 0; }
	int GetProcessedTiingoCryptoWebSocket(void) noexcept { return m_iProcessedTiingoCryptoWebSocket; }
	void SetProcessedTiingoCryptoWebSocket(int iValue) noexcept { m_iProcessedTiingoCryptoWebSocket = iValue; }
	void ClearProcessedTiingoCryptoWebSocket(void) noexcept { m_iProcessedTiingoCryptoWebSocket = 0; }
	int GetProcessedTiingoForexWebSocket(void) noexcept { return m_iProcessedTiingoForexWebSocket; }
	void SetProcessedTiingoForexWebSocket(int iValue) noexcept { m_iProcessedTiingoForexWebSocket = iValue; }
	void ClearProcessedTiingoForexWebSocket(void) noexcept { m_iProcessedTiingoForexWebSocket = 0; }

	void SetStockCodeForInquiringRTData(CString strStockCode) { m_strStockCodeForInquiringRTData = strStockCode; }
	CString GetStockCodeForInquiringRTData(void) const { return m_strStockCodeForInquiringRTData; }
	void SetStockCodeForInquiringNeteaseDayLine(CString strStockCode) { m_strStockCodeForInquiringNeteaseDayLine = strStockCode; }
	CString GetStockCodeForInquiringNeteaseDayLine(void) const { return m_strStockCodeForInquiringNeteaseDayLine; }

	void SetCurrentFinnhubWebSocketStake(CString s) { m_strCurrentFinnhubWebSocketStake = s; }
	CString GetCurrentFinnhubWebSocketStake(void) { return m_strCurrentFinnhubWebSocketStake; }
	void SetCurrentTiingoWebSocketIEX(CString s) { m_strCurrentTiingoWebSocketIEX = s; }
	CString GetCurrentTiingoWebSocketIEX(void) { return m_strCurrentTiingoWebSocketIEX; }
	void SetCurrentTiingoWebSocketForex(CString s) { m_strCurrentTiingoWebSocketForex = s; }
	CString GetCurrentTiingoWebSocketForex(void) { return m_strCurrentTiingoWebSocketForex; }
	void SetCurrentTiingoWebSocketCrypto(CString s) { m_strCurrentTiingoWebSocketCrypto = s; }
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
