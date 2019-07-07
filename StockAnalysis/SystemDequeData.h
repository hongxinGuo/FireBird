#pragma once

#include"afxmt.h"

#include"RTData.h"

using namespace std;
#include<deque>

class CSystemDequeData
{
public:
  CSystemDequeData();
  ~CSystemDequeData();

  void              PushRTData(CStockRTDataPtr pData);
  CStockRTDataPtr   PopRTData(void);
  long              GetRTDataDequeSize(void);

protected:
  deque<CStockRTDataPtr>  m_dequeRTStockData;
  CCriticalSection        m_RTDataLock;
};

