#pragma once

#include"WebRTData.h"

using namespace std;
#include<queue>
#include<mutex>

class CQueueWebRTData final
{
public:
  CQueueWebRTData();
  ~CQueueWebRTData();
  void Reset(void);

  // ͨ�ýӿں���
  void PushData(CWebRTDataPtr pData);
  CWebRTDataPtr PopData(void);
  size_t GetDataSize(void);

protected:
  queue<CWebRTDataPtr> m_qStockData;
  mutex m_MutexAccessData;
};
