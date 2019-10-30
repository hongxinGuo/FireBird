#pragma once

#include"stdafx.h"

class CCriticalSectionBool {
public:
  CCriticalSectionBool() { m_fFlag = false; }
  ~CCriticalSectionBool() {}

  void SetFlag(bool fFlag);
  bool IsTrue(void);
  bool CheckTrueAndThenClearIt(void);
  bool CheckFalseAndThenSetIt(void);

protected:
  bool m_fFlag; // ʵʱ������Ԥ����ã�׼������
  CCriticalSection m_Lock;
};
