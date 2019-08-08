#pragma once

#include"afxmt.h"

#include"RTData.h"

using namespace std;
#include<queue>

class CSystemQueueData
{
public:
  CSystemQueueData();
  ~CSystemQueueData();
  void Reset(void);

  void              PushRTData(CStockRTDataPtr pData);
  CStockRTDataPtr   PopRTData(void);
  long              GetRTDataDequeSize(void);

protected:
  queue<CStockRTDataPtr>  m_queueRTStockData;
  CCriticalSection        m_RTDataLock;
};

