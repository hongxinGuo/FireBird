#pragma once

#include"stdafx.h"
#include"afxmt.h"

using namespace std;
#include<deque>

class CSystemMessage
{
public:
  CSystemMessage();
  ~CSystemMessage();

  void PushInformationMessage(CString str);
  CString PopInformationMessage(void);
  long    GetInformationDequeSize(void);

  void PushDayLineInfoMessage(CString str);
  CString PopDayLineInfoMessage(void);
  long    GetDayLineInfoDequeSize(void);

  void PushTransactionMessage(CString str);
  CString PopTransactionMessage(void);
  long    GetTransactionDequeSize(void);

  void PushCancelSellMessage(CString str);
  CString PopCancelSellMessage(void);
  long    GetCancelSellDequeSize(void);

  void PushCancelBuyMessage(CString str);
  CString PopCancelBuyMessage(void);
  long    GetCancelBuyDequeSize(void);

  void PushTrace2Message(CString str);
  CString PopTrace2Message(void);
  long    GetTrace2DequeSize(void);

protected:
  // 信息输出队列群
  deque<CString> m_dequeInformationMessage; // 输出显示队列
  CCriticalSection     m_InformationLock;       // 使用本资源

  deque<CString> m_dequeDayLineInfoMessage; // 输出显示队列
  CCriticalSection     m_DayLineInfoLock;       // 使用本资源
  
  deque<CString> m_dequeTransactionMessage; // 输出显示队列
  CCriticalSection     m_TransactionLock;       // 使用本资源
  
  deque<CString> m_dequeCancelSellMessage; // 警告显示队列
  CCriticalSection     m_CancelSellLock;       // 使用本资源
  
  deque<CString> m_dequeCancelBuyMessage; // 跟踪显示队列1
  CCriticalSection     m_CancelBuyLock;       // 使用本资源

  deque<CString> m_dequeTrace2Message; // 跟踪显示队列2
  CCriticalSection     m_Trace2Lock;       // 使用本资源

};

