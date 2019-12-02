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

  // 通用接口函数
  void PushRTData(CRTDataPtr pData);
  CRTDataPtr PopRTData(void);
  long GetRTDataSize(void);

protected:
  queue<CRTDataPtr> m_queueRTStockData;
  CCriticalSection m_cs;
};
