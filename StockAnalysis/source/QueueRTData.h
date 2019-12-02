#pragma once

#include"afxmt.h"

#include"RTData.h"

using namespace std;
#include<queue>

class CQueueRTData final
{
public:
  CQueueRTData();
  ~CQueueRTData();
  void Reset(void);

  // ͨ�ýӿں���
  void PushRTData(CRTDataPtr pData);
  CRTDataPtr PopRTData(void);
  long GetRTDataSize(void);

protected:
  queue<CRTDataPtr> m_queueRTStockData;
  CCriticalSection m_cs;
};
