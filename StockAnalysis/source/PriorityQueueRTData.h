#pragma once

#include"RTData.h"

using namespace std;
#include<queue>

// 此变量用于决定使用deque还是priority_ueue来实现存储实时数据的队列。以后应该采用priority_queue来实现，这样可以实现多数据来源。
// 目前使用deque队列存储实时数据，故此变量初始值设置为假。在测试使用priority_queue成功后，即弃用deque(此标识永远为真）。
extern bool gl_fUsingPriorityQueue;

class CPriorityQueueRTData final
{
public:
  CPriorityQueueRTData();
  ~CPriorityQueueRTData();
  void Reset(void);

  // 通用接口函数
  void PushRTData(CRTDataPtr pData);
  CRTDataPtr PopRTData(void);
  long GetRTDataSize(void);

protected:
  // 需要定义下述结构，结构中重载（）运算符，定义如何确定指针的大小（按时间顺序从小到大排列,相同时间的按先后放入的顺序排列）。
  struct cmpRTData {
    bool operator() (const CRTDataPtr p1, const CRTDataPtr p2) const {
      // 有优先级的队列默认排列顺序是从大到小，故而从小到大排列需要使用 > 符号。
      // 相同时间的数据，先放入的数据位于后放入的数据前面。
      return(p1->GetTransactionTime() > p2->GetTransactionTime());
    }
  };

protected:
  // 当系统更新到采集多元实时数据后，此队列需要改为有优先级的队列，优先级为实时数据的交易时间，
  // 这样就能够将多处采集到的实时数据按照时间排列为一个队列。
  // 大致为这样： priority_queue<CStockRTdataPtr, vector<CStockRTDataDataPtr>, cmpRTData> m_queueRTStockData;
  // 结构cmpRTData定义为：
  // struct cmpRTData {
  //   bool operator () (const CRTDataPtr p1, const CRTDataPtr p2) const {
  //     return(p1->GetTime() > p2->GetTime());
  //   }
  // };
  priority_queue<CRTDataPtr, vector<CRTDataPtr>, cmpRTData> m_priorityqueueRTStockData;
};