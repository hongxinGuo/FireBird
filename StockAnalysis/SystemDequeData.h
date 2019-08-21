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

  // 采用队列的函数
  void              PushRTData(CStockRTDataPtr pData);
  CStockRTDataPtr   PopRTData(void);
  long              GetRTDataDequeSize(void);

  // 这是采用有优先级队列的函数
  void              PushPriorityRTData(CStockRTDataPtr pData);
  CStockRTDataPtr   PopPriorityRTData(void);
  long              GetPriorityRTDataDequeSize(void);
  
  // 需要定义下述结构，结构中重载（）运算符，定义如何确定指针的大小（按时间顺序从小到大排列）。
  struct cmpRTData {
    bool operator () (const CStockRTDataPtr p1, const CStockRTDataPtr p2) const {
      return p1->GetTransactionTime() > p2->GetTransactionTime(); // 有优先级的队列默认排列顺序是从大到小，故而从小到大排列需要使用 > 符号。
    }
  };

protected:
  // 当系统更新到采集多元实时数据后，此队列需要改为有优先级的队列，优先级为实时数据的交易时间，
  // 这样就能够将多处采集到的实时数据按照时间排列为一个队列。
  // 大致为这样： priority_queue<CStockRTdataPtr, vector<CStockRTDataDataPtr>, cmpRTData> m_queueRTStockData;
  // 结构cmpRTData定义为：
  // struct cmpRTData {
  //   bool operator () (const CStockRTDataPtr p1, const CStockRTDataPtr p2) const {
  //     return p1->GetTime() < p2->GetTime();
  //   }
  // };

  // 目前使用队列实现
  queue<CStockRTDataPtr>  m_queueRTStockData;
  CCriticalSection        m_RTDataLock;
  
  // 准备使用有优先级的队列实现
  priority_queue<CStockRTDataPtr, vector<CStockRTDataPtr>, cmpRTData>  m_priorityqueueRTStockData;
  CCriticalSection        m_PriorityRTDataLock;
};

