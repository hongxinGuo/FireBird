#pragma once

#include"stdafx.h"

class CCriticalSectionCounter
{
public:
  CCriticalSectionCounter();
  ~CCriticalSectionCounter();

  // ����ִ�м����������ǿ�ȵļ��������������gl_cMaxCalculatingRSThreads���߳�ͬʱִ��
  bool IncreasingCounter(void);  // ͬʱ�����߳�����һ
  bool DecreasingCounter(void);  // ͬʱ�����߳�����һ
  bool IsActive(void);          // �������ߵ��߳��Ƿ���������

protected:
  int m_iCounter;  // ���������������m_MaxNumber������
  CCriticalSection m_Lock;
};
