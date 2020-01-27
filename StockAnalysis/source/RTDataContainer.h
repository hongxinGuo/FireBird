/////////////////////////////////////////////////////////////////////////////////////
//
// 存储实时数据的聚合类。只有头文件。
//
//
/////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"QueueRTData.h"
#include"PriorityQueueRTData.h"

class CRTDataContainer final : public CObject {
public:
  void ResetSinaRTQueue(void) { m_qSinaRTData.Reset(); }
  long GetSinaRTDataSize(void) { return m_qSinaRTData.GetRTDataSize(); }
  void PushSinaRTData(CRTDataPtr pData) { m_qSinaRTData.PushRTData(pData); }
  CRTDataPtr PopSinaRTData(void) { return m_qSinaRTData.PopRTData(); }
  long GetTengxunRTDataSize(void) { return m_qTengxunRTData.GetRTDataSize(); }
  void PushTengxunRTData(CRTDataPtr pData) { m_qTengxunRTData.PushRTData(pData); }
  CRTDataPtr PopTengxunRTData(void) { return m_qTengxunRTData.PopRTData(); }
  long GetNeteaseRTDataSize(void) { return m_qNeteaseRTData.GetRTDataSize(); }
  void PushNeteaseRTData(CRTDataPtr pData) { m_qNeteaseRTData.PushRTData(pData); }
  CRTDataPtr PopNeteaseRTData(void) { return m_qNeteaseRTData.PopRTData(); }

protected:
  CPriorityQueueRTData m_qSinaRTData; // 新浪实时数据队列。
  CQueueRTData m_qSinaRTDataForSave; // 用于存储的新浪实时数据队列
  CPriorityQueueRTData m_qTengxunRTData; // 腾讯实时数据队列。
  CPriorityQueueRTData m_qNeteaseRTData; // 网易实时数据队列。
};
