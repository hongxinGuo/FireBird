/////////////////////////////////////////////////////////////////////////////////////
//
// �洢ʵʱ���ݵľۺ��ࡣֻ��ͷ�ļ���
//
//
/////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"QueueRTDataImp.h"
#include"PriorityQueueRTData.h"

class CQueueRTData final : public CObject {
public:
  void ResetSinaRTQueue(void) { m_queueSinaRTData.Reset(); }
  long GetSinaRTDataSize(void) { return m_queueSinaRTData.GetRTDataSize(); }
  void PushSinaRTData(CRTDataPtr pData) { m_queueSinaRTData.PushRTData(pData); }
  CRTDataPtr PopSinaRTData(void) { return m_queueSinaRTData.PopRTData(); }
  long GetTengxunRTDataSize(void) { return m_queueTengxunRTData.GetRTDataSize(); }
  void PushTengxunRTData(CRTDataPtr pData) { m_queueTengxunRTData.PushRTData(pData); }
  CRTDataPtr PopTengxunRTData(void) { return m_queueTengxunRTData.PopRTData(); }
  long GetNeteaseRTDataSize(void) { return m_queueNeteaseRTData.GetRTDataSize(); }
  void PushNeteaseRTData(CRTDataPtr pData) { m_queueNeteaseRTData.PushRTData(pData); }
  CRTDataPtr PopNeteaseRTData(void) { return m_queueNeteaseRTData.PopRTData(); }

protected:
  CPriorityQueueRTData m_queueSinaRTData; // ����ʵʱ���ݶ��С�
  CQueueRTDataImp m_queueSinaRTDataForSave; // ���ڴ洢������ʵʱ���ݶ���
  CPriorityQueueRTData m_queueTengxunRTData; // ��Ѷʵʱ���ݶ��С�
  CPriorityQueueRTData m_queueNeteaseRTData; // ����ʵʱ���ݶ��С�
};
