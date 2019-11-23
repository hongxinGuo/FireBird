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
  bool m_fFlag; // 实时数据已预处理好，准备计算
  CCriticalSection m_Lock;
};
