///////////////////////////////////////////////////////////
//
// ������У����ڴ洢����������ȡ�����ݡ�������ȡ���ݲ����߳�ģʽ��
// �ʶ��˶�����Ҫʹ�û��������Է�ֹ����ͬ�����⡣
//
///////////////////////////////////////////////////////////
#pragma once

#include"afxmt.h"

#include"ReceivedData.h"

using namespace std;
#include<queue>
#include<mutex>

class CQueueWebData final
{
public:
  CQueueWebData();
  ~CQueueWebData();
  void Reset(void);

  // ͨ�ýӿں���
  void PushWebData(CWebDataReceivedPtr pData);
  CWebDataReceivedPtr PopWebData(void);
  long GetWebDataSize(void);

protected:
  queue<CWebDataReceivedPtr> m_qWebData;
  mutex m_MutexAccessWebData;
};
