#pragma once

#include <afx.h>

#include"QueueWebData.h"

class CQueueWebInquire final : public CObject
{
public:
  long GetSinaRTDataSize(void) { return m_queueSinaRTWebData.GetWebDataSize(); }
  void PushSinaRTData(CWebDataReceivedPtr pData) { m_queueSinaRTWebData.PushWebData(pData); }
  CWebDataReceivedPtr PopSinaRTData(void) { return m_queueSinaRTWebData.PopWebData(); }
  long GetTengxunRTDataSize(void) { return m_queueTengxunRTWebData.GetWebDataSize(); }
  void PushTengxunRTData(CWebDataReceivedPtr pData) { m_queueTengxunRTWebData.PushWebData(pData); }
  CWebDataReceivedPtr PopTengxunRTData(void) { return m_queueTengxunRTWebData.PopWebData(); }
  long GetNeteaseRTDataSize(void) { return m_queueNeteaseRTWebData.GetWebDataSize(); }
  void PushNeteaseRTData(CWebDataReceivedPtr pData) { m_queueNeteaseRTWebData.PushWebData(pData); }
  CWebDataReceivedPtr PopNeteaseRTData(void) { return m_queueNeteaseRTWebData.PopWebData(); }
  long GetCrweberDataSize(void) { return m_queueCrweberWebData.GetWebDataSize(); }
  void PushCrweberData(CWebDataReceivedPtr pData) { m_queueCrweberWebData.PushWebData(pData); }
  CWebDataReceivedPtr PopCrweberData(void) { return m_queueCrweberWebData.PopWebData(); }

protected:
  CQueueWebData m_queueSinaRTWebData; // 新浪网络数据暂存队列
  CQueueWebData m_queueTengxunRTWebData; // 腾讯网络数据暂存队列
  CQueueWebData m_queueNeteaseRTWebData; // 网易网络数据暂存队列
  CQueueWebData m_queueCrweberWebData; // crweber.com网络数据暂存队列
};
