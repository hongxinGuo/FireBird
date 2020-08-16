/////////////////////////////////////////////////////////////////////////////////////
//
// 存储实时数据的聚合类。只有头文件。
//
//
/////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"QueueWebRTData.h"
#include"PriorityQueueWebRTData.h"

class CWebRTDataContainer final : public CObject {
public:
  CWebRTDataContainer();
  ~CWebRTDataContainer();
  void ResetSinaQueue(void) { m_qSinaRTData.Reset(); }
  size_t GetSinaDataSize(void) { return m_qSinaRTData.GetDataSize(); }
  void PushSinaData(CWebRTDataPtr pData) { m_qSinaRTData.PushData(pData); }
  CWebRTDataPtr PopSinaData(void) { return m_qSinaRTData.PopData(); }
  size_t GetTengxunDataSize(void) { return m_qTengxunRTData.GetDataSize(); }
  void PushTengxunData(CWebRTDataPtr pData) { m_qTengxunRTData.PushData(pData); }
  CWebRTDataPtr PopTengxunData(void) { return m_qTengxunRTData.PopData(); }
  size_t GetNeteaseDataSize(void) { return m_qNeteaseRTData.GetDataSize(); }
  void PushNeteaseData(CWebRTDataPtr pData) { m_qNeteaseRTData.PushData(pData); }
  CWebRTDataPtr PopNeteaseData(void) { return m_qNeteaseRTData.PopData(); }

protected:
  CPriorityQueueWebRTData m_qSinaRTData; // 新浪实时数据队列。
  CQueueWebRTData m_qSinaRTDataForSave; // 用于存储的新浪实时数据队列
  CPriorityQueueWebRTData m_qTengxunRTData; // 腾讯实时数据队列。
  CPriorityQueueWebRTData m_qNeteaseRTData; // 网易实时数据队列。
};
