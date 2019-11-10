#pragma once

#include"afxmt.h"

#include"RTData.h"

using namespace std;
#include<queue>

// �˱������ھ���ʹ��deque����priority_ueue��ʵ�ִ洢ʵʱ���ݵĶ��С��Ժ�Ӧ�ò���priority_queue��ʵ�֣���������ʵ�ֶ�������Դ��
// Ŀǰʹ��deque���д洢ʵʱ���ݣ��ʴ˱�����ʼֵ����Ϊ�١��ڲ���ʹ��priority_queue�ɹ��󣬼�����deque(�˱�ʶ��ԶΪ�棩��
extern bool gl_fUsingPriorityQueue;

class CQueueRTData final
{
public:
  CQueueRTData();
  ~CQueueRTData();
  void Reset(void);

  // ͨ�ýӿں���
  void PushRTData(CRTDataPtr pData);
  CRTDataPtr PopRTData(void);
  long GetRTDataSize(void);

protected:
  // ���ö��еĺ���
  void PushDequeRTData(CRTDataPtr pData);
  CRTDataPtr PopDequeRTData(void);
  long GetDequeRTDataSize(void);

  // ���������ȼ����еĺ���
  void PushPriorityRTData(CRTDataPtr pData);
  CRTDataPtr PopPriorityRTData(void);
  long GetPriorityRTDataSize(void);

  // ��Ҫ���������ṹ���ṹ�����أ�����������������ȷ��ָ��Ĵ�С����ʱ��˳���С��������,��ͬʱ��İ��Ⱥ�����˳�����У���
  struct cmpRTData {
    bool operator() (const CRTDataPtr p1, const CRTDataPtr p2) const {
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
  //   bool operator () (const CRTDataPtr p1, const CRTDataPtr p2) const {
  //     return(p1->GetTime() > p2->GetTime());
  //   }
  // };

  // Ŀǰʹ�ö���ʵ�֣�ͣ�ã�
  queue<CRTDataPtr> m_queueRTStockData;
  CCriticalSection m_RTDataLock;

  // ׼��ʹ�������ȼ��Ķ���ʵ��(��ʵ�֣�
  priority_queue<CRTDataPtr, vector<CRTDataPtr>, cmpRTData> m_priorityqueueRTStockData;
  CCriticalSection m_PriorityRTDataLock;
};
