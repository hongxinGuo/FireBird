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

  void PushOutputMessage(CString str);
  void PushDataBaseMessage(CString str);
  void PushFindMessage(CString str);
  void PushWarningMessage(CString str);
  void PushTrace1Message(CString str);
  void PushTrace2Message(CString str);

  CString PopOutputMessage(void);
  CString PopDataBaseMessage(void);
  CString PopFindMessage(void);
  CString PopWarningMessage(void);
  CString PopTrace1Message(void);
  CString PopTrace2Message(void);

  long    GetOutputDequeSize(void);
  long    GetDataBaseDequeSize(void);
  long    GetFindDequeSize(void);
  long    GetWarningDequeSize(void);
  long    GetTrace1DequeSize(void);
  long    GetTrace2DequeSize(void);

protected:
  // ��Ϣ�������Ⱥ
  deque<CString> m_dequeOutputMessage; // �����ʾ����
  deque<CString> m_dequeDataBaseMessage; // �����ʾ����
  deque<CString> m_dequeFindMessage; // �����ʾ����
  deque<CString> m_dequeWarningMessage; // ������ʾ����
  deque<CString> m_dequeTrace1Message; // ������ʾ����1
  deque<CString> m_dequeTrace2Message; // ������ʾ����2

  CCriticalSection     m_OutputLock;       // ʹ�ñ���Դ
  CCriticalSection     m_DataBaseLock;       // ʹ�ñ���Դ
  CCriticalSection     m_Trace1Lock;       // ʹ�ñ���Դ
  CCriticalSection     m_Trace2Lock;       // ʹ�ñ���Դ
  CCriticalSection     m_FindLock;       // ʹ�ñ���Դ
  CCriticalSection     m_WarningLock;       // ʹ�ñ���Դ

};

