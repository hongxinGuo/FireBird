#pragma once

#include"afxmt.h"

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
  void PushWebRTData(CWebRTDataPtr pData);
  CWebRTDataPtr PopWebRTData(void);
  long GetWebRTDataSize(void);

protected:
  queue<CWebRTDataPtr> m_queueWebRTData;
  mutex m_MutexAccessWebRTData;
};
