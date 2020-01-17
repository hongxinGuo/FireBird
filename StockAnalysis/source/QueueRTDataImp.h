#pragma once

#include"RTData.h"

using namespace std;
#include<queue>
#include<mutex>

class CQueueRTDataImp final
{
public:
  CQueueRTDataImp();
  ~CQueueRTDataImp();
  void Reset(void);

  // ͨ�ýӿں���
  void PushRTData(CRTDataPtr pData);
  CRTDataPtr PopRTData(void);
  long GetRTDataSize(void);

protected:
  queue<CRTDataPtr> m_queueRTStockData;
  mutex m_MutexAccessRTData;
};
