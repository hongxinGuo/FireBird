#pragma once

#include"stdafx.h"
#include"afxmt.h"

using namespace std;
#include<deque>

class CSystemDeque {
public:
  CSystemDeque();
  ~CSystemDeque();

  void PushMessage(CString str);
  CString PopMessage(void);
  long GetDequeSize(void);

protected:
  deque<CString> m_dequeMessage;
  CCriticalSection m_Lock;
};

class CSystemMessage final
{
public:
  CSystemMessage();
  ~CSystemMessage();

  void PushInformationMessage(CString str) { m_InformationDeque.PushMessage(str); }
  CString PopInformationMessage(void) { return m_InformationDeque.PopMessage(); }
  long    GetInformationDequeSize(void) { return m_InformationDeque.GetDequeSize(); }

  void PushDayLineInfoMessage(CString str) { m_DayLineInfoMessage.PushMessage(str); }
  CString PopDayLineInfoMessage(void) { return m_DayLineInfoMessage.PopMessage(); }
  long    GetDayLineInfoDequeSize(void) { return m_DayLineInfoMessage.GetDequeSize(); }

  void PushTransactionMessage(CString str) { m_TransactionMessage.PushMessage(str); }
  CString PopTransactionMessage(void) { return m_TransactionMessage.PopMessage(); }
  long    GetTransactionDequeSize(void) { return m_TransactionMessage.GetDequeSize(); }

  void PushCancelSellMessage(CString str) { m_CancelSellMessage.PushMessage(str); }
  CString PopCancelSellMessage(void) { return m_CancelSellMessage.PopMessage(); }
  long    GetCancelSellDequeSize(void) { return m_CancelSellMessage.GetDequeSize(); }

  void PushCancelBuyMessage(CString str) { m_CancelBuyMessage.PushMessage(str); }
  CString PopCancelBuyMessage(void) { return m_CancelBuyMessage.PopMessage(); }
  long    GetCancelBuyDequeSize(void) { return m_CancelBuyMessage.GetDequeSize(); }

  void PushTrace2Message(CString str) { m_Trace2Message.PushMessage(str); }
  CString PopTrace2Message(void) { return m_Trace2Message.PopMessage(); }
  long    GetTrace2DequeSize(void) { return m_Trace2Message.GetDequeSize(); }

protected:
  // 信息输出队列群
  CSystemDeque m_InformationDeque;
  CSystemDeque m_DayLineInfoMessage;
  CSystemDeque m_TransactionMessage;
  CSystemDeque m_CancelSellMessage;
  CSystemDeque m_CancelBuyMessage;
  CSystemDeque m_Trace2Message;
};
