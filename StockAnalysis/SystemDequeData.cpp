#include"globedef.h"

#include "SystemDequeData.h"
#include "SystemMessage.h"


CSystemQueueData::CSystemQueueData()
{
  static int siCounter = 0;
  if (siCounter++ > 0) {
    TRACE("系统数据队列只允许一个实例\n");
    gl_systemMessage.PushInformationMessage(_T("错误：系统数据队列只允许生成一个实例"));
  }
}


CSystemQueueData::~CSystemQueueData()
{

}

void CSystemQueueData::Reset(void)
{
  CSingleLock singleLock(&m_RTDataLock);
  long lTotal = m_queueRTStockData.size();
  for (int i = 0; i < lTotal; i++) { // 清空队列
    m_queueRTStockData.pop(); 
  }
}

void CSystemQueueData::PushRTData(CStockRTDataPtr pData)
{
  CSingleLock singleLock(&m_RTDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_queueRTStockData.push(pData);
    singleLock.Unlock();
  }
}

CStockRTDataPtr CSystemQueueData::PopRTData(void)
{
  CStockRTDataPtr pData;
  CSingleLock singleLock(&m_RTDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    pData = m_queueRTStockData.front();
    m_queueRTStockData.pop();
    singleLock.Unlock();
    return pData;
  }
}

long CSystemQueueData::GetRTDataDequeSize(void)
{
  CSingleLock singleLock(&m_RTDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    const long lCount = m_queueRTStockData.size();
    singleLock.Unlock();
    return lCount;
  }
}
