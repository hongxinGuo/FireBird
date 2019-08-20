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
  // ��ϵͳ���µ��ɼ���Ԫʵʱ���ݺ󣬴˶�����Ҫ��Ϊ�����ȼ��Ķ��У����ȼ�Ϊʵʱ���ݵĽ���ʱ�䣬
  // �������ܹ����ദ�ɼ�����ʵʱ���ݰ���ʱ������Ϊһ�����С�
  // ����Ϊ������ priority_queue<CStockRTdataPtr, vector<CStockRTDataDataPtr>, greater<CStockRTDataPtr>> m_queueRTStockData;
  // ָ��CStockRTDataPtr��Ҫ���� < ���������������ܿ϶��ܹ���ȷ���㣬�п����ǱȽ���ָ��ֵ�Ĵ�С����Ҫ��֤��
  queue<CStockRTDataPtr>  m_queueRTStockData; 
  CCriticalSection        m_RTDataLock;
};

