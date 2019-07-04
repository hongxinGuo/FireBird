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
  // 信息输出队列群
  deque<CString> m_dequeOutputMessage; // 输出显示队列
  deque<CString> m_dequeDataBaseMessage; // 输出显示队列
  deque<CString> m_dequeFindMessage; // 输出显示队列
  deque<CString> m_dequeWarningMessage; // 警告显示队列
  deque<CString> m_dequeTrace1Message; // 跟踪显示队列1
  deque<CString> m_dequeTrace2Message; // 跟踪显示队列2

  CCriticalSection     m_OutputLock;       // 使用本资源
  CCriticalSection     m_DataBaseLock;       // 使用本资源
  CCriticalSection     m_Trace1Lock;       // 使用本资源
  CCriticalSection     m_Trace2Lock;       // 使用本资源
  CCriticalSection     m_FindLock;       // 使用本资源
  CCriticalSection     m_WarningLock;       // 使用本资源

};

