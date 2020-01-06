#pragma once

#include <afx.h>

#include"QueueWebData.h"

class CQueueWebInquire final : public CObject
{
public:
  long GetSinaRTDataSize(void) { return m_QueueSinaRTWebData.GetWebDataSize(); }
  void PushSinaRTData(CWebDataReceivedPtr pData) { m_QueueSinaRTWebData.PushWebData(pData); }
  CWebDataReceivedPtr PopSinaRTData(void) { return m_QueueSinaRTWebData.PopWebData(); }
  long GetTengxunRTDataSize(void) { return m_QueueTengxunRTWebData.GetWebDataSize(); }
  void PushTengxunRTData(CWebDataReceivedPtr pData) { m_QueueTengxunRTWebData.PushWebData(pData); }
  CWebDataReceivedPtr PopTengxunRTData(void) { return m_QueueTengxunRTWebData.PopWebData(); }
  long GetNeteaseRTDataSize(void) { return m_QueueNeteaseRTWebData.GetWebDataSize(); }
  void PushNeteaseRTData(CWebDataReceivedPtr pData) { m_QueueNeteaseRTWebData.PushWebData(pData); }
  CWebDataReceivedPtr PopNeteaseRTData(void) { return m_QueueNeteaseRTWebData.PopWebData(); }
  long GetCrweberDataSize(void) { return m_QueueCrweberWebData.GetWebDataSize(); }
  void PushCrweberData(CWebDataReceivedPtr pData) { m_QueueCrweberWebData.PushWebData(pData); }
  CWebDataReceivedPtr PopCrweberData(void) { return m_QueueCrweberWebData.PopWebData(); }

protected:
  CQueueWebData m_QueueSinaRTWebData; // 新浪网络数据暂存队列
  CQueueWebData m_QueueTengxunRTWebData; // 腾讯网络数据暂存队列
  CQueueWebData m_QueueNeteaseRTWebData; // 网易网络数据暂存队列
  CQueueWebData m_QueueCrweberWebData; // crweber.com网络数据暂存队列
};
