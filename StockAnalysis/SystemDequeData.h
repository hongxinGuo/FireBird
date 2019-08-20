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
  // 当系统更新到采集多元实时数据后，此队列需要改为有优先级的队列，优先级为实时数据的交易时间，
  // 这样就能够将多处采集到的实时数据按照时间排列为一个队列。
  // 大致为这样： priority_queue<CStockRTdataPtr, vector<CStockRTDataDataPtr>, greater<CStockRTDataPtr>> m_queueRTStockData;
  // 指针CStockRTDataPtr需要重载 < 运算符。（这个不能肯定能够正确运算，有可能是比较了指针值的大小，需要验证）
  queue<CStockRTDataPtr>  m_queueRTStockData; 
  CCriticalSection        m_RTDataLock;
};

