#pragma once

#include"stdafx.h"

class CVirtualMarket : public CObject {
public:
  CVirtualMarket(void);
  ~CVirtualMarket(void);

  virtual bool SchedulingTask(void) = 0; // �ɳ���Ķ�ʱ�����ȣ���Լÿ100����һ��

  bool IsReadyToRun(void) noexcept { return m_fReadyToRun; }
  void SetReadyToRun(bool fFlag) noexcept { m_fReadyToRun = fFlag; }

protected:
  bool m_fReadyToRun;
};
