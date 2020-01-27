/////////////////////////////////////////////////////////////////////////////////////
//
// �洢ʵʱ���ݵľۺ��ࡣֻ��ͷ�ļ���
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
  CPriorityQueueRTData m_qSinaRTData; // ����ʵʱ���ݶ��С�
  CQueueRTData m_qSinaRTDataForSave; // ���ڴ洢������ʵʱ���ݶ���
  CPriorityQueueRTData m_qTengxunRTData; // ��Ѷʵʱ���ݶ��С�
  CPriorityQueueRTData m_qNeteaseRTData; // ����ʵʱ���ݶ��С�
};
