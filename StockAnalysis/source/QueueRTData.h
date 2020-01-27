#pragma once

#include"RTData.h"

using namespace std;
#include<queue>
#include<mutex>

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
  queue<CRTDataPtr> m_qRTStockData;
  mutex m_MutexAccessRTData;
};
