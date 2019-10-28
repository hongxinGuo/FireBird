#pragma once

#include"stdafx.h"

class CCriticalSectionCounter
{
public:
  CCriticalSectionCounter();
  ~CCriticalSectionCounter();

  void SetMaxCounter(int iValue) noexcept { m_MaxNumber = iValue; }
  // ����ִ�м����������ǿ�ȵļ��������������gl_cMaxCalculatingRSThreads���߳�ͬʱִ��
  void IncreaseCounter(void);  // ͬʱ�����߳�����һ
  void DecreaseCounter(void);  // ͬʱ�����߳�����һ
  bool IsCounterAvailable(void);        // �Ƿ����������µĹ����߳�
  bool IsActive(void);          // �������ߵ��߳��Ƿ���������

protected:
  int m_iCounter;  // ���������������m_MaxNumber������
  CCriticalSection m_Lock;
  int m_MaxNumber;
};
