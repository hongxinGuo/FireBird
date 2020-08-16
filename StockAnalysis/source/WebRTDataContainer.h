/////////////////////////////////////////////////////////////////////////////////////
//
// �洢ʵʱ���ݵľۺ��ࡣֻ��ͷ�ļ���
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
  CPriorityQueueWebRTData m_qSinaRTData; // ����ʵʱ���ݶ��С�
  CQueueWebRTData m_qSinaRTDataForSave; // ���ڴ洢������ʵʱ���ݶ���
  CPriorityQueueWebRTData m_qTengxunRTData; // ��Ѷʵʱ���ݶ��С�
  CPriorityQueueWebRTData m_qNeteaseRTData; // ����ʵʱ���ݶ��С�
};
