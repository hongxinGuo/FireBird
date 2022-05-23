/////////////////////////////////////////////////////////////////////////////////////
//
// �洢ʵʱ���ݵľۺ��ࡣֻ��ͷ�ļ���
//
// �˶����д洢���Ǿ��������ʵʱ���ݣ�ÿ������Ϊһ����Ʊ�ļ�ʱ����״̬��
//
/////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include"PriorityQueueWebRTData.h"

class CWebRTDataContainer final : public CObject {
public:
	CWebRTDataContainer();
	// ������ֵ��
	CWebRTDataContainer(const CWebRTDataContainer&) = delete;
	CWebRTDataContainer& operator=(const CWebRTDataContainer&) = delete;
	CWebRTDataContainer(const CWebRTDataContainer&&) noexcept = delete;
	CWebRTDataContainer& operator=(const CWebRTDataContainer&&) noexcept = delete;
	~CWebRTDataContainer();

	void ResetSinaQueue(void) { m_qSinaRTData.Reset(); }
	size_t SinaDataSize(void) { return m_qSinaRTData.Size(); }
	void PushSinaData(CWebRTDataPtr pData) { m_qSinaRTData.PushData(pData); }
	CWebRTDataPtr PopSinaData(void) { return m_qSinaRTData.PopData(); }
	size_t TengxunDataSize(void) { return m_qTengxunRTData.Size(); }
	void PushTengxunData(CWebRTDataPtr pData) { m_qTengxunRTData.PushData(pData); }
	CWebRTDataPtr PopTengxunData(void) { return m_qTengxunRTData.PopData(); }
	size_t NeteaseDataSize(void) { return m_qNeteaseRTData.Size(); }
	void PushNeteaseData(CWebRTDataPtr pData) { m_qNeteaseRTData.PushData(pData); }
	CWebRTDataPtr PopNeteaseData(void) { return m_qNeteaseRTData.PopData(); }

protected:
	CPriorityQueueWebRTData m_qSinaRTData; // ����ʵʱ���ݶ��С�
	CPriorityQueueWebRTData m_qTengxunRTData; // ��Ѷʵʱ���ݶ��С�
	CPriorityQueueWebRTData m_qNeteaseRTData; // ����ʵʱ���ݶ��С�
};
