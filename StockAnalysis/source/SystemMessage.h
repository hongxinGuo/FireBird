#pragma once

#include"stdafx.h"
#include"afxmt.h"

using namespace std;
#include<deque>
#include<mutex>

class CSystemDeque {
public:
  CSystemDeque();
  ~CSystemDeque();

  void PushMessage(CString str);
  CString PopMessage(void);
  long GetDequeSize(void);

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
  long GetInformationDequeSize(void) { return m_SystemInformation.GetDequeSize(); }

  void PushDayLineInfoMessage(CString str) { m_DayLineInformation.PushMessage(str); }
  CString PopDayLineInfoMessage(void) { return m_DayLineInformation.PopMessage(); }
  long GetDayLineInfoDequeSize(void) { return m_DayLineInformation.GetDequeSize(); }

  void PushTransactionMessage(CString str) { m_Transaction.PushMessage(str); }
  CString PopTransactionMessage(void) { return m_Transaction.PopMessage(); }
  long GetTransactionDequeSize(void) { return m_Transaction.GetDequeSize(); }

  void PushCancelSellMessage(CString str) { m_CancelSell.PushMessage(str); }
  CString PopCancelSellMessage(void) { return m_CancelSell.PopMessage(); }
  long GetCancelSellDequeSize(void) { return m_CancelSell.GetDequeSize(); }

  void PushCancelBuyMessage(CString str) { m_CancelBuy.PushMessage(str); }
  CString PopCancelBuyMessage(void) { return m_CancelBuy.PopMessage(); }
  long GetCancelBuyDequeSize(void) { return m_CancelBuy.GetDequeSize(); }

  void PushTrace2Message(CString str) { m_Trace2.PushMessage(str); }
  CString PopTrace2Message(void) { return m_Trace2.PopMessage(); }
  long GetTrace2DequeSize(void) { return m_Trace2.GetDequeSize(); }

  void PushInnerSystemInformationMessage(CString str) { m_InnerSystemInformation.PushMessage(str); }
  CString PopInnerSystemInformationMessage(void) { return m_InnerSystemInformation.PopMessage(); }
  long GetInnerSystemInformationDequeSize(void) { return m_InnerSystemInformation.GetDequeSize(); }

protected:
  // 信息输出队列群
  CSystemDeque m_SystemInformation;
  CSystemDeque m_DayLineInformation;
  CSystemDeque m_Transaction;
  CSystemDeque m_CancelSell;
  CSystemDeque m_CancelBuy;
  CSystemDeque m_Trace2;
  CSystemDeque m_InnerSystemInformation;
};
