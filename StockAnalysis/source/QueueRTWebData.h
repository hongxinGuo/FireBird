#pragma once

#include"afxmt.h"

#include"RTWebData.h"

using namespace std;
#include<queue>
#include<mutex>

class CQueueRTWebData final
{
public:
  CQueueRTWebData();
  ~CQueueRTWebData();
  void Reset(void);

  // ͨ�ýӿں���
  void PushRTWebData(CRTWebDataPtr pData);
  CRTWebDataPtr PopRTWebData(void);
  long GetRTWebDataSize(void);

protected:
  queue<CRTWebDataPtr> m_queueRTWebData;
  mutex m_MutexAccessRTWebData;
};
