#pragma once

#include"stdafx.h"

class CCriticalSectionBool {
public:
  CCriticalSectionBool() { m_fDoing = false; }
  ~CCriticalSectionBool() {}

  void SetFlag(bool fFlag);
  bool IsTrue(void);

protected:
  bool m_fDoing;                // ʵʱ������Ԥ����ã�׼������
  CCriticalSection m_Lock;
};
