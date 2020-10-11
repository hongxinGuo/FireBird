#pragma once

#include"WebRTData.h"

using namespace std;
#include<queue>

class CPriorityQueueWebRTData final
{
public:
  CPriorityQueueWebRTData();
  ~CPriorityQueueWebRTData();
  void Reset(void);

  // ͨ�ýӿں���
  void PushData(CWebRTDataPtr pData);
  CWebRTDataPtr PopData(void);
  CWebRTDataPtr GetHead(void) noexcept { return m_priorityqueueStockData.top(); }
  size_t GetDataSize(void);

protected:
  // ��Ҫ���������ṹ���ṹ�����أ�����������������ȷ��ָ��Ĵ�С����ʱ��˳���С��������,��ͬʱ��İ��Ⱥ�����˳�����У���
  struct cmpRTData {
    bool operator() (const CWebRTDataPtr p1, const CWebRTDataPtr p2) const noexcept {
      // �����ȼ��Ķ���Ĭ������˳���ǴӴ�С���ʶ���С����������Ҫʹ�� > ���š�
      // ��ͬʱ������ݣ��ȷ��������λ�ں���������ǰ�档
      return(p1->GetTransactionTime() > p2->GetTransactionTime());
    }
  };

protected:
  // ��ϵͳ���µ��ɼ���Ԫʵʱ���ݺ󣬴˶�����Ҫ��Ϊ�����ȼ��Ķ��У����ȼ�Ϊʵʱ���ݵĽ���ʱ�䣬
  // �������ܹ����ദ�ɼ�����ʵʱ���ݰ���ʱ������Ϊһ�����С�
  // ����Ϊ������ priority_queue<CStockRTdataPtr, vector<CStockRTDataDataPtr>, cmpRTData> m_qRTStockData;
  // �ṹcmpRTData����Ϊ��
  // struct cmpRTData {
  //   bool operator () (const CWebRTDataPtr p1, const CWebRTDataPtr p2) const {
  //     return(p1->GetFormatedMarketTime() > p2->GetFormatedMarketTime());
  //   }
  // };
  priority_queue<CWebRTDataPtr, vector<CWebRTDataPtr>, cmpRTData> m_priorityqueueStockData;
};