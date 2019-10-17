#pragma once

#include"afxmt.h"

#include"RTData.h"

using namespace std;
#include<queue>

// �˱������ھ���ʹ��deque����priority_ueue��ʵ�ִ洢ʵʱ���ݵĶ��С��Ժ�Ӧ�ò���priority_queue��ʵ�֣���������ʵ�ֶ�������Դ��
extern bool gl_fUsingPriorityQueue;

class CSystemQueueData
{
public:
  CSystemQueueData();
  ~CSystemQueueData();
  void Reset(void);

  // ͨ�ýӿں���
  void              PushRTData(CStockRTDataPtr pData);
  CStockRTDataPtr   PopRTData(void);
  long              GetRTDataSize(void);

  // ���ö��еĺ���
  void              PushDequeRTData(CStockRTDataPtr pData);
  CStockRTDataPtr   PopDequeRTData(void);
  long              GetDequeRTDataSize(void);

  // ���������ȼ����еĺ���
  void              PushPriorityRTData(CStockRTDataPtr pData);
  CStockRTDataPtr   PopPriorityRTData(void);
  long              GetPriorityRTDataSize(void);

  // ��Ҫ���������ṹ���ṹ�����أ�����������������ȷ��ָ��Ĵ�С����ʱ��˳���С��������,��ͬʱ��İ��Ⱥ�����˳�����У���
  struct cmpRTData {
    bool operator() (const CStockRTDataPtr p1, const CStockRTDataPtr p2) const {
      // �����ȼ��Ķ���Ĭ������˳���ǴӴ�С���ʶ���С����������Ҫʹ�� > ���š�
      // ��ͬʱ������ݣ��ȷ��������λ�ں���������ǰ�档
      return(p1->GetTransactionTime() > p2->GetTransactionTime()); 
    }
  };

protected:
  // ��ϵͳ���µ��ɼ���Ԫʵʱ���ݺ󣬴˶�����Ҫ��Ϊ�����ȼ��Ķ��У����ȼ�Ϊʵʱ���ݵĽ���ʱ�䣬
  // �������ܹ����ദ�ɼ�����ʵʱ���ݰ���ʱ������Ϊһ�����С�
  // ����Ϊ������ priority_queue<CStockRTdataPtr, vector<CStockRTDataDataPtr>, cmpRTData> m_queueRTStockData;
  // �ṹcmpRTData����Ϊ��
  // struct cmpRTData {
  //   bool operator () (const CStockRTDataPtr p1, const CStockRTDataPtr p2) const {
  //     return(p1->GetTime() > p2->GetTime());
  //   }
  // };

  // Ŀǰʹ�ö���ʵ��
  queue<CStockRTDataPtr>  m_queueRTStockData;
  CCriticalSection        m_RTDataLock;

  // ׼��ʹ�������ȼ��Ķ���ʵ��
  priority_queue<CStockRTDataPtr, vector<CStockRTDataPtr>, cmpRTData>  m_priorityqueueRTStockData;
  CCriticalSection        m_PriorityRTDataLock;
};
