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

  // ���ö��еĺ���
  void              PushRTData(CStockRTDataPtr pData);
  CStockRTDataPtr   PopRTData(void);
  long              GetRTDataDequeSize(void);

  // ���ǲ��������ȼ����еĺ���
  void              PushPriorityRTData(CStockRTDataPtr pData);
  CStockRTDataPtr   PopPriorityRTData(void);
  long              GetPriorityRTDataDequeSize(void);
  
  // ��Ҫ���������ṹ���ṹ�����أ�����������������ȷ��ָ��Ĵ�С����ʱ��˳���С�������У���
  struct cmpRTData {
    bool operator () (const CStockRTDataPtr p1, const CStockRTDataPtr p2) const {
      return p1->GetTransactionTime() > p2->GetTransactionTime(); // �����ȼ��Ķ���Ĭ������˳���ǴӴ�С���ʶ���С����������Ҫʹ�� > ���š�
    }
  };

protected:
  // ��ϵͳ���µ��ɼ���Ԫʵʱ���ݺ󣬴˶�����Ҫ��Ϊ�����ȼ��Ķ��У����ȼ�Ϊʵʱ���ݵĽ���ʱ�䣬
  // �������ܹ����ദ�ɼ�����ʵʱ���ݰ���ʱ������Ϊһ�����С�
  // ����Ϊ������ priority_queue<CStockRTdataPtr, vector<CStockRTDataDataPtr>, cmpRTData> m_queueRTStockData;
  // �ṹcmpRTData����Ϊ��
  // struct cmpRTData {
  //   bool operator () (const CStockRTDataPtr p1, const CStockRTDataPtr p2) const {
  //     return p1->GetTime() < p2->GetTime();
  //   }
  // };

  // Ŀǰʹ�ö���ʵ��
  queue<CStockRTDataPtr>  m_queueRTStockData;
  CCriticalSection        m_RTDataLock;
  
  // ׼��ʹ�������ȼ��Ķ���ʵ��
  priority_queue<CStockRTDataPtr, vector<CStockRTDataPtr>, cmpRTData>  m_priorityqueueRTStockData;
  CCriticalSection        m_PriorityRTDataLock;
};

