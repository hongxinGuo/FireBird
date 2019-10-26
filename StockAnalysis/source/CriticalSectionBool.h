#pragma once

#include"stdafx.h"

class CCriticalSectionBool {
public:
  CCriticalSectionBool() { m_fDoing = false; }
  ~CCriticalSectionBool() {}

  void SetFlag(bool fFlag);
  bool IsTrue(void);

protected:
  bool m_fDoing;                // 实时数据已预处理好，准备计算
  CCriticalSection m_Lock;
};
