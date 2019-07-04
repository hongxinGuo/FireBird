#include "SystemDequeData.h"



CSystemDequeData::CSystemDequeData()
{
  static int siCounter = 0;
  if (siCounter > 1) {
    TRACE("ϵͳ��Ϣֻ����һ��ʵ��\n");
  }
  else siCounter++;

}


CSystemDequeData::~CSystemDequeData()
{
  m_dequeRTStockData.clear();
}

void CSystemDequeData::PushRTData(CStockRTDataPtr pData)
{
  CSingleLock singleLock(&m_RTDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    m_dequeRTStockData.push_back(pData);
    singleLock.Unlock();
  }
}

CStockRTDataPtr CSystemDequeData::PopRTData(void)
{
  CStockRTDataPtr pData;
  CSingleLock singleLock(&m_RTDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    pData = m_dequeRTStockData.front();
    m_dequeRTStockData.pop_front();
    singleLock.Unlock();
    return pData;
  }
  return nullptr;
}

long CSystemDequeData::GetRTDataDequeSize(void)
{
  CSingleLock singleLock(&m_RTDataLock);
  singleLock.Lock();
  if (singleLock.IsLocked()) {
    long lCount = m_dequeRTStockData.size();
    singleLock.Unlock();
    return lCount;
  }
  return 0;

}
