///////////////////////////////////////////////////////////
//
// ������У����ڴ洢����������ȡ�����ݡ�������ȡ���ݲ����߳�ģʽ��
// �ʶ��˶�����Ҫʹ�û��������Է�ֹ����ͬ�����⡣
//
///////////////////////////////////////////////////////////
#pragma once

#include"afxmt.h"

#include"WebData.h"

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
  void PushWebData(CWebDataPtr pData);
  CWebDataPtr PopWebData(void);
  size_t GetWebDataSize(void);

protected:
  queue<CWebDataPtr> m_qWebData;
  mutex m_MutexAccessWebData;
};
