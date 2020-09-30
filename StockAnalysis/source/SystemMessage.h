#pragma once

#include"stdafx.h"

#include"OutputWnd.h"

using namespace std;
#include<deque>
#include<mutex>

class CSystemDeque {
public:
  CSystemDeque();
  ~CSystemDeque();

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

  void PushDLInfoMessage(CString str) { m_DLInformation.PushMessage(str); }
  CString PopDLInfoMessage(void) { return m_DLInformation.PopMessage(); }
  size_t GetDLInfoDequeSize(void) { return m_DLInformation.GetDequeSize(); }
  void DisplayDLInfo(COutputList* pOutputList, CString strTime) { m_DLInformation.Display(pOutputList, strTime); }

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

  void PushInnerSystemInformationMessage(CString str) { m_InnerSystemInformation.PushMessage(str); }
  CString PopInnerSystemInformationMessage(void) { return m_InnerSystemInformation.PopMessage(); }
  size_t GetInnerSystemInformationDequeSize(void) { return m_InnerSystemInformation.GetDequeSize(); }
  void DisplayInnerSystemInformation(COutputList* pOutputList, CString strTime) { m_InnerSystemInformation.Display(pOutputList, strTime); }

protected:
  // 信息输出队列群
  CSystemDeque m_SystemInformation;
  CSystemDeque m_DLInformation;
  CSystemDeque m_Transaction;
  CSystemDeque m_CancelSell;
  CSystemDeque m_CancelBuy;
  CSystemDeque m_Trace2;
  CSystemDeque m_InnerSystemInformation;
};
