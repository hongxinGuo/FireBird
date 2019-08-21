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

  void              PushPriorityRTData(CStockRTDataPtr pData);
  CStockRTDataPtr   PopPriorityRTData(void);
  long              GetPriorityRTDataDequeSize(void);
  
  struct cmpRTData {
    bool operator () (const CStockRTDataPtr p1, const CStockRTDataPtr p2) const {
      return p1->GetTime() > p2->GetTime();
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

  queue<CStockRTDataPtr>  m_queueRTStockData;
  CCriticalSection        m_RTDataLock;
  
  priority_queue<CStockRTDataPtr, vector<CStockRTDataPtr>, cmpRTData>  m_priorityqueueRTStockData;
  CCriticalSection        m_PriorityRTDataLock;
};

