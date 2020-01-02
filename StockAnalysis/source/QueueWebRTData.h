///////////////////////////////////////////////////////////
//
// ������У����ڴ洢����������ȡ�����ݡ�������ȡ���ݲ����߳�ģʽ��
// �ʶ��˶�����Ҫʹ�û��������Է�ֹ����ͬ�����⡣
//
///////////////////////////////////////////////////////////
#pragma once

#include"afxmt.h"

#include"WebDataReceived.h"

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
  void PushWebRTData(CWebDataReceivedPtr pData);
  CWebDataReceivedPtr PopWebRTData(void);
  long GetWebRTDataSize(void);

protected:
  queue<CWebDataReceivedPtr> m_queueWebRTData;
  mutex m_MutexAccessWebRTData;
};
