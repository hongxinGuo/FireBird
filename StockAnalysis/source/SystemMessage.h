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
  // ��Ϣ�������Ⱥ
  deque<CString> m_dequeInformationMessage; // �����ʾ����
  CCriticalSection     m_InformationLock;       // ʹ�ñ���Դ

  deque<CString> m_dequeDayLineInfoMessage; // �����ʾ����
  CCriticalSection     m_DayLineInfoLock;       // ʹ�ñ���Դ
  
  deque<CString> m_dequeTransactionMessage; // �����ʾ����
  CCriticalSection     m_TransactionLock;       // ʹ�ñ���Դ
  
  deque<CString> m_dequeCancelSellMessage; // ������ʾ����
  CCriticalSection     m_CancelSellLock;       // ʹ�ñ���Դ
  
  deque<CString> m_dequeCancelBuyMessage; // ������ʾ����1
  CCriticalSection     m_CancelBuyLock;       // ʹ�ñ���Դ

  deque<CString> m_dequeTrace2Message; // ������ʾ����2
  CCriticalSection     m_Trace2Lock;       // ʹ�ñ���Դ

};

